gt24c64a_s5k4e8_EEPROM_LIBS_PATH := $(call my-dir)

# ---------------------------------------------------------------------------
#        Make the shared library (libmmcamera_gt24c64a_s5k4e8_eeprom.so)
# ---------------------------------------------------------------------------

include $(CLEAR_VARS)
LOCAL_PATH := $(gt24c64a_s5k4e8_EEPROM_LIBS_PATH)
LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../includes/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../includes/0309

LOCAL_HEADER_LIBRARIES := libutils_headers
LOCAL_SRC_FILES        := gt24c64a_s5k4e8_eeprom.c
LOCAL_MODULE           := libmmcamera_gt24c64a_s5k4e8_eeprom
LOCAL_SHARED_LIBRARIES := liblog libcutils libmmcamera_eeprom_util

LOCAL_MODULE_OWNER := qti
LOCAL_PROPRIETARY_MODULE := true
LOCAL_32_BIT_ONLY := true

include $(BUILD_SHARED_LIBRARY)
