/*
Copyright (c) 2012-2015, Certivox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For full details regarding our CertiVox terms of service please refer to
the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
*/

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
#undef unsign32
#define unsign32 uint32_t
#define __int32 int32_t
#define __int64 int64_t
#elif __APPLE__
#define WORD_LENGTH 32
#define OS "Apple"
typedef int32_t __int32;
#undef unsign32
typedef uint32_t unsign32;
#else /* 32-bit C-Only build - should work on any little Endian processor */
#define WORD_LENGTH 32
#define OS "Universal"
#endif

#endif /* PLATFORM_H */
