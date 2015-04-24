LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

PATH_TO_CORE	:= ../../../core/crypto

LOCAL_MODULE    :=  AndroidMpinSDK
LOCAL_SRC_FILES :=  ../../../../ext/cvshared/cpp/CvString.cpp \
					../../../../ext/cvshared/cpp/CvXcode.cpp \
					AndroidMpinSDK.cpp \
					HTTPConnector.cpp \
					Storage.cpp \
					Context.cpp \
					JNICommon.cpp \
					$(PATH_TO_CORE)/aesGcmWrapper.c \
					$(PATH_TO_CORE)/certivox.c \
					$(PATH_TO_CORE)/eccsiWrapper.c \
					$(PATH_TO_CORE)/eccsi_c.c \
					$(PATH_TO_CORE)/mpinWrapper.c \
					$(PATH_TO_CORE)/mpin_c.c \
					$(PATH_TO_CORE)/mraes.c \
					$(PATH_TO_CORE)/mralloc.c \
					$(PATH_TO_CORE)/mrarth0.c \
					$(PATH_TO_CORE)/mrarth1.c \
					$(PATH_TO_CORE)/mrarth2.c \
					$(PATH_TO_CORE)/mrarth3.c \
					$(PATH_TO_CORE)/mrbits.c \
					$(PATH_TO_CORE)/mrcore.c \
					$(PATH_TO_CORE)/mrcurve.c \
					$(PATH_TO_CORE)/mrecn2.c \
					$(PATH_TO_CORE)/mrgcd.c \
					$(PATH_TO_CORE)/mrgcm.c \
					$(PATH_TO_CORE)/mrio1.c \
					$(PATH_TO_CORE)/mrjack.c \
					$(PATH_TO_CORE)/mrlucas.c \
					$(PATH_TO_CORE)/mrmonty.c \
					$(PATH_TO_CORE)/mrpower.c \
					$(PATH_TO_CORE)/mrprime.c \
					$(PATH_TO_CORE)/mrrand.c \
					$(PATH_TO_CORE)/mrshs.c \
					$(PATH_TO_CORE)/mrshs256.c \
					$(PATH_TO_CORE)/mrsmall.c \
					$(PATH_TO_CORE)/mrsroot.c \
					$(PATH_TO_CORE)/mrstrong.c \
					$(PATH_TO_CORE)/mrxgcd.c \
					$(PATH_TO_CORE)/mrzzn2.c \
					$(PATH_TO_CORE)/mrzzn2b.c \
					$(PATH_TO_CORE)/mrzzn4.c \
					$(PATH_TO_CORE)/octet.c \
					$(PATH_TO_CORE)/sakkeWrapper.c \
					$(PATH_TO_CORE)/sakke_bn_c.c \
					$(PATH_TO_CORE)/mrcombaARM32.c.arm \
					$(PATH_TO_CORE)/mrmuldvARM32.c.arm \
					\
					$(PATH_TO_CORE)/../mpin_sdk.cpp \
					$(PATH_TO_CORE)/../utils.cpp \
					$(PATH_TO_CORE)/../mpin_crypto_non_tee.cpp

LOCAL_C_INCLUDES += jni/$(PATH_TO_CORE)/../ ../../../ext/cvshared/cpp/include
				
LOCAL_CFLAGS := -fexceptions -fomit-frame-pointer
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
