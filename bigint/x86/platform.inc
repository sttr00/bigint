%ifidn __OUTPUT_FORMAT__,win32
 %macro proc 1
  global _%1
  _%1:
 %endmacro
%else
 %macro proc 1
  global %1
  %1:
 %endmacro
%endif
