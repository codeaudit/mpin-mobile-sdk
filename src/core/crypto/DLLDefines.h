/* Use with Visual Studio Compiler for building Shared libraries */
#ifndef _DLLDEFINES_H_
#define _DLLDEFINES_H_

/* Cmake will define sakke_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define sakke_EXPORTS when
building a DLL on windows. */
#define sakke_EXPORTS
#define aesgcm_EXPORTS
#define eccsi_EXPORTS
#define mpin_EXPORTS


#if defined (_WIN32) 

  #if defined(sakke_EXPORTS) || defined(eccsi_EXPORTS) || defined(aesGcm_EXPORTS) || defined(mpin_EXPORTS)
    #define  DLL_EXPORT __declspec(dllexport)
  #else
    #define  DLL_EXPORT __declspec(dllimport)
  #endif /* sakke_EXPORTS || eccsi_EXPORTS || aesgcm_EXPORTS || mpin_EXPORTS */

#else /* defined (_WIN32) */

 #define DLL_EXPORT

#endif

#endif /* _DLLDEFINES_H_ */
