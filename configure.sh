#!/bin/bash

DEPTOOL=cpp
BUILDLOG='.build.log'

all_flags='CPPFLAGS CFLAGS CXXFLAGS LDFLAGS ASFLAGS'
unset rm_files
unset obj_names

QT_UIC="${UIC:=uic}"
QT_MOC="${MOC:=moc}"
QT_RCC="${RCC:=rcc}"
QT_LUPDATE="${QT_LUPDATE:=lupdate}"
QT_LRELEASE="${QT_LRELEASE:=lrelease}"

# utility function used by 'sources' script
check_library()
{
 local tmpname cname result
 tmpname=`mktemp`
 cname="$tmpname.c"
 mv $tmpname $cname
 echo -e "int main() { $1(); return 0; }" >$cname
 tmpname=`mktemp`
 cc $cname -o $tmpname "-l$2" 2>/dev/null
 result=$?
 rm -f $tmpname $cname
 return $result
}

# utility function used by 'sources' script
find_library()
{
 unset __found
 local name lib result
 while true ; do
  name=$1
  lib=$2
  [ -z "$lib" ] && return
  check_library $name $lib
  if [ $? -eq 0 ] ; then
   __found=$lib
   return
  fi
  shift 2
 done
}

check_rule()
{
 create_rule=
 local decorated="<$target_type|$check_target>"
 if [ "${__all_rules/$decorated//}" == "${__all_rules}" ]; then
  __all_rules="${__all_rules}$decorated"
  create_rule=yes
 fi
}

get_deps()
{
 basename $src | grep -E "^(moc|qrc)_" >/dev/null
 if [ $? -eq 0 ]; then
  deps="$obj: $src"
  return
 fi
 if [ ! -f "$src" ]; then
  echo "$src: File not found" >&2
  exit 1
 fi
 deps=$($DEPTOOL ${CPPFLAGS}${target_cppflags} -MM $src) 
 if [ $? -ne 0 ]; then
  echo "Preprocessing failed!" >&2
  rm -f ./$OUTFILE
  exit 1
 fi
}

get_obj_name()
{
 local name=$1 
 local path=$2
 obj_unmodified="$name.o"
 if [ "${obj_names/|$name|/}" = "$obj_names" ]; then
  obj_names="$obj_names|$name|"
  obj="$obj_unmodified"
  return
 fi
 if [ $(command -v cksum) ]; then
  local cksum=$(cksum <<< $path)
  cksum=${cksum// /_}
  obj="${name}_$cksum.o"
  return
 fi
 local tmpfile=`mktemp`
 rm $tmpfile
 tmpfile=${tmpfile//\//_}
 obj="${name}_$tmpfile.o"
}

check_src_ext()
{
 local src_base=$1
 shift
 local ext
 for ext in $* ; do
  src_noext=${src_base%.$ext}
  [ "$src_noext" != "$src_base" ] && return 0
 done
 return 1 
}

parse_sources()
{
 local tool flags errfile src src_base cmd cmd2
 for src in ${use_sources} ; do  
  src_base=$(basename $src)

  unset cmd obj obj_unmodified deps src_noext

  if check_src_ext "$src_base" ui ; then
   continue
  fi

  # C source?
  if check_src_ext "$src_base" c ; then
   cmd="\$(CC) -c \$(CPPFLAGS)${target_cppflags} \$(CFLAGS) \$< -o \$@"
   tool='CC      '
   flags='.CFLAGS'
   get_obj_name "$src_noext" "$src"
   [ -n "$use_deps" ] && get_deps  
  # C++ source?
  elif check_src_ext "$src_base" cpp cxx c++ C ; then
   cmd="\$(CXX) -c \$(CPPFLAGS)${target_cppflags} \$(CXXFLAGS) \$< -o \$@"
   tool='CXX     '
   flags='.CXXFLAGS'
   get_obj_name "$src_noext" "$src"
   [ -n "$use_deps" ] && get_deps
  # ASM + CPP source? 
  elif check_src_ext "$src_base" S ; then
   cmd="cpp \$(CPPFLAGS)${target_cppflags} \$< | \$(AS) \$(ASFLAGS) -o \$@"
   tool='AS      '
   flags='.ASFLAGS'
   get_obj_name "$src_noext" "$src"
   [ -n "$use_deps" ] && get_deps
  # ASM source? 
  elif check_src_ext "$src_base" s asm ; then
   cmd="\$(AS) \$(ASFLAGS) -o \$@ \$<"
   tool='AS      '
   flags='.ASFLAGS'
   get_obj_name "$src_noext" "$src"
  fi

  if [ -z "$obj" ]; then
   echo "Don't know what to do with $src_base" >&2
   rm -f ./$OUTFILE
   exit 1
  fi

  errfile="$CONFIG/$obj.err"
  cmd="@echo \"  $tool$src_base\"\n\t@echo '$cmd' >> $CONFIG/$BUILDLOG\n\t@$cmd 2>$errfile; status=\$\$?; if [ -s \"$errfile\" ]; then cat $errfile >> $CONFIG/$BUILDLOG; cat $errfile >&2; rm -f $errfile; [ \$\$status ] && exit \$\$status; fi"
  cmd2="@-rm -f $errfile"
  check_target="$CONFIG/$obj"
  target_type=O
  objs="$objs $check_target"
  check_rule
  if [ -n "$create_rule" ]; then
   [ -z "$deps" ] && deps="$obj: $src" # no use_deps
   local add_cppflags
   [ -n "$target_cppflags" ] && add_cppflags=" $CONFIG/.${sane_target}_CPPFLAGS"
   [ "$obj_unmodified" != "$obj" ] && deps=${deps/$obj_unmodified/$obj}
   echo -e "$CONFIG/$deps $CONFIG/$flags $CONFIG/.CPPFLAGS${add_cppflags}\\n\\t$cmd\\n\\t$cmd2\\n" >> $OUTFILE
  fi
  echo "$src_base"
 done
}

print_flags_rule()
{
 local flags=$1
 local contents=$2
 local filename="$CONFIG/.$flags"
 local tmpfile=$(TMPDIR=/tmp mktemp)
 local tempname=${tmpfile/\/tmp/$CONFIG}-$flags
 local output="$filename: $tempname
\\tdiff --brief $filename $tempname >/dev/null 2>/dev/null; if [ \$\$? -eq 0 ]; then rm -f $tempname; else mv -f $tempname $filename; fi

$tempname:
\\techo $contents > $tempname
"
 echo -e "$output" >> $OUTFILE
 silent="$silent $filename $tempname"
 rm -f $tmpfile
 unset flags
}

print_flags_rules()
{
 local flags=$1
 local target
 print_flags_rule $flags "\$($flags)"
 if [ -n "$multiple_targets" ]; then
  for target in $TARGETS; do
   sane_target="${target/./_}"
   name="${sane_target}_$flags"
   local contents="${!name}"
   [ -n "$contents" ] && print_flags_rule $name "'$contents'"
  done
 fi
}

unset custom_makefile
__all_rules=

while getopts "m:" option ; do
 [ "$option" == "?" ] && exit 1
 [ "$option" == "m" ] && custom_makefile=$OPTARG
done
shift $(($OPTIND-1))
srcfile=$1

# default input file: "sources"
[ -z "$srcfile" ] && srcfile="./sources"

if [ ! -f "$srcfile" ]; then
 echo "$srcfile file is not found!"
 exit 1
fi

. $srcfile

if [ -n "$custom_makefile" ]; then
 OUTFILE=$custom_makefile
elif [ -n "$MAKEFILE" ]; then
 OUTFILE=$MAKEFILE
else
 OUTFILE="$CONFIG.mak"
fi

multiple_targets='yes'
if [ -z "$TARGETS" -a -n "$TARGET" ]; then
 TARGETS="$TARGET"
 multiple_targets=''
fi
if [ -z "$CONFIG" -o -z "$TARGETS" ]; then
 echo '$CONFIG and $TARGET (or $TARGETS) must be defined!' >&2
 rm -f ./$OUTFILE
 exit 1
fi

if [ "$CONFIG" == "$TARGET" ]; then
 echo '$CONFIG and $TARGET must not be the same!' >&2
 rm -f ./$OUTFILE
 exit 1
fi

echo "CFLAGS = $CFLAGS" > $OUTFILE
echo "CXXFLAGS = $CXXFLAGS" >> $OUTFILE
echo "CPPFLAGS = $CPPFLAGS" >> $OUTFILE
echo "LDFLAGS = $LDFLAGS" >> $OUTFILE
[ -n "$ASFLAGS" ] && echo "ASFLAGS = $ASFLAGS" >> $OUTFILE

for tool in CC CXX AS LD AR ; do
 tool_cmd=${!tool} 
 if [ -n "$tool_cmd" ]; then
  command -v $tool_cmd >/dev/null
  if [ $? -ne 0 ]; then
   echo "Program '$tool_cmd' is not found" >&2
   rm -f ./$OUTFILE
   exit 1
  fi
  echo "$tool = $tool_cmd" >> $OUTFILE
 fi
done
unset tool

[ -n "$PREBUILD_CMD" ] &&  PREBUILD="__prebuild"
echo -e "\\nall: $CONFIG __cleanlog $PREBUILD $TARGETS" >> $OUTFILE
[ -n "$PREBUILD_CMD" ] && echo -e "\\n$PREBUILD: \\n\\t$PREBUILD_CMD\\n" >> $OUTFILE

echo -e "\\n__cleanlog:\\n\\t@-rm -f $CONFIG/$BUILDLOG\\n" >> $OUTFILE

# custom rules

for ((i=1;;i++)); do
 custom_target="CUSTOM_TARGET$i"
 custom_source="CUSTOM_SOURCE$i"
 custom_target=${!custom_target}
 custom_source=${!custom_source}
 if [ -z "$custom_target" -o -z "$custom_source" ]; then
  break
 fi
 custom_rule="CUSTOM_RULE$i"
 custom_rule=${!custom_rule}
 echo $custom_source
 echo -e "$custom_target: $custom_source" >> $OUTFILE
 if [ -z "$custom_rule" ]; then
  echo "" >> $OUTFILE
 else
  echo -e "\\t$custom_rule\\n" >> $OUTFILE
 fi
done

all_objs=
install_list=

for target in $TARGETS; do

target_output=elf
echo "$target" | grep "\.a$" >/dev/null
[ $? -eq 0 ] && target_output='static-lib'

unset objs srcadd moc_rm target_cppflags target_ldflags

if [ -z "$multiple_targets" ]; then
# sources=`echo -n $SOURCES`
 sources="$SOURCES"
 uic4_sources="$UIC4_SOURCES"
 moc_sources="$MOC_SOURCES"
 sources_nocheck="$SOURCES_NOCHECK"
 install_dir="$INSTALL_DIR"
else
 echo "======== $target ========"
 sane_target="${target/./_}"
 name="${sane_target}_SOURCES"
 sources=${!name}
 if [ -z "$sources" ]; then
  echo "$name not defined" >&2
  rm -f ./$OUTFILE
  exit 2
 fi

 # target's install dir
 name="${sane_target}_INSTALL_DIR"
 install_dir="${!name}"

 # target's LDFLAGS
 name="${sane_target}_LDFLAGS"
 target_ldflags="${!name}"
 [ -n "$target_ldflags" ] && target_ldflags=" $target_ldflags"

 # target's CPPFLAGS
 name="${sane_target}_CPPFLAGS"
 target_cppflags="${!name}"
 [ -n "$target_cppflags" ] && target_cppflags=" $target_cppflags"

 unset name
 uic4_sources="${target}_UIC4_SOURCES"
 uic4_sources=${!uic4_sources}
 moc_sources="${target}_MOC_SOURCES"
 moc_sources=${!moc_sources}
 sources_nocheck="${target}_SOURCES_NOCHECK"
 sources_nocheck=${!sources_nocheck}
fi

target_type=U

for src in $uic4_sources ; do
 output=${src/.ui/.h}
 if [ "$output" != "$src" -a "$src" -nt "ui/ui_$output" ]; then
  mkdir -p ui
  output="ui/ui_$output"
  $QT_UIC $src -o $output
  check_target="$output"
  check_rule
  [ -n "$create_rule" ] && echo -e "$output: $src\\n\\t$QT_UIC \$< -o \$@\\n" >> $OUTFILE
  echo $src
 fi
done

target_type=Q

for src in $sources ; do
 src_base=$(basename $src) 
 if check_src_ext "$src_base" ui ; then
  # Qt3 ui file
  check_target="$src_noext"
  check_rule
  if [ -n "$create_rule" ]; then
   echo -e "$src_noext.h: $src\\n\\t$QT_UIC \$< -o \$@\\n" >> $OUTFILE
   echo -e "$src_noext.cpp: $src_noext.h\\n\\t$QT_UIC -impl \$< \$*.ui -o \$@\\n" >> $OUTFILE
  fi
  echo "$src_base"
  srcadd="$srcadd $src_noext.cpp"
  moc_sources="$moc_sources $src_noext.h"
  unset src_noext
 fi
done

target_type=M

for src in $moc_sources ; do
 src_base=$(basename $src)
 if check_src_ext "$src_base" h ; then
  prefix="moc"
  tool="$QT_MOC"
  ext="h"
 elif check_src_ext "$src_base" qrc ; then
  prefix="qrc"
  tool="$QT_RCC"
  ext="qrc"
 else
  echo "Don't know how to process $src_base" >&2
  rm -f ./$OUTFILE
  exit 1
 fi 
 dir=`dirname $src`
 if [ "$dir" != "." ]; then
  dir="$dir/"
 else
  dir=""
 fi
 cpp_file="${dir}${prefix}_$src_noext.cpp"
 check_target="$cpp_file"
 check_rule
 if [ -n "$create_rule" ]; then
  echo -e "$cpp_file: $dir$src_base" >> $OUTFILE
  if [ "$dir" != "" ]; then
   echo -e -n "\\tcd $dir; " >> $OUTFILE
   echo -e "$tool $src_noext.$ext >${prefix}_$src_noext.cpp\\n" >> $OUTFILE
  else
   echo -e "\\t$tool \$< >\$@\\n" >> $OUTFILE
  fi
  moc_rm="$moc_rm $cpp_file"
 fi
 srcadd="$srcadd $cpp_file"
done

for src in $TEMP_FILES ; do
 if [ ! -f "$src" ]; then
  touch $src
  rm_files="$rm_files $src" 
 fi
done

use_sources="$sources $srcadd"
use_deps="yes"
parse_sources

use_sources="$sources_nocheck"
unset use_deps
parse_sources

if [ "$target_output" = "static-lib" ]; then
 cmd="\$(AR) cr \$@ $objs"
 tool='AR      '
 flags=''
 install_perm=0644
else
 flags="$CONFIG/.LDFLAGS"
 if [ -n "$multiple_targets" ]; then
  for __flags in $all_flags ; do
   name="${sane_target}_${__flags}"
   [ -n "${!name}" ] && flags="$flags $CONFIG/.$name"
  done
 fi
 cmd="\$(LD) $objs \$(LDFLAGS)${target_ldflags} -o \$@"
 tool='LINK    '
 install_perm=0755
fi

cmd="@echo \"  $tool$target\"\n\t@echo '$cmd' >> $CONFIG/$BUILDLOG\n\t@$cmd"
final_rule="$target: $CONFIG $objs $flags"
[ -n "$PRELINK_CMD" ] && final_rule="$final_rule\n\t$PRELINK_CMD"
echo -e "$final_rule\n\t$cmd\n" >> $OUTFILE

if [ -n "$install_dir" ]; then
 tool='INSTALL '
 if [ -n "$multiple_targets" ]; then
  install_list="$install_list install_$target" 
  final_rule="install_$target: $target"
 else
  final_rule="install: $target"
 fi
 cmd="install -d \$(DESTDIR)$install_dir; install -m$install_perm $target \$(DESTDIR)$install_dir"
 cmd="@echo \"  $tool$target\"\n\t@echo '$cmd' >> $CONFIG/$BUILDLOG\n\t@$cmd"
 echo -e "$final_rule\n\t$cmd\n" >> $OUTFILE
fi

all_objs="$all_objs $objs"

done

# clean rule
echo -e "clean:\n\t@rm -f $all_objs $CONFIG/.*FLAGS $CONFIG/$BUILDLOG\n\t@rm -f $TARGETS" >> $OUTFILE
echo -e "\t@if [ -d \"$CONFIG\" ]; then rmdir --ignore-fail-on-non-empty $CONFIG ; else true; fi" >> $OUTFILE
if [ -n "$moc_rm" ]; then
 echo -e "\trm -f $moc_rm" >> $OUTFILE
fi
echo -e -n "\\n" >> $OUTFILE

# Qt: lupdate rules
if [ -n "$moc_rm" ]; then
 for qt_msg in ./*.ts ; do
  qt_qm=${qt_msg/ts/qm}
  # old way to remove newlines: $(echo $SOURCES)
  # the new way: ${SOURCES//$'\n'/ }  
  echo -e "lupdate:\\n\\t$QT_LUPDATE ${SOURCES//$'\n'/ } -ts $qt_msg\\n" >> $OUTFILE
  echo -e "$qt_qm: $qt_msg\\n\\t$QT_LRELEASE $qt_msg -qm $qt_qm\\n" >>$OUTFILE
 done
fi

[ -n "$install_list" ] && echo -e "install: $install_list\\n\\n" >> $OUTFILE

# output files directory
echo -e "$CONFIG:\\n\\tmkdir -p $CONFIG\\n" >> $OUTFILE

# .FLAGS rules

silent='.SILENT:'
for flags in $all_flags ; do
 print_flags_rules $flags
done
unset flags
echo -e "$silent\\n" >> $OUTFILE

[ -n "$rm_files" ] && rm $rm_files

echo "Built $OUTFILE for configuration '$CONFIG'"
