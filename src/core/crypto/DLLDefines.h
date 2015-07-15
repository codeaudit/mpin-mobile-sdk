/* Use with Visual Studio Compiler for building Shared libraries */
#ifndef _DLLDEFINES_H_
#define _DLLDEFINES_H_

/* Cmake will define sok_EXPORTS and mpin_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define sok_EXPORTS and mpin_EXPORTS when
building a DLL on windows. */
/* #define sok_EXPORTS */
/* #define mpin_EXPORTS */


#if defined (_MSC_VER) 

 #define DLL_EXPORT extern
/* This code does not work with cl */
/*  #if defined(sok_EXPORTS) || defined(mpin_EXPORTS) */
/*    #define  DLL_EXPORT __declspec(dllexport) */
/*  #else */
/*    #define  DLL_EXPORT __declspec(dllimport) */
/*  #endif /\* sok_EXPORTS || mpin_EXPORTS *\/ */

#else /* defined (_WIN32) */

 #define DLL_EXPORT extern

#endif

#endif /* _DLLDEFINES_H_ */
