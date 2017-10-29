#include "utils.h"
#include "clock.h"
#include <stdio.h>

#define NUM_COUNT 50

static const uint8_t prime4k[] =
{
 0x92, 0x39, 0x33, 0x12, 0xD9, 0xBB, 0x9F, 0x3C, 0x69, 0xBF, 0x59, 0x82, 0xC7, 0x72, 0xEB, 0xFD,
 0x2B, 0xC5, 0x50, 0xA3, 0xA6, 0x77, 0x2F, 0x94, 0x0C, 0x10, 0xC9, 0xEA, 0x57, 0x66, 0x5F, 0x28,
 0xE4, 0x22, 0x74, 0x6E, 0x97, 0xE5, 0x94, 0x3F, 0x15, 0xC1, 0x58, 0x11, 0x12, 0xF2, 0x6D, 0xBF,
 0x57, 0x62, 0xD6, 0xCE, 0x21, 0xC4, 0x44, 0xD2, 0x47, 0x16, 0x33, 0x0F, 0x6E, 0x67, 0x57, 0xDF,
 0xE8, 0x4D, 0x04, 0xD2, 0xCB, 0x01, 0x5C, 0xAC, 0x9C, 0x30, 0x94, 0x1D, 0x8F, 0x88, 0xD9, 0x2E,
 0xD0, 0xC9, 0xEC, 0x73, 0x9B, 0x78, 0xE5, 0x5F, 0xFA, 0xCE, 0x0F, 0xA3, 0x79, 0x48, 0xFA, 0xE2,
 0xC7, 0x79, 0x34, 0xF6, 0x9D, 0xF6, 0x17, 0x8D, 0xF9, 0x00, 0xF1, 0xD0, 0xC2, 0x25, 0x11, 0xE8,
 0x26, 0x3E, 0xF5, 0x6E, 0xC2, 0x98, 0x17, 0x81, 0x20, 0xB2, 0x0C, 0xEC, 0xE2, 0x41, 0x81, 0xD4,
 0x33, 0xA5, 0x2C, 0xE1, 0xBC, 0xFC, 0x14, 0xAF, 0x8B, 0xB4, 0xC3, 0x33, 0x55, 0xF0, 0x8C, 0x56,
 0x02, 0x0A, 0xBD, 0x5D, 0x18, 0xB4, 0xD3, 0xBC, 0x06, 0x70, 0xBA, 0x90, 0xC6, 0x06, 0x79, 0xA4,
 0x75, 0xA2, 0xE7, 0xAE, 0x6A, 0xBA, 0x95, 0xD8, 0x92, 0xCF, 0x9C, 0x2E, 0x20, 0x84, 0x7A, 0x78,
 0xDF, 0x48, 0xD3, 0xC9, 0x9C, 0x60, 0xE9, 0x10, 0xCE, 0x4E, 0xF1, 0x4B, 0xAE, 0xCB, 0x75, 0x31,
 0xD4, 0x1F, 0x35, 0x39, 0x17, 0x81, 0x6C, 0x02, 0xE5, 0x97, 0x26, 0x1E, 0x52, 0x92, 0x4F, 0x27,
 0x41, 0x7E, 0xB4, 0x15, 0xE4, 0x9B, 0x44, 0x87, 0xB2, 0x56, 0x0A, 0x2A, 0x47, 0x93, 0xB5, 0xAE,
 0x5E, 0x86, 0xA0, 0x1B, 0xC1, 0x43, 0x73, 0x71, 0x4F, 0xC2, 0x43, 0x9F, 0x77, 0xEC, 0xDB, 0x86,
 0x99, 0x39, 0x00, 0x94, 0xF7, 0xC3, 0x32, 0xBB, 0x77, 0x48, 0x9C, 0xCE, 0xF6, 0xB9, 0x5F, 0x8D,
 0x8D, 0xC4, 0xB9, 0x1E, 0xDB, 0x60, 0x01, 0xE9, 0x23, 0x45, 0x2A, 0xB1, 0xC5, 0x59, 0x8D, 0x66,
 0x0F, 0x20, 0xF9, 0x43, 0x5A, 0xD7, 0x57, 0xA5, 0xBC, 0xF6, 0x87, 0xC8, 0xCB, 0xC8, 0xEC, 0xFF,
 0xBA, 0x10, 0x07, 0xAC, 0xB0, 0xAB, 0x30, 0x18, 0xEB, 0x70, 0x50, 0x0F, 0x81, 0xF5, 0xE1, 0xC5,
 0x01, 0x6E, 0x73, 0xC6, 0xB0, 0x84, 0xC5, 0xE3, 0xBE, 0xFF, 0xAA, 0x57, 0x03, 0x8B, 0xD9, 0xD9,
 0x72, 0xD9, 0x6F, 0x46, 0x46, 0xB3, 0xB7, 0x54, 0x23, 0xBA, 0x13, 0x73, 0xA5, 0x91, 0x70, 0xCD,
 0xB4, 0x61, 0x7F, 0x5C, 0xDE, 0x51, 0x2E, 0x07, 0x29, 0xB2, 0x0C, 0x51, 0x6A, 0x58, 0x14, 0x12,
 0x29, 0xFE, 0x30, 0x41, 0x18, 0x61, 0x16, 0xD4, 0xF3, 0xBE, 0x1E, 0x98, 0xDA, 0xF6, 0xB8, 0x8E,
 0xE8, 0x70, 0xDE, 0x09, 0x3F, 0x4D, 0x11, 0x0D, 0x55, 0x41, 0xC3, 0xA4, 0x16, 0x9C, 0x57, 0xEB,
 0xB6, 0xFC, 0xDE, 0x2A, 0x0B, 0x2E, 0x9A, 0xFD, 0xF1, 0xB0, 0xBD, 0x87, 0x4D, 0xC2, 0x68, 0xB7,
 0x0C, 0xEF, 0xA6, 0x2F, 0xAB, 0x46, 0x65, 0x82, 0x52, 0x38, 0x0D, 0xB1, 0xF6, 0x1B, 0xBB, 0xE3,
 0x22, 0x02, 0x48, 0x98, 0x37, 0xA7, 0x72, 0x76, 0x31, 0xC9, 0x58, 0x24, 0x46, 0x07, 0x1E, 0xE7,
 0xE8, 0xB3, 0xD0, 0x7D, 0xBD, 0x98, 0x9E, 0x0D, 0x21, 0xA3, 0xAA, 0xE4, 0x07, 0x0C, 0x2D, 0xD1,
 0xA8, 0xCA, 0xDC, 0x53, 0x05, 0x89, 0x0D, 0xE3, 0x60, 0xCF, 0x40, 0x49, 0x98, 0xA6, 0xFA, 0xD5,
 0x9A, 0x81, 0x1D, 0x57, 0x43, 0x98, 0x78, 0x8B, 0x5F, 0xFC, 0x20, 0x5E, 0x9A, 0x26, 0x1F, 0x93,
 0x92, 0x58, 0x5F, 0x7A, 0xFA, 0x8E, 0x6C, 0x51, 0x73, 0x71, 0x60, 0x8C, 0x5C, 0x89, 0xE7, 0x21,
 0x88, 0x32, 0x19, 0x25, 0xFD, 0xEF, 0xA5, 0x65, 0xF1, 0x58, 0x8B, 0xE6, 0xAA, 0xC6, 0xF5, 0x33
};

static const uint8_t prime8k[] =
{
 0xAD, 0x8D, 0xA9, 0x0A, 0x43, 0xD7, 0x66, 0x65, 0x23, 0x16, 0x69, 0x53, 0x04, 0xD1, 0x72, 0x8C,
 0x06, 0x1A, 0xBF, 0xE0, 0xA9, 0x52, 0xDD, 0x5A, 0x33, 0x4E, 0x80, 0xFD, 0xE6, 0xD9, 0x00, 0x77,
 0x9A, 0x5B, 0x14, 0xC0, 0x21, 0x67, 0x4E, 0x61, 0x88, 0x71, 0xD4, 0x56, 0x5B, 0x54, 0xF2, 0x95,
 0x62, 0xDB, 0x36, 0xC4, 0x65, 0xCB, 0xC1, 0xA8, 0xCF, 0x53, 0x7D, 0xEA, 0xCC, 0x49, 0x72, 0x43,
 0x60, 0xAA, 0xAC, 0x12, 0x3E, 0xEC, 0xC3, 0x4A, 0xCD, 0xB5, 0x43, 0x52, 0xE8, 0x90, 0xB0, 0x4F,
 0x6D, 0x11, 0x7A, 0xE5, 0xE9, 0x4A, 0x03, 0xBA, 0xF3, 0x2D, 0x36, 0xA4, 0x4D, 0xA0, 0x72, 0x07,
 0x8D, 0x8F, 0x13, 0x4D, 0xE8, 0x2D, 0xA5, 0xBB, 0xC4, 0xAC, 0x69, 0x65, 0xCD, 0xB6, 0x90, 0x28,
 0xE3, 0x69, 0xD2, 0xE4, 0xA4, 0xF6, 0xB4, 0xF2, 0x1C, 0xAE, 0x86, 0xAB, 0x1C, 0x4C, 0xFB, 0x6F,
 0x48, 0xB7, 0x3B, 0xC5, 0x43, 0x38, 0xF3, 0xBD, 0x96, 0x62, 0x95, 0x2A, 0x9F, 0x36, 0x99, 0x6C,
 0x60, 0xE5, 0x5B, 0x98, 0x46, 0xFD, 0x39, 0x56, 0x2C, 0x9B, 0x29, 0xD7, 0x40, 0xEF, 0x89, 0x1A,
 0x2B, 0x9F, 0x8D, 0xA2, 0x2A, 0xC7, 0x50, 0xB6, 0xDB, 0x4B, 0xD9, 0x6D, 0x26, 0x32, 0xC1, 0x12,
 0x18, 0xF7, 0x48, 0xD0, 0xD1, 0xBE, 0x18, 0x43, 0x4F, 0x2C, 0xD9, 0x94, 0x8C, 0x96, 0x85, 0xDF,
 0x6D, 0xEA, 0xDD, 0xE9, 0x18, 0x40, 0x87, 0x26, 0xFA, 0xF8, 0x96, 0x02, 0x6E, 0xB9, 0x94, 0x08,
 0xF3, 0x63, 0x53, 0xEE, 0x2A, 0x93, 0x8E, 0xA5, 0x50, 0x3C, 0xCF, 0xDB, 0xB1, 0xED, 0x7F, 0x1B,
 0x06, 0x8A, 0xD7, 0xEC, 0xD3, 0xA5, 0x6A, 0x88, 0x4E, 0xF1, 0x2A, 0x18, 0x16, 0x7A, 0xC5, 0x4E,
 0x08, 0x9E, 0x68, 0xCB, 0x96, 0x54, 0x2B, 0x0C, 0xE3, 0xB1, 0x5A, 0x77, 0x77, 0xDC, 0x3C, 0x7E,
 0x03, 0x43, 0xFF, 0x41, 0x9A, 0x5F, 0xD7, 0x9C, 0x82, 0xE3, 0xE5, 0xE2, 0xD3, 0x31, 0xE1, 0x88,
 0x8D, 0xAF, 0xDC, 0x67, 0xBD, 0xDC, 0xCE, 0x63, 0x2F, 0x40, 0x04, 0xEC, 0x11, 0x4A, 0xD8, 0xC4,
 0xB2, 0x9C, 0xA6, 0xE1, 0xA1, 0x25, 0x69, 0x04, 0x7D, 0xD1, 0x9E, 0xEC, 0xE1, 0xBF, 0xB8, 0x85,
 0x7D, 0x53, 0x8A, 0x10, 0x07, 0xA5, 0xE7, 0x42, 0x0A, 0x97, 0xFC, 0x2E, 0xB6, 0xAD, 0x2D, 0xFF,
 0xD9, 0x69, 0xB6, 0xC9, 0xBB, 0x00, 0x1B, 0x5B, 0xAE, 0x49, 0xB4, 0x35, 0x5E, 0x43, 0x07, 0xBD,
 0xF7, 0x27, 0x3C, 0xCC, 0xD3, 0x7C, 0x7A, 0x6F, 0x81, 0x0F, 0xC5, 0x55, 0x91, 0x0A, 0xBE, 0x1D,
 0xBD, 0xFC, 0x3C, 0x87, 0x59, 0x5E, 0xF4, 0x71, 0xD4, 0xAE, 0x0E, 0x28, 0x7B, 0x66, 0x68, 0x3A,
 0x6A, 0xDE, 0x5B, 0x69, 0x99, 0x66, 0xD6, 0x91, 0xA0, 0xE5, 0x4E, 0x49, 0x9B, 0xCE, 0x88, 0x53,
 0x41, 0xB6, 0xBC, 0x3C, 0x60, 0xF6, 0x56, 0xD3, 0x22, 0x52, 0x8F, 0x8D, 0x90, 0xF0, 0xCA, 0x51,
 0xE3, 0x28, 0x8F, 0x14, 0x88, 0x12, 0xFC, 0x15, 0x24, 0x8B, 0x02, 0x24, 0x5C, 0x04, 0xCE, 0xC0,
 0xE0, 0xFD, 0xFD, 0xEE, 0x0A, 0xF3, 0xAA, 0x0C, 0xE1, 0x26, 0x4C, 0x64, 0x1F, 0xB4, 0xC6, 0x8B,
 0xEA, 0xD1, 0x85, 0x8A, 0xA9, 0x29, 0x5C, 0xFD, 0x34, 0xAE, 0x8D, 0xBB, 0x02, 0x88, 0xCE, 0xBA,
 0xC7, 0xC4, 0xFF, 0x41, 0xA2, 0x3F, 0x27, 0xEC, 0x0D, 0x7D, 0x18, 0x36, 0x54, 0x61, 0x11, 0x8F,
 0xF2, 0xBA, 0x49, 0x2F, 0xFB, 0x66, 0x5C, 0x80, 0xF7, 0x26, 0xC3, 0x28, 0x0F, 0x9C, 0xEF, 0xCD,
 0xF6, 0xE6, 0xF4, 0xC6, 0xF5, 0x2C, 0x7A, 0x4C, 0x20, 0xB6, 0x4F, 0x27, 0xB5, 0xD3, 0xF3, 0xDC,
 0x51, 0xF1, 0xD7, 0x55, 0xA4, 0x60, 0xB8, 0xDD, 0xDE, 0x79, 0x7B, 0xB8, 0x74, 0xC6, 0x2F, 0x8F,
 0x18, 0x6B, 0xC6, 0xA5, 0xEA, 0xCA, 0xEF, 0x89, 0x90, 0xBB, 0x4B, 0x24, 0x06, 0xA3, 0x6C, 0xD7,
 0xFE, 0x00, 0x60, 0x95, 0x42, 0xDC, 0x08, 0xE8, 0x44, 0x00, 0x60, 0xD5, 0x8D, 0x71, 0xAC, 0x08,
 0xF3, 0x72, 0xE8, 0xD2, 0xE6, 0xCC, 0x5D, 0xD7, 0xED, 0xC2, 0x55, 0xAA, 0xB9, 0xCE, 0x3F, 0x4A,
 0x0B, 0x15, 0x49, 0x43, 0x97, 0x77, 0xA9, 0x99, 0x55, 0x41, 0xDC, 0xE6, 0x29, 0x64, 0x49, 0x97,
 0x18, 0xF0, 0x3D, 0x1C, 0x05, 0x0D, 0xDA, 0x2B, 0x54, 0x95, 0x2A, 0xCF, 0x27, 0x1B, 0x63, 0x50,
 0x96, 0x6A, 0x2C, 0xA4, 0xA0, 0x79, 0xA3, 0x30, 0x11, 0xC0, 0x7A, 0xC8, 0x22, 0x56, 0x63, 0xD7,
 0x09, 0x29, 0xE3, 0xF6, 0x0D, 0xD1, 0xC3, 0x9B, 0xB6, 0x64, 0x56, 0x31, 0x58, 0x5B, 0xA1, 0xE5,
 0x69, 0xD6, 0x38, 0xBA, 0xE7, 0xEA, 0xD6, 0xFA, 0x2A, 0x9B, 0x4D, 0x9A, 0x07, 0x63, 0x23, 0xBD,
 0x20, 0x12, 0xCE, 0xA9, 0xD2, 0xBA, 0x06, 0x5B, 0x5D, 0x5B, 0x20, 0x64, 0x8D, 0x25, 0x40, 0x93,
 0x7A, 0x70, 0xFE, 0x0F, 0x79, 0x2D, 0x52, 0xA3, 0xA6, 0xF8, 0xCD, 0x38, 0xB9, 0x10, 0x37, 0xDD,
 0x48, 0x48, 0xE2, 0xBC, 0x43, 0x6A, 0x84, 0x80, 0xDC, 0xC9, 0x13, 0x47, 0x3C, 0xC7, 0x06, 0x8D,
 0xBA, 0xD4, 0x27, 0x10, 0x8E, 0xC9, 0x0D, 0x11, 0xAC, 0x94, 0xCF, 0x18, 0x87, 0xAE, 0x18, 0x95,
 0xF3, 0xD5, 0xCD, 0xB4, 0x49, 0x4B, 0x4D, 0xDF, 0x98, 0x15, 0xF0, 0x51, 0x1A, 0x65, 0xC1, 0x17,
 0x6D, 0xBC, 0xAF, 0x19, 0x66, 0xE2, 0x52, 0xFD, 0xD7, 0x8E, 0x83, 0xFF, 0x41, 0x6F, 0x4E, 0x87,
 0x20, 0xD1, 0x52, 0xEC, 0x65, 0x59, 0xFB, 0x53, 0x05, 0xC6, 0x08, 0x47, 0x0E, 0xCF, 0x92, 0xE9,
 0xC8, 0x00, 0xDD, 0x6A, 0x12, 0xF2, 0x82, 0xB7, 0x63, 0xDE, 0xED, 0x81, 0x01, 0x72, 0x2A, 0xB1,
 0xF1, 0x7B, 0xF0, 0x41, 0xE7, 0x6F, 0x00, 0x6E, 0x3A, 0xEF, 0x27, 0x6F, 0x8D, 0x96, 0xE2, 0xBB,
 0xA7, 0xDD, 0xC6, 0x57, 0xCC, 0x81, 0x28, 0x31, 0xBB, 0x8E, 0xE6, 0xB5, 0xD9, 0x67, 0x0A, 0xAE,
 0x98, 0xEF, 0x40, 0x19, 0x07, 0x94, 0x04, 0x4E, 0xC9, 0xAB, 0x21, 0xCF, 0xBE, 0x56, 0x80, 0xD4,
 0x72, 0x21, 0x8A, 0x95, 0x45, 0x77, 0x1F, 0x98, 0x06, 0xAF, 0x33, 0x75, 0xF6, 0x75, 0xD7, 0xDC,
 0xA6, 0xAE, 0x7A, 0x4E, 0x3C, 0x6E, 0xEF, 0x72, 0x99, 0xFB, 0xF2, 0x83, 0xD1, 0xC0, 0xED, 0x62,
 0xDF, 0x80, 0x09, 0x9C, 0x22, 0x5F, 0x81, 0xF6, 0x5B, 0x7C, 0x51, 0x8C, 0x0A, 0xA1, 0x66, 0x52,
 0x0B, 0x2C, 0xFD, 0xE3, 0x36, 0x1A, 0x9A, 0x91, 0x6F, 0x03, 0xE1, 0x9E, 0xF7, 0xC8, 0xDC, 0x58,
 0x32, 0xB1, 0x54, 0x0F, 0x32, 0xEC, 0x95, 0x81, 0x08, 0x11, 0x8D, 0x7D, 0x3F, 0xE1, 0xFA, 0xC5,
 0xEC, 0xEC, 0x5A, 0xB4, 0xD9, 0xEB, 0xA6, 0xA9, 0x0F, 0x9A, 0xEA, 0x30, 0x92, 0x3E, 0x9A, 0x20,
 0xF6, 0x2A, 0xAC, 0x9B, 0x87, 0xA0, 0xA3, 0x7A, 0x60, 0x71, 0xC0, 0x5D, 0xC7, 0x46, 0xE4, 0x69,
 0xB8, 0x0E, 0x64, 0x2D, 0x54, 0x3D, 0x9D, 0x3D, 0x31, 0x8A, 0xE3, 0x37, 0x93, 0xA7, 0x4A, 0x55,
 0xBF, 0xEB, 0x3A, 0x1E, 0x42, 0x2A, 0x24, 0x9C, 0x92, 0xAF, 0x91, 0xAA, 0x18, 0x46, 0x49, 0xAF,
 0xF7, 0xD7, 0xF7, 0x24, 0x4A, 0xEF, 0xEA, 0x0C, 0x1B, 0x74, 0xCD, 0x5F, 0xFC, 0x94, 0x0D, 0xCF,
 0xB3, 0xF7, 0x90, 0xD8, 0x9B, 0xA7, 0x96, 0x71, 0x30, 0xF3, 0x4C, 0x47, 0x32, 0xEF, 0xE9, 0x34,
 0x00, 0x78, 0xF1, 0x87, 0x01, 0x2D, 0x0B, 0x4A, 0x91, 0x68, 0x9C, 0xC2, 0x87, 0x35, 0x0D, 0x26,
 0xFA, 0x40, 0xD9, 0x00, 0xD2, 0x4E, 0x6F, 0x43, 0x06, 0xEE, 0xDC, 0xFC, 0x50, 0xC3, 0x81, 0x0F
};

static const uint8_t prime16k[] =
{
 0xAA, 0x08, 0xBE, 0x2B, 0xED, 0x15, 0x69, 0x8A, 0xA0, 0x5F, 0x29, 0x5E, 0xAB, 0x41, 0x20, 0x69,
 0x2E, 0x0A, 0x64, 0x00, 0x75, 0xE3, 0x32, 0x08, 0xA8, 0xC0, 0xAB, 0x42, 0xDB, 0xBE, 0xC1, 0x31,
 0x12, 0x39, 0xCC, 0x24, 0x43, 0xCA, 0xC1, 0x57, 0x2B, 0xF2, 0xD8, 0xE1, 0x6B, 0xD7, 0x7D, 0x7A,
 0x7D, 0xD0, 0x9C, 0x10, 0xC1, 0x07, 0x54, 0x27, 0x20, 0x13, 0x42, 0x2F, 0xD9, 0x83, 0x4F, 0x1F,
 0xEA, 0xAC, 0x89, 0xB5, 0xD0, 0xE9, 0x41, 0x17, 0xD1, 0x10, 0x2B, 0xB8, 0xF7, 0xCF, 0x8D, 0xAF,
 0x02, 0x29, 0xF2, 0x1F, 0x5A, 0x65, 0xA5, 0x3F, 0x85, 0xFF, 0x2A, 0x77, 0xAB, 0x26, 0xC5, 0xC6,
 0xE2, 0x79, 0xD5, 0xC9, 0xA8, 0x37, 0xDD, 0x7A, 0x51, 0x29, 0x96, 0x81, 0x54, 0xE4, 0xE5, 0x53,
 0x37, 0xF4, 0xC2, 0xCA, 0xBD, 0xB2, 0xE7, 0x8E, 0x0B, 0x2D, 0x75, 0xEB, 0x06, 0xD5, 0xE0, 0x8D,
 0x13, 0xA7, 0x68, 0xC6, 0x95, 0x6B, 0x3E, 0xBD, 0xEB, 0xF8, 0xD8, 0x4D, 0xD7, 0x34, 0xB0, 0xFC,
 0x1D, 0x15, 0xAD, 0x00, 0x1A, 0xAF, 0x0C, 0xF6, 0xF4, 0x28, 0x52, 0x76, 0x9B, 0x52, 0x0C, 0x7E,
 0x3A, 0xC1, 0x81, 0x1B, 0x84, 0xF0, 0xC5, 0x43, 0x52, 0xDF, 0xA5, 0x3E, 0x2A, 0xAA, 0x24, 0x1D,
 0x03, 0x47, 0xDD, 0xBB, 0xBA, 0x88, 0x32, 0x0D, 0x9B, 0x03, 0xEA, 0x8D, 0xAE, 0xCF, 0x9B, 0x48,
 0x96, 0x19, 0x3F, 0x3F, 0xD7, 0x17, 0x02, 0x4C, 0x32, 0x78, 0xB4, 0x1D, 0xB4, 0x80, 0xCF, 0xDD,
 0xB2, 0xF4, 0x74, 0x17, 0x22, 0x73, 0xD2, 0x4A, 0xBC, 0xEB, 0x8D, 0x55, 0xDD, 0x50, 0x52, 0x19,
 0x13, 0x81, 0x0F, 0x71, 0x1C, 0x91, 0x95, 0x0A, 0x18, 0x5D, 0xB5, 0x53, 0xA2, 0x4A, 0x3A, 0x93,
 0x39, 0xD0, 0xFC, 0xC2, 0xBE, 0x04, 0xD6, 0x58, 0xD9, 0x5B, 0x64, 0xF6, 0xEF, 0x54, 0xA8, 0xB6,
 0xBE, 0x58, 0x1B, 0x06, 0x81, 0x15, 0x38, 0xB8, 0x61, 0x65, 0x58, 0x1E, 0xC1, 0x0B, 0x31, 0x67,
 0xF1, 0xBA, 0xDA, 0x29, 0xDD, 0x13, 0x0E, 0xD0, 0x49, 0xF2, 0xE3, 0x5B, 0x9F, 0x85, 0xAB, 0x7D,
 0x5A, 0xFC, 0x63, 0x42, 0x77, 0x80, 0x30, 0xA0, 0x92, 0xF6, 0x4C, 0xCD, 0x6F, 0x9E, 0xB1, 0x22,
 0x85, 0xE2, 0x39, 0x1F, 0x8A, 0x26, 0x7B, 0xD2, 0x56, 0x7D, 0xFC, 0x65, 0xB0, 0x5B, 0xD6, 0x18,
 0x49, 0x21, 0xCE, 0x1A, 0x60, 0x27, 0x93, 0x70, 0x53, 0xB3, 0xFF, 0xDA, 0x2C, 0xFD, 0xC7, 0xE1,
 0x89, 0x70, 0x04, 0x4D, 0x47, 0xFF, 0xBD, 0xC7, 0x40, 0x85, 0xBD, 0x8D, 0x56, 0x79, 0xFC, 0x57,
 0x8A, 0x36, 0xB7, 0x54, 0x96, 0x57, 0xBD, 0x57, 0xDE, 0xA0, 0xAB, 0x4E, 0x06, 0x71, 0xC9, 0x1D,
 0x35, 0xB8, 0xD1, 0x10, 0xA4, 0x80, 0x6A, 0x35, 0x94, 0x73, 0x68, 0x98, 0xC7, 0xB9, 0x16, 0xBB,
 0x4E, 0xD9, 0x37, 0x64, 0x73, 0x58, 0x18, 0x33, 0xEA, 0x29, 0x93, 0x1C, 0x7C, 0x21, 0x4F, 0xA0,
 0xA4, 0xE8, 0xA0, 0x43, 0x24, 0xD0, 0xB5, 0x03, 0xAE, 0x9D, 0x08, 0x05, 0xCA, 0xF5, 0x07, 0xDA,
 0x4E, 0xDF, 0xF6, 0x7A, 0x44, 0x0D, 0x0D, 0x1D, 0x4B, 0xA0, 0x2F, 0x05, 0x2F, 0x18, 0xB2, 0x33,
 0xB9, 0x5B, 0x69, 0x3E, 0x97, 0xC2, 0xEA, 0x25, 0x96, 0x05, 0xF7, 0x9F, 0x73, 0x5F, 0x2A, 0x0D,
 0x42, 0x05, 0x7D, 0xAE, 0x4D, 0x8B, 0x73, 0x9C, 0xB7, 0xEE, 0xF0, 0x1E, 0x54, 0x87, 0x45, 0xA8,
 0xB7, 0x41, 0x0A, 0x52, 0x7B, 0xD5, 0xEE, 0x1D, 0x2E, 0x2F, 0xB7, 0x65, 0x0B, 0x15, 0xA0, 0x02,
 0xCB, 0x6E, 0xCB, 0x39, 0x2A, 0xE9, 0x3F, 0x67, 0x9A, 0x6D, 0xC2, 0x84, 0xE6, 0x47, 0x1F, 0x81,
 0x27, 0xD8, 0x74, 0xCA, 0x46, 0x86, 0x28, 0x60, 0x2C, 0xE1, 0xD5, 0xBA, 0xEE, 0x9A, 0x6B, 0xBA,
 0x48, 0xBF, 0x07, 0xBB, 0x86, 0xF5, 0x4D, 0xDF, 0x59, 0x82, 0x83, 0x78, 0x47, 0x29, 0x60, 0x1C,
 0xF6, 0xA5, 0x68, 0x94, 0x02, 0xC9, 0x4C, 0xE0, 0xC0, 0x5F, 0x15, 0x3D, 0x7F, 0xD5, 0x5B, 0x00,
 0xF2, 0xC9, 0x4C, 0x12, 0x23, 0x25, 0xB2, 0xE1, 0x1C, 0x05, 0xCA, 0x25, 0x38, 0x0F, 0xD0, 0xD7,
 0x5A, 0xBA, 0x11, 0x57, 0xE9, 0xC1, 0xFA, 0xD8, 0x38, 0x02, 0x6C, 0x3F, 0xF5, 0x1A, 0x41, 0x25,
 0x09, 0x41, 0x28, 0x6B, 0x07, 0xA5, 0x82, 0x7F, 0xD0, 0x02, 0x50, 0xD1, 0xF2, 0x55, 0xB3, 0x93,
 0xFE, 0x52, 0x56, 0x61, 0x50, 0xB6, 0xEC, 0xC9, 0x48, 0xDC, 0xAD, 0xE7, 0xB2, 0x62, 0x88, 0x35,
 0x87, 0xF6, 0xB5, 0xC8, 0x79, 0xBE, 0x4B, 0x6A, 0xC1, 0x01, 0x6B, 0xF7, 0xA4, 0x27, 0x8F, 0x45,
 0x16, 0x54, 0xED, 0x01, 0xDD, 0x0A, 0xC3, 0x0D, 0xD8, 0xE9, 0xAB, 0x00, 0xA9, 0x16, 0x28, 0x6F,
 0x4C, 0x62, 0x3E, 0xBF, 0x1B, 0x1D, 0x25, 0x3A, 0x11, 0xC7, 0x3F, 0xFD, 0x34, 0xBC, 0x6A, 0xA0,
 0xB5, 0x89, 0xEC, 0x99, 0x86, 0x2D, 0x34, 0x6B, 0x6C, 0xFE, 0xC6, 0xF3, 0x0F, 0x12, 0xBF, 0x53,
 0x6A, 0x42, 0x78, 0x3F, 0xDE, 0x57, 0x16, 0x0B, 0x5F, 0x1B, 0x78, 0x40, 0x14, 0x67, 0x15, 0xC0,
 0x4C, 0xC8, 0x99, 0x04, 0x67, 0x63, 0xB7, 0x2A, 0x39, 0x38, 0xFC, 0x38, 0x21, 0x2B, 0x1C, 0x46,
 0x45, 0x80, 0x82, 0x92, 0xE6, 0xF4, 0xFE, 0x60, 0xBD, 0xC5, 0x4E, 0xAA, 0x43, 0x47, 0x63, 0xF0,
 0xF3, 0xD4, 0x8F, 0x1C, 0x7F, 0x1E, 0x6F, 0xFC, 0x7D, 0xC1, 0xA3, 0x14, 0x25, 0xC8, 0x7E, 0x3C,
 0x99, 0x49, 0x44, 0x2D, 0x04, 0x4A, 0xDC, 0x62, 0x72, 0xA5, 0xEB, 0xA1, 0xF6, 0xBE, 0x9A, 0x6F,
 0xC6, 0x88, 0x95, 0x27, 0x7D, 0x08, 0x7D, 0xCE, 0x9C, 0x38, 0xAE, 0x6E, 0x50, 0xCE, 0xCB, 0x90,
 0x3A, 0xB5, 0x9C, 0xDE, 0x22, 0x4D, 0x3C, 0xA6, 0xB9, 0x4C, 0xE0, 0x37, 0xD3, 0x82, 0x29, 0xA6,
 0x85, 0x3F, 0xFA, 0xF2, 0x23, 0x45, 0xB3, 0x45, 0xAD, 0x50, 0xF8, 0x47, 0x5F, 0x55, 0xCF, 0x0A,
 0x3B, 0xAC, 0x51, 0xDA, 0x0D, 0xF8, 0x12, 0x37, 0xD1, 0x1A, 0x27, 0x4F, 0x0B, 0xC9, 0x87, 0xDA,
 0xD6, 0x96, 0x38, 0xBF, 0xBA, 0xDC, 0xF4, 0xC3, 0x62, 0xBB, 0x48, 0x20, 0xBF, 0x54, 0x11, 0x71,
 0x15, 0xAF, 0x92, 0xDC, 0x8A, 0x83, 0xDC, 0x79, 0xEF, 0xD9, 0x48, 0x60, 0xB6, 0x2E, 0xDB, 0x9A,
 0x06, 0x7E, 0xBC, 0x78, 0x1A, 0x7A, 0x7C, 0x8E, 0xB6, 0xEF, 0xE9, 0xE7, 0x88, 0xE9, 0x85, 0xE6,
 0x28, 0xE8, 0xC9, 0x86, 0x48, 0x6C, 0x02, 0xBD, 0x26, 0x8B, 0xCF, 0x74, 0xA8, 0xEC, 0xB8, 0x34,
 0x29, 0x00, 0xCA, 0xD5, 0x2A, 0xD4, 0xFE, 0xF5, 0x88, 0x1B, 0x57, 0x9D, 0xFD, 0x39, 0x34, 0x03,
 0x22, 0x8D, 0x00, 0x47, 0x16, 0x0B, 0xCD, 0x13, 0xB1, 0x7D, 0x46, 0xFD, 0xF7, 0x56, 0xAA, 0x96,
 0xC5, 0x91, 0x4A, 0xA2, 0x5F, 0x91, 0x04, 0x7C, 0xED, 0x79, 0xC5, 0x58, 0x43, 0x5E, 0xFF, 0x39,
 0x02, 0x3D, 0xDD, 0x51, 0x1D, 0xFA, 0x0C, 0x47, 0x52, 0xEF, 0xDC, 0x82, 0xD4, 0x46, 0x37, 0xBF,
 0x64, 0x4E, 0x91, 0xA5, 0x18, 0x75, 0xBE, 0x9B, 0xDC, 0xA5, 0xB2, 0x41, 0x1D, 0x6D, 0xB9, 0xE9,
 0x41, 0xD7, 0x66, 0xDD, 0xDE, 0x7A, 0x5A, 0x7B, 0xB7, 0xEC, 0x95, 0xC3, 0xF6, 0x9A, 0xF4, 0x14,
 0x17, 0x35, 0xBA, 0xD1, 0x72, 0x53, 0xD1, 0xC0, 0x16, 0xF0, 0x5F, 0x9C, 0x7A, 0xF0, 0x10, 0x5B,
 0xF0, 0xB1, 0xBA, 0xAB, 0x94, 0x27, 0xEB, 0x0A, 0x6A, 0x32, 0x9A, 0x91, 0xA9, 0x77, 0xF6, 0x82,
 0x4A, 0xB4, 0x17, 0xDC, 0x03, 0xE8, 0xE6, 0x1C, 0xF3, 0x2F, 0xD2, 0x1D, 0x93, 0xB7, 0x66, 0x38,
 0x2F, 0xA2, 0x9B, 0xA7, 0xB1, 0x42, 0xB4, 0x0E, 0x09, 0x09, 0xA9, 0xE3, 0x66, 0xE3, 0x18, 0x90,
 0x2C, 0xB7, 0x59, 0x82, 0xC9, 0x50, 0x6C, 0x6D, 0x92, 0x71, 0xD8, 0xCE, 0xEA, 0x9C, 0xD0, 0x91,
 0x8B, 0x7C, 0x71, 0x65, 0x4E, 0x49, 0x80, 0xCF, 0x42, 0x3E, 0x25, 0x1D, 0xE0, 0x30, 0x6D, 0x12,
 0x64, 0x0F, 0x7A, 0x57, 0xCD, 0x74, 0x3C, 0x45, 0xDE, 0x3F, 0x23, 0xD3, 0x96, 0x60, 0x82, 0x87,
 0xA5, 0xD4, 0x17, 0x97, 0x9D, 0x90, 0x80, 0x49, 0xBE, 0xFB, 0x1F, 0x9A, 0xCB, 0x68, 0x6A, 0x61,
 0x70, 0x4A, 0xA4, 0x52, 0x07, 0xE7, 0x0D, 0xCF, 0xAF, 0x9C, 0xBD, 0xC1, 0xC8, 0x52, 0xF9, 0x5F,
 0x5D, 0x73, 0x67, 0x2D, 0x78, 0xE5, 0xF8, 0xBC, 0x3F, 0x8A, 0x4F, 0x36, 0x44, 0x8F, 0x64, 0x65,
 0x03, 0x26, 0xEA, 0xD4, 0x5F, 0x2E, 0x02, 0xAC, 0x6B, 0xFC, 0x0C, 0xDF, 0xE0, 0x10, 0x54, 0xD2,
 0xB9, 0x3F, 0x72, 0x0A, 0x76, 0x4B, 0x25, 0x4A, 0x0A, 0xE6, 0xCC, 0x33, 0xD3, 0x5D, 0x0A, 0x1D,
 0x9C, 0x17, 0xDA, 0xF6, 0xB8, 0x0A, 0x8D, 0xB9, 0x3D, 0xA2, 0xF6, 0xDC, 0xDF, 0xFF, 0x5E, 0x70,
 0xFF, 0x02, 0xBE, 0x7F, 0x81, 0x6D, 0xF9, 0x79, 0x8E, 0x1D, 0x59, 0xD6, 0x3C, 0xE9, 0x0B, 0xF8,
 0xDA, 0xAB, 0xB4, 0xFB, 0x52, 0x87, 0x78, 0x24, 0xCE, 0xE5, 0xBF, 0x79, 0xB0, 0x73, 0xD0, 0x5D,
 0x19, 0x37, 0xC8, 0x9D, 0xE4, 0x4C, 0x60, 0xBC, 0x7F, 0xA4, 0x80, 0xF3, 0xEA, 0x04, 0xFE, 0x14,
 0xD0, 0xE6, 0x45, 0x5F, 0xF8, 0xCA, 0xC8, 0x76, 0xB5, 0x10, 0x3B, 0x81, 0x37, 0xF5, 0x9C, 0xB3,
 0x4B, 0x87, 0x75, 0x55, 0xE0, 0xB7, 0xDB, 0xB2, 0xF9, 0xFA, 0xA8, 0xD7, 0x4C, 0xBB, 0xEE, 0x2C,
 0xC1, 0x54, 0x76, 0x92, 0xE5, 0x9B, 0x72, 0x2A, 0xB5, 0x0D, 0x06, 0x06, 0x5D, 0x35, 0x51, 0x83,
 0x9C, 0xFE, 0x8F, 0x4F, 0xEC, 0xD5, 0xB1, 0x83, 0x3C, 0xF1, 0x1C, 0x78, 0x71, 0x10, 0x00, 0x55,
 0xF7, 0x1B, 0xD6, 0xF6, 0xB5, 0x3A, 0xA0, 0xD9, 0xC8, 0xD0, 0xB5, 0xD0, 0xF9, 0x9F, 0x47, 0x61,
 0xE3, 0xF6, 0x94, 0x3C, 0x1E, 0x66, 0x06, 0x28, 0x72, 0x7C, 0x4D, 0x9B, 0x13, 0xC1, 0xB9, 0x0D,
 0xAE, 0x29, 0x3A, 0xB3, 0x6B, 0x0D, 0x10, 0xB7, 0xB6, 0x61, 0x5B, 0x30, 0xC3, 0xF2, 0xA6, 0x53,
 0x36, 0xC4, 0x05, 0xF6, 0xF1, 0x14, 0x1E, 0x4B, 0x6E, 0x07, 0x7E, 0x6B, 0x83, 0x63, 0x24, 0x65,
 0x4A, 0x40, 0xB7, 0xD6, 0x00, 0x9B, 0x91, 0x73, 0x4A, 0xB1, 0x14, 0x90, 0x35, 0x75, 0x65, 0x63,
 0x61, 0x11, 0xC0, 0x4A, 0x37, 0x2D, 0x9E, 0x90, 0x1D, 0x67, 0x9E, 0xE2, 0x6C, 0xF0, 0x17, 0x3B,
 0x1C, 0x88, 0xC1, 0x01, 0xB2, 0x21, 0x5D, 0xF3, 0x71, 0xAD, 0x3C, 0xF2, 0x14, 0xCF, 0x06, 0xD4,
 0x71, 0xFB, 0x6E, 0x82, 0x03, 0x18, 0x65, 0x3F, 0xA3, 0xE1, 0x38, 0x80, 0x66, 0xA1, 0x41, 0x86,
 0xDB, 0x22, 0xEC, 0x50, 0x36, 0x06, 0x6A, 0xDA, 0xAF, 0x2E, 0x50, 0xBC, 0x22, 0xF3, 0x85, 0x1D,
 0xF3, 0x14, 0xBF, 0xE9, 0x5E, 0x8E, 0xC0, 0x82, 0x4D, 0x19, 0x1A, 0xB3, 0x97, 0x90, 0x3B, 0x3F,
 0x10, 0x2D, 0x1D, 0xF0, 0x3F, 0xE7, 0xF2, 0xED, 0x88, 0xC4, 0xCC, 0x5A, 0x81, 0x62, 0x52, 0xE2,
 0x96, 0x7C, 0xA1, 0x4C, 0x95, 0xC5, 0xC3, 0x7A, 0x5F, 0x91, 0xCD, 0xB1, 0xF5, 0xF9, 0x83, 0xA8,
 0x37, 0x37, 0xF5, 0xC0, 0x88, 0xE8, 0x18, 0xDD, 0xD4, 0xD6, 0x0C, 0xF9, 0xA1, 0x6C, 0x29, 0x48,
 0x4B, 0xB0, 0xFC, 0x5D, 0x3B, 0x70, 0x30, 0x81, 0x6B, 0x1C, 0x12, 0xD0, 0x95, 0xF3, 0xB8, 0xA5,
 0x2A, 0xEA, 0x2A, 0x4D, 0xE3, 0xB4, 0xE2, 0x5E, 0x64, 0xE3, 0xE3, 0xA5, 0xC8, 0x35, 0x98, 0xF8,
 0x86, 0x76, 0xFA, 0xC0, 0x93, 0xBC, 0x3E, 0xEB, 0x4C, 0xEC, 0xE6, 0x8F, 0x0E, 0xB0, 0x35, 0x9B,
 0xF8, 0x89, 0x5A, 0xC4, 0x04, 0xBD, 0x36, 0xAA, 0x8E, 0xC7, 0x22, 0x61, 0x2A, 0xFA, 0xFF, 0xE7,
 0x9F, 0xE8, 0x94, 0x0E, 0x95, 0xDF, 0x38, 0x91, 0x29, 0xF9, 0xDB, 0x38, 0x08, 0x07, 0x28, 0xD2,
 0xAA, 0x6B, 0x66, 0x3F, 0xB5, 0x13, 0x16, 0x85, 0xB6, 0x2B, 0x35, 0x59, 0x5A, 0x2C, 0x51, 0x6B,
 0x52, 0xE0, 0x30, 0xD1, 0xFC, 0xFD, 0xE0, 0x9D, 0xF3, 0x7C, 0x05, 0xC0, 0xC0, 0x89, 0xAA, 0x2A,
 0xB0, 0xFB, 0xDC, 0x10, 0x9C, 0xE2, 0x88, 0x5A, 0xFE, 0x00, 0x01, 0xDF, 0x8B, 0xFB, 0xA7, 0x33,
 0x1A, 0xAD, 0xBC, 0x63, 0x4E, 0xFE, 0x1A, 0x31, 0x32, 0xAA, 0x5B, 0x43, 0xD8, 0xBB, 0xE4, 0x8F,
 0x7F, 0x39, 0x4C, 0x86, 0x47, 0xF2, 0x1E, 0xBC, 0x78, 0x9D, 0x11, 0x9E, 0xFB, 0x7D, 0x26, 0x78,
 0xC4, 0xA5, 0x08, 0x92, 0x9D, 0xF4, 0x09, 0xD9, 0x75, 0xE9, 0x34, 0x98, 0x62, 0x4A, 0xD9, 0x70,
 0x60, 0x75, 0xD1, 0x17, 0xE9, 0xAB, 0x2A, 0x9F, 0xF2, 0x39, 0x3E, 0x0D, 0xFA, 0xBA, 0x0F, 0xCE,
 0x7F, 0x9F, 0x75, 0x8D, 0xD3, 0x03, 0x8C, 0xB0, 0xC9, 0xF1, 0x68, 0x17, 0x38, 0xFC, 0x23, 0x06,
 0x8C, 0x03, 0x0B, 0xA5, 0x3A, 0x57, 0x76, 0xFD, 0x5C, 0x63, 0x2C, 0x99, 0x4F, 0x6F, 0x81, 0x88,
 0x6D, 0xE6, 0xBC, 0xB5, 0x89, 0x03, 0x53, 0x54, 0x8C, 0x92, 0xA2, 0x42, 0xF9, 0xE1, 0xE2, 0x41,
 0x60, 0xF1, 0xC9, 0x89, 0x4B, 0x9B, 0x46, 0x34, 0x47, 0x5C, 0x3F, 0x94, 0x22, 0x4A, 0x97, 0x5E,
 0xEA, 0xAB, 0xA8, 0x1B, 0x35, 0xE1, 0x4D, 0x25, 0xCB, 0x66, 0x5D, 0xBB, 0x47, 0x0A, 0xA7, 0xFC,
 0x4C, 0xB8, 0x14, 0x18, 0x0C, 0x5B, 0xBC, 0x73, 0x48, 0x57, 0xCA, 0x56, 0x36, 0x4D, 0xBD, 0x26,
 0x9B, 0x46, 0x81, 0x14, 0x4E, 0xB8, 0x48, 0xC1, 0xEC, 0xAB, 0xD9, 0x4B, 0xEF, 0x6F, 0xCA, 0x3F,
 0x71, 0xF9, 0x4A, 0x86, 0xFA, 0xC2, 0x18, 0x2D, 0xD5, 0x54, 0x5B, 0xA8, 0x34, 0x45, 0xA0, 0x8A,
 0x82, 0xD7, 0xB9, 0xE6, 0x1C, 0x8A, 0xEB, 0xD5, 0x69, 0xE4, 0xA9, 0x16, 0x16, 0x8E, 0x42, 0x1F,
 0xB8, 0x19, 0x28, 0x39, 0xC2, 0xFB, 0x61, 0xF6, 0xF5, 0x82, 0xC6, 0x3D, 0x45, 0x88, 0x41, 0x80,
 0x23, 0xB8, 0x6D, 0x86, 0x22, 0xFA, 0x83, 0x88, 0xB0, 0x70, 0x63, 0x65, 0x15, 0xB0, 0xAA, 0xF9,
 0xB5, 0xB6, 0xB4, 0x46, 0xFD, 0x7C, 0x22, 0xCA, 0xB4, 0x98, 0xFE, 0xD8, 0x90, 0x09, 0xCC, 0x7C,
 0xED, 0xDC, 0xF6, 0xA0, 0x6E, 0x08, 0x7E, 0x54, 0x92, 0x99, 0xA4, 0x09, 0x15, 0xA2, 0xE1, 0x26,
 0x5F, 0x23, 0xAF, 0xB4, 0xB4, 0x6E, 0xB1, 0xA1, 0x87, 0x92, 0x03, 0x96, 0xB1, 0xB7, 0xF2, 0xF2,
 0xB0, 0xDB, 0xD2, 0x82, 0x27, 0x40, 0x40, 0x7A, 0xA7, 0x7B, 0x10, 0xFF, 0x60, 0x29, 0xE2, 0x30,
 0x37, 0x00, 0xF0, 0xCA, 0x73, 0x64, 0x8C, 0xD0, 0x40, 0xD6, 0x0C, 0x55, 0xBF, 0x9F, 0xAA, 0xBB,
 0xE3, 0x69, 0x56, 0xBE, 0xCA, 0x99, 0x25, 0x95, 0x25, 0xB7, 0x5C, 0x36, 0x82, 0x09, 0x1F, 0x2F,
 0xFB, 0xE9, 0x2C, 0x51, 0x4F, 0x0D, 0x7A, 0xFA, 0xB0, 0xF8, 0xF9, 0xEE, 0x09, 0x73, 0x78, 0xFB,
 0x0D, 0x4A, 0x0B, 0xC8, 0x42, 0xFE, 0xA6, 0x11, 0x62, 0xD1, 0x18, 0x66, 0x7C, 0x33, 0x0F, 0x5C,
 0xA2, 0x63, 0xA0, 0xBD, 0x0F, 0xC5, 0xDA, 0xCB, 0x09, 0x47, 0x15, 0x67, 0x13, 0xF3, 0x27, 0x8E,
 0x0E, 0xBF, 0x0D, 0xBE, 0xAA, 0x9B, 0xA1, 0x6D, 0x14, 0x50, 0xBB, 0xBC, 0xF6, 0x29, 0xD1, 0xE7,
 0x90, 0xD0, 0xA3, 0x85, 0x90, 0x89, 0x2B, 0x64, 0xF0, 0xC3, 0x65, 0xF4, 0xBA, 0x10, 0x8A, 0x33
};

static void benchmark_pow1(const uint8_t *data, int size)
{
 bigint_t p = bigint_create_bytes_be(data, size);
 bigint_t a[NUM_COUNT];
 bigint_t b[NUM_COUNT];
 bigint_t res[NUM_COUNT];
 int i;
 int64_t tstart, tsum = 0;
 for (i=0; i<NUM_COUNT; i++)
 {
  a[i] = create_random(i+1, size);
  b[i] = create_random(0x80000000+i, size);
  res[i] = bigint_create((2*size + BIGINT_WORD_SIZE - 1)>>BIGINT_WORD_SHIFT);
 }
 tstart = get_clock();
 for (i=0; i<NUM_COUNT; i++)
 {
  #if 0
  printf("Iteration %d\n", i+1);
  #endif
  bigint_mpow(res[i], a[i], b[i], p);
 }
 tsum += get_clock() - tstart;

 for (i=0; i<NUM_COUNT; i++)
 {
  bigint_destroy(a[i]);
  bigint_destroy(b[i]);
  bigint_destroy(res[i]);
 }
 bigint_destroy(p);
 printf("pow time (%d bits): %d avg\n", size<<3, (int) (tsum/NUM_COUNT));
}

void benchmark_pow()
{
 benchmark_pow1(prime4k, sizeof(prime4k));
 benchmark_pow1(prime8k, sizeof(prime8k));
 benchmark_pow1(prime16k, sizeof(prime16k));
}