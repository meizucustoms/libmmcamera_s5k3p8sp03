/*
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Copyright (c) 2022 MeizuCustoms inc.
 */

#include <stdio.h>
#include "s5k3p8sp03_lib.h"
#include "debug_lib.h"

/**
 * FUNCTION: s5k3p8sp03_real_to_register_gain
 *
 * DESCRIPTION: Calcuate sensor analog gain register value
 **/
static uint32_t s5k3p8sp03_real_to_register_gain(float real_gain)
{
  uint32_t reg_gain = 0;

  if (real_gain < 1.0){
    real_gain = 1.0;
  }
  else if (real_gain > 16.0){
    real_gain = 16.0;
  }

  reg_gain = (uint32_t)(real_gain * 32.0);

  return reg_gain;
}

/**
 * FUNCTION: s5k3p8sp03_register_to_real_gain
 *
 * DESCRIPTION: Calcuate the sensor exposure
 **/
static float s5k3p8sp03_register_to_real_gain(uint32_t reg_gain)
{
  float real_gain;

  if (reg_gain > 512) { // >> 9)
    reg_gain = 512;
  }

  real_gain = (float)reg_gain / 32.0; // * 0.03125;

  return real_gain;
}

static uint32_t s5k3p8sp03_digital_gain_calc(float real_gain, float sensor_real_gain)
{
  uint32_t reg_dig_gain = s5k3p8sp03_MIN_DGAIN_REG_VAL;
  float real_dig_gain = s5k3p8sp03_MIN_DGAIN;

  if (real_gain > 16.0){
    real_dig_gain = real_gain / sensor_real_gain;
  } else {
    real_dig_gain = s5k3p8sp03_MIN_DGAIN;
  }

  if (real_dig_gain > s5k3p8sp03_MAX_DGAIN) {
    real_dig_gain = s5k3p8sp03_MAX_DGAIN;
  }
  reg_dig_gain = (uint32_t)(real_dig_gain * 256);
  return reg_dig_gain;
}
/**
 * FUNCTION: s5k3p8sp03_calculate_exposure
 *
 * DESCRIPTION: Calcuate the sensor exposure
 **/
int32_t s5k3p8sp03_calculate_exposure(float real_gain,
  unsigned int line_count, sensor_exposure_info_t *exp_info, float s_real_gain)
{
  float real_gain_temp = s_real_gain;
  if (!exp_info){
    return -1;
  }

  exp_info->reg_gain = s5k3p8sp03_real_to_register_gain(real_gain);
  exp_info->sensor_real_gain = s5k3p8sp03_register_to_real_gain(exp_info->reg_gain);
  exp_info->sensor_digital_gain =
    s5k3p8sp03_digital_gain_calc(real_gain, exp_info->sensor_real_gain);
  exp_info->sensor_real_dig_gain = exp_info->sensor_digital_gain / 256;
  exp_info->digital_gain = real_gain /
    (exp_info->sensor_real_gain * exp_info->sensor_real_dig_gain);
  exp_info->line_count = line_count;

  return 0;
}

/**
 * FUNCTION: s5k3p8sp03_fill_exposure_array
 *
 * DESCRIPTION: Fill the sensor exposure array
 **/
int32_t s5k3p8sp03_fill_exposure_array(unsigned int gain,
  unsigned int digital_gain, unsigned int line, unsigned int fl_lines,
  __attribute__((unused)) int luma_avg, unsigned int hdr_param,
  struct camera_i2c_reg_setting* reg_setting,
  __attribute__((unused)) unsigned int s_gain,
  __attribute__((unused)) int s_linecount,
  __attribute__((unused)) int is_hdr_enabled)
{
  uint16_t reg_count = 0;
  uint16_t i = 0;
  unsigned int digital_gain_temp = digital_gain;
  unsigned int hdr_param_temp = hdr_param;

  if (!reg_setting) {
    return -1;
  }

  for (i = 0; i < sensor_lib_ptr.groupon_settings.size; i++) {
    reg_setting->reg_setting[reg_count].reg_addr =
      sensor_lib_ptr.groupon_settings.reg_setting_a[i].reg_addr;
    reg_setting->reg_setting[reg_count].reg_data =
      sensor_lib_ptr.groupon_settings.reg_setting_a[i].reg_data;
    reg_count = reg_count + 1;
  }

    reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.output_reg_addr.frame_length_lines;
  reg_setting->reg_setting[reg_count].reg_data = (fl_lines & 0xFF00) >> 8;
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.output_reg_addr.frame_length_lines + 1;
  reg_setting->reg_setting[reg_count].reg_data = (fl_lines & 0xFF);
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info.coarse_int_time_addr;
  reg_setting->reg_setting[reg_count].reg_data = (line & 0xFF00) >> 8;
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info.coarse_int_time_addr + 1;
  reg_setting->reg_setting[reg_count].reg_data = (line & 0xFF);
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info.global_gain_addr;
  reg_setting->reg_setting[reg_count].reg_data = (gain & 0xFF00) >> 8;
  reg_count++;

  reg_setting->reg_setting[reg_count].reg_addr =
    sensor_lib_ptr.exp_gain_info.global_gain_addr + 1;
  reg_setting->reg_setting[reg_count].reg_data = (gain & 0xFF);
  reg_count++;

  for (i = 0; i < sensor_lib_ptr.groupoff_settings.size; i++) {
    reg_setting->reg_setting[reg_count].reg_addr =
      sensor_lib_ptr.groupoff_settings.reg_setting_a[i].reg_addr;
    reg_setting->reg_setting[reg_count].reg_data =
      sensor_lib_ptr.groupoff_settings.reg_setting_a[i].reg_data;
    reg_count = reg_count + 1;
  }

  reg_setting->size = reg_count;
  reg_setting->addr_type = CAMERA_I2C_WORD_ADDR;
  reg_setting->data_type = CAMERA_I2C_BYTE_DATA;
  reg_setting->delay = 0;

    return 0;
}

/**
 * FUNCTION: sensor_open_lib
 *
 * DESCRIPTION: Open sensor library and returns data pointer
 **/
void *sensor_open_lib(void)
{
  return &sensor_lib_ptr;
}
