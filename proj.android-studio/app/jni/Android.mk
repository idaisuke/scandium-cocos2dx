LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

# Project-specific import path BEGIN
$(call import-add-path,$(LOCAL_PATH)/../../../Libraries)
# Project-specific import path END

MY_SRC_FILES := $(shell find $(LOCAL_PATH)/../../../Classes -name *.cpp)
MY_SRC_FILES += $(LOCAL_PATH)/../../../Submodules/scandium/test_scandium.cpp

MY_C_INCLUDES := $(shell find $(LOCAL_PATH)/../../../Classes -type d)
MY_C_INCLUDES += $(LOCAL_PATH)/../../../Libraries/boost/include
MY_C_INCLUDES += $(LOCAL_PATH)/../../../Submodules/scandium/include

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := main.cpp
LOCAL_SRC_FILES += $(MY_SRC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(MY_C_INCLUDES)

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

# Project-specific lib BEGIN
LOCAL_STATIC_LIBRARIES += sqlcipher
LOCAL_STATIC_LIBRARIES += crypto ssl
# Project-specific lib END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

# Project-specific import BEGIN
$(call import-module,SQLCipher/Android)
$(call import-module,OpenSSL/Android)
# Project-specific import END
