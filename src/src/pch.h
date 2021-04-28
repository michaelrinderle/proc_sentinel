#ifndef PCH_H
#define PCH_H

//////////////////////////////////////////////////
// process watch (procwatch)
// by michael gary rinderle
// winapi process watcher for kill
//////////////////////////////////////////////////

#include "stdint.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "shlwapi.h"
#include "windows.h"
#include "winuser.h"
#include "tlhelp32.h"

#pragma comment(lib, "Shlwapi.lib")

#define WIN32_LEAN_AND_MEAN

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char  uchar;

#endif // PCH_H