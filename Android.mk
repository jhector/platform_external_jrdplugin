LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := main.c
LOCAL_MODULE := libjrd
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
