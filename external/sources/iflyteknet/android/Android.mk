LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  

#添加预编译的openssl
include $(CLEAR_VARS)
LOCAL_MODULE := libopenssl
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../android/$(TARGET_ARCH_ABI)/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../android/$(TARGET_ARCH_ABI)/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := iflyteknet_shared
LOCAL_MODULE_FILENAME := libiflyteknet
   
LOCAL_SRC_FILES := 	../src/iflytek/net/TcpConnection.cpp \
					../src/iflytek/net/Compression.cpp
					
					

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src/boost/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../android/$(TARGET_ARCH_ABI)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../src/
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../src/boost/

LOCAL_STATIC_LIBRARIES += libopenssl
LOCAL_STATIC_LIBRARIES += libcrypto

LOCAL_LDLIBS := -pthread -lz

LOCAL_CFLAGS := -DASIO_STANDALONE

#导入proto库
#LOCAL_WHOLE_STATIC_LIBRARIES += protobuf_static

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

#$(call import-module,sources/protobuf/src)