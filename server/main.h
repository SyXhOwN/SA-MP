
#ifndef SAMPSRV_MAIN_H
#define SAMPSRV_MAIN_H

// ------------
// VERSION INFO
// ------------

#define SAMP_VERSION "0.3.7-R2"

// ------------
// OS SPECIFICS
// ------------

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
	#include <time.h>
#else
#endif

// --------
// INCLUDES
// --------

// Regular crap
#include <stdio.h>
#include <stdlib.h>

// Std
#include <map>
#include <string>

#include "console.h"

void logprintf(char* format, ...);

#ifdef LINUX
char* strlwr(char* str);
#endif

#endif // SAMPSRV_MAIN_H
