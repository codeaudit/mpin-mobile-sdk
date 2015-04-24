#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/620013993/CvString.o \
	${OBJECTDIR}/_ext/620013993/CvXcode.o \
	${OBJECTDIR}/_ext/580510450/CvHttpRequest.o \
	${OBJECTDIR}/_ext/580510450/CvLogger.o \
	${OBJECTDIR}/_ext/580510450/CvMutex.o \
	${OBJECTDIR}/_ext/580510450/CvSemaphore.o \
	${OBJECTDIR}/_ext/580510450/CvThread.o \
	${OBJECTDIR}/_ext/1341795113/aesGcmWrapper.o \
	${OBJECTDIR}/_ext/1341795113/certivox.o \
	${OBJECTDIR}/_ext/1341795113/eccsiWrapper.o \
	${OBJECTDIR}/_ext/1341795113/eccsi_c.o \
	${OBJECTDIR}/_ext/1341795113/mpinWrapper.o \
	${OBJECTDIR}/_ext/1341795113/mpin_c.o \
	${OBJECTDIR}/_ext/1341795113/mraes.o \
	${OBJECTDIR}/_ext/1341795113/mralloc.o \
	${OBJECTDIR}/_ext/1341795113/mrarth0.o \
	${OBJECTDIR}/_ext/1341795113/mrarth1.o \
	${OBJECTDIR}/_ext/1341795113/mrarth2.o \
	${OBJECTDIR}/_ext/1341795113/mrarth3.o \
	${OBJECTDIR}/_ext/1341795113/mrbits.o \
	${OBJECTDIR}/_ext/1341795113/mrcombaLinux64.o \
	${OBJECTDIR}/_ext/1341795113/mrcore.o \
	${OBJECTDIR}/_ext/1341795113/mrcurve.o \
	${OBJECTDIR}/_ext/1341795113/mrecn2.o \
	${OBJECTDIR}/_ext/1341795113/mrgcd.o \
	${OBJECTDIR}/_ext/1341795113/mrgcm.o \
	${OBJECTDIR}/_ext/1341795113/mrio1.o \
	${OBJECTDIR}/_ext/1341795113/mrjack.o \
	${OBJECTDIR}/_ext/1341795113/mrlucas.o \
	${OBJECTDIR}/_ext/1341795113/mrmonty.o \
	${OBJECTDIR}/_ext/1341795113/mrmuldvLinux64.o \
	${OBJECTDIR}/_ext/1341795113/mrpower.o \
	${OBJECTDIR}/_ext/1341795113/mrprime.o \
	${OBJECTDIR}/_ext/1341795113/mrrand.o \
	${OBJECTDIR}/_ext/1341795113/mrshs.o \
	${OBJECTDIR}/_ext/1341795113/mrshs256.o \
	${OBJECTDIR}/_ext/1341795113/mrsmall.o \
	${OBJECTDIR}/_ext/1341795113/mrsroot.o \
	${OBJECTDIR}/_ext/1341795113/mrstrong.o \
	${OBJECTDIR}/_ext/1341795113/mrxgcd.o \
	${OBJECTDIR}/_ext/1341795113/mrzzn2.o \
	${OBJECTDIR}/_ext/1341795113/mrzzn2b.o \
	${OBJECTDIR}/_ext/1341795113/mrzzn4.o \
	${OBJECTDIR}/_ext/1341795113/octet.o \
	${OBJECTDIR}/_ext/1341795113/sakkeWrapper.o \
	${OBJECTDIR}/_ext/1341795113/sakke_bn_c.o \
	${OBJECTDIR}/_ext/608447161/mpin_crypto_non_tee.o \
	${OBJECTDIR}/_ext/608447161/mpin_sdk.o \
	${OBJECTDIR}/_ext/608447161/utils.o \
	${OBJECTDIR}/HttpRequest.o \
	${OBJECTDIR}/MpinClient.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread -lcurl -lcrypto

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpinclient

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpinclient: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpinclient ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/620013993/CvString.o: ../../../ext/cvshared/cpp/CvString.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/620013993
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/620013993/CvString.o ../../../ext/cvshared/cpp/CvString.cpp

${OBJECTDIR}/_ext/620013993/CvXcode.o: ../../../ext/cvshared/cpp/CvXcode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/620013993
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/620013993/CvXcode.o ../../../ext/cvshared/cpp/CvXcode.cpp

${OBJECTDIR}/_ext/580510450/CvHttpRequest.o: ../../../ext/cvshared/cpp/linux/CvHttpRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/580510450
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/580510450/CvHttpRequest.o ../../../ext/cvshared/cpp/linux/CvHttpRequest.cpp

${OBJECTDIR}/_ext/580510450/CvLogger.o: ../../../ext/cvshared/cpp/linux/CvLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/580510450
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/580510450/CvLogger.o ../../../ext/cvshared/cpp/linux/CvLogger.cpp

${OBJECTDIR}/_ext/580510450/CvMutex.o: ../../../ext/cvshared/cpp/linux/CvMutex.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/580510450
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/580510450/CvMutex.o ../../../ext/cvshared/cpp/linux/CvMutex.cpp

${OBJECTDIR}/_ext/580510450/CvSemaphore.o: ../../../ext/cvshared/cpp/linux/CvSemaphore.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/580510450
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/580510450/CvSemaphore.o ../../../ext/cvshared/cpp/linux/CvSemaphore.cpp

${OBJECTDIR}/_ext/580510450/CvThread.o: ../../../ext/cvshared/cpp/linux/CvThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/580510450
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/580510450/CvThread.o ../../../ext/cvshared/cpp/linux/CvThread.cpp

${OBJECTDIR}/_ext/1341795113/aesGcmWrapper.o: ../../../src/core/crypto/aesGcmWrapper.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/aesGcmWrapper.o ../../../src/core/crypto/aesGcmWrapper.c

${OBJECTDIR}/_ext/1341795113/certivox.o: ../../../src/core/crypto/certivox.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/certivox.o ../../../src/core/crypto/certivox.c

${OBJECTDIR}/_ext/1341795113/eccsiWrapper.o: ../../../src/core/crypto/eccsiWrapper.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/eccsiWrapper.o ../../../src/core/crypto/eccsiWrapper.c

${OBJECTDIR}/_ext/1341795113/eccsi_c.o: ../../../src/core/crypto/eccsi_c.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/eccsi_c.o ../../../src/core/crypto/eccsi_c.c

${OBJECTDIR}/_ext/1341795113/mpinWrapper.o: ../../../src/core/crypto/mpinWrapper.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mpinWrapper.o ../../../src/core/crypto/mpinWrapper.c

${OBJECTDIR}/_ext/1341795113/mpin_c.o: ../../../src/core/crypto/mpin_c.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mpin_c.o ../../../src/core/crypto/mpin_c.c

${OBJECTDIR}/_ext/1341795113/mraes.o: ../../../src/core/crypto/mraes.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mraes.o ../../../src/core/crypto/mraes.c

${OBJECTDIR}/_ext/1341795113/mralloc.o: ../../../src/core/crypto/mralloc.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mralloc.o ../../../src/core/crypto/mralloc.c

${OBJECTDIR}/_ext/1341795113/mrarth0.o: ../../../src/core/crypto/mrarth0.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrarth0.o ../../../src/core/crypto/mrarth0.c

${OBJECTDIR}/_ext/1341795113/mrarth1.o: ../../../src/core/crypto/mrarth1.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrarth1.o ../../../src/core/crypto/mrarth1.c

${OBJECTDIR}/_ext/1341795113/mrarth2.o: ../../../src/core/crypto/mrarth2.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrarth2.o ../../../src/core/crypto/mrarth2.c

${OBJECTDIR}/_ext/1341795113/mrarth3.o: ../../../src/core/crypto/mrarth3.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrarth3.o ../../../src/core/crypto/mrarth3.c

${OBJECTDIR}/_ext/1341795113/mrbits.o: ../../../src/core/crypto/mrbits.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrbits.o ../../../src/core/crypto/mrbits.c

${OBJECTDIR}/_ext/1341795113/mrcombaLinux64.o: ../../../src/core/crypto/mrcombaLinux64.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrcombaLinux64.o ../../../src/core/crypto/mrcombaLinux64.c

${OBJECTDIR}/_ext/1341795113/mrcore.o: ../../../src/core/crypto/mrcore.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrcore.o ../../../src/core/crypto/mrcore.c

${OBJECTDIR}/_ext/1341795113/mrcurve.o: ../../../src/core/crypto/mrcurve.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrcurve.o ../../../src/core/crypto/mrcurve.c

${OBJECTDIR}/_ext/1341795113/mrecn2.o: ../../../src/core/crypto/mrecn2.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrecn2.o ../../../src/core/crypto/mrecn2.c

${OBJECTDIR}/_ext/1341795113/mrgcd.o: ../../../src/core/crypto/mrgcd.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrgcd.o ../../../src/core/crypto/mrgcd.c

${OBJECTDIR}/_ext/1341795113/mrgcm.o: ../../../src/core/crypto/mrgcm.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrgcm.o ../../../src/core/crypto/mrgcm.c

${OBJECTDIR}/_ext/1341795113/mrio1.o: ../../../src/core/crypto/mrio1.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrio1.o ../../../src/core/crypto/mrio1.c

${OBJECTDIR}/_ext/1341795113/mrjack.o: ../../../src/core/crypto/mrjack.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrjack.o ../../../src/core/crypto/mrjack.c

${OBJECTDIR}/_ext/1341795113/mrlucas.o: ../../../src/core/crypto/mrlucas.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrlucas.o ../../../src/core/crypto/mrlucas.c

${OBJECTDIR}/_ext/1341795113/mrmonty.o: ../../../src/core/crypto/mrmonty.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrmonty.o ../../../src/core/crypto/mrmonty.c

${OBJECTDIR}/_ext/1341795113/mrmuldvLinux64.o: ../../../src/core/crypto/mrmuldvLinux64.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrmuldvLinux64.o ../../../src/core/crypto/mrmuldvLinux64.c

${OBJECTDIR}/_ext/1341795113/mrpower.o: ../../../src/core/crypto/mrpower.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrpower.o ../../../src/core/crypto/mrpower.c

${OBJECTDIR}/_ext/1341795113/mrprime.o: ../../../src/core/crypto/mrprime.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrprime.o ../../../src/core/crypto/mrprime.c

${OBJECTDIR}/_ext/1341795113/mrrand.o: ../../../src/core/crypto/mrrand.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrrand.o ../../../src/core/crypto/mrrand.c

${OBJECTDIR}/_ext/1341795113/mrshs.o: ../../../src/core/crypto/mrshs.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrshs.o ../../../src/core/crypto/mrshs.c

${OBJECTDIR}/_ext/1341795113/mrshs256.o: ../../../src/core/crypto/mrshs256.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrshs256.o ../../../src/core/crypto/mrshs256.c

${OBJECTDIR}/_ext/1341795113/mrsmall.o: ../../../src/core/crypto/mrsmall.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrsmall.o ../../../src/core/crypto/mrsmall.c

${OBJECTDIR}/_ext/1341795113/mrsroot.o: ../../../src/core/crypto/mrsroot.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrsroot.o ../../../src/core/crypto/mrsroot.c

${OBJECTDIR}/_ext/1341795113/mrstrong.o: ../../../src/core/crypto/mrstrong.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrstrong.o ../../../src/core/crypto/mrstrong.c

${OBJECTDIR}/_ext/1341795113/mrxgcd.o: ../../../src/core/crypto/mrxgcd.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrxgcd.o ../../../src/core/crypto/mrxgcd.c

${OBJECTDIR}/_ext/1341795113/mrzzn2.o: ../../../src/core/crypto/mrzzn2.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrzzn2.o ../../../src/core/crypto/mrzzn2.c

${OBJECTDIR}/_ext/1341795113/mrzzn2b.o: ../../../src/core/crypto/mrzzn2b.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrzzn2b.o ../../../src/core/crypto/mrzzn2b.c

${OBJECTDIR}/_ext/1341795113/mrzzn4.o: ../../../src/core/crypto/mrzzn4.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/mrzzn4.o ../../../src/core/crypto/mrzzn4.c

${OBJECTDIR}/_ext/1341795113/octet.o: ../../../src/core/crypto/octet.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/octet.o ../../../src/core/crypto/octet.c

${OBJECTDIR}/_ext/1341795113/sakkeWrapper.o: ../../../src/core/crypto/sakkeWrapper.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/sakkeWrapper.o ../../../src/core/crypto/sakkeWrapper.c

${OBJECTDIR}/_ext/1341795113/sakke_bn_c.o: ../../../src/core/crypto/sakke_bn_c.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1341795113
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../src/core/crypto -I../../../src/core -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1341795113/sakke_bn_c.o ../../../src/core/crypto/sakke_bn_c.c

${OBJECTDIR}/_ext/608447161/mpin_crypto_non_tee.o: ../../../src/core/mpin_crypto_non_tee.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/608447161
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/608447161/mpin_crypto_non_tee.o ../../../src/core/mpin_crypto_non_tee.cpp

${OBJECTDIR}/_ext/608447161/mpin_sdk.o: ../../../src/core/mpin_sdk.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/608447161
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/608447161/mpin_sdk.o ../../../src/core/mpin_sdk.cpp

${OBJECTDIR}/_ext/608447161/utils.o: ../../../src/core/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/608447161
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/608447161/utils.o ../../../src/core/utils.cpp

${OBJECTDIR}/HttpRequest.o: HttpRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HttpRequest.o HttpRequest.cpp

${OBJECTDIR}/MpinClient.o: MpinClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MpinClient.o MpinClient.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../src/core/crypto -I../../../src/core -I../../../ext/cvshared/cpp/include -I../../../src/core/json -I../../../src/core/utf8 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpinclient

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
