// the configured options and settings for MPin_Crypto
#define MPin_Crypto_VERSION_MAJOR 0
#define MPin_Crypto_VERSION_MINOR 2

#if _WIN64 /* Windows 64-bit build */
    #define BUILD_VERSION "Windows 64-bit build ASM Support"
#elif _WIN32 /* Windows 32-bit build */
    #define BUILD_VERSION "Windows 32-bit build ASM Support"
#elif __linux && __x86_64 /* Linux 64-bit build*/
    #define BUILD_VERSION "Linux 64-bit build ASM Support"
#elif __linux /* Linux 32-bit build */
    #define BUILD_VERSION "Linux 32-bit build No ASM Support"
#else /* 32-bit C-Only build - should work on any little Endian processor */
    #define BUILD_VERSION "C 32-bit build No ASM Support"
#endif

