#ifndef MIRDEF_H
#define MIRDEF_H

#if _WIN64 /* Windows 64-bit build */
    #include "mirdef.wc64"
#elif _WIN32 /* Windows 32-bit build */
    #include "mirdef.wc32"
#elif __linux && __x86_64 /* Linux 64-bit build*/
    #include "mirdef.lc64"
#elif __linux /* Linux 32-bit build */
    #include "mirdef.lc32"
#else /* 32-bit C-Only build - should work on any little Endian processor */
    #include "mirdef.gcc"
#endif

#endif /* MIRDEF_H */
