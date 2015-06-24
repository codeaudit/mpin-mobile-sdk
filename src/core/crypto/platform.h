#ifndef PLATFORM_H
#define PLATFORM_H

#if _WIN64 /* Windows 64-bit build */
#define WORD_LENGTH 64
#define OS "Windows"
#elif _WIN32 /* Windows 32-bit build */
#define WORD_LENGTH 32
#define OS "Windows"
#elif __linux && __x86_64 /* Linux 64-bit build*/
#define WORD_LENGTH 64
#define OS "Linux"
#elif __linux /* Linux 32-bit build */
#define WORD_LENGTH 32
#define OS "Linux"
#else /* 32-bit C-Only build - should work on any little Endian processor */
#define WORD_LENGTH 32
#define OS "Universal"
#endif

#endif /* PLATFORM_H */
