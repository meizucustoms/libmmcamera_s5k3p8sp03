S5K2L7_SENSOR_LIBS_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PATH             := $(S5K2L7_SENSOR_LIBS_PATH)
LOCAL_C_INCLUDES       := $(LOCAL_PATH)/../includes/
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/../includes/0309

LOCAL_C_INCLUDES       += s5k2l7_lib.h
LOCAL_SRC_FILES        := s5k2l7_lib.c
LOCAL_MODULE           := libmmcamera_s5k2l7
LOCAL_SHARED_LIBRARIES := libcutils

include $(BUILD_SHARED_LIBRARY)