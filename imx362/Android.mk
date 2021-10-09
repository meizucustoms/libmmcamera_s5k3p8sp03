IMX362_SENSOR_LIBS_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PATH             := $(IMX362_SENSOR_LIBS_PATH)
LOCAL_C_INCLUDES       := $(LOCAL_PATH)/../includes/
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/../includes/0309

LOCAL_C_INCLUDES       += imx362_lib.h
LOCAL_SRC_FILES        := imx362_lib.c
LOCAL_MODULE           := libmmcamera_imx362
LOCAL_SHARED_LIBRARIES := libcutils

include $(BUILD_SHARED_LIBRARY)