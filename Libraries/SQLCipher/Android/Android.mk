LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_MODULE := sqlcipher
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsqlcipher.a
include $(PREBUILT_STATIC_LIBRARY)
