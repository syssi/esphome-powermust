#include <gtest/gtest.h>
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "common.h"

namespace esphome::powermust::testing {

// Responses sourced from tests/esp8266-megatec-protocol.yaml

class PowermustQ1DecodeTest : public ::testing::Test {
 protected:
  TestablePowermust ups_;
  sensor::Sensor grid_voltage_;
  sensor::Sensor grid_fault_voltage_;
  sensor::Sensor ac_output_voltage_;
  sensor::Sensor ac_output_load_percent_;
  sensor::Sensor grid_frequency_;
  sensor::Sensor battery_voltage_;
  sensor::Sensor temperature_;
  binary_sensor::BinarySensor utility_fail_;
  binary_sensor::BinarySensor battery_low_;
  binary_sensor::BinarySensor bypass_active_;
  binary_sensor::BinarySensor ups_failed_;
  binary_sensor::BinarySensor ups_type_standby_;
  binary_sensor::BinarySensor test_in_progress_;
  binary_sensor::BinarySensor shutdown_active_;
  binary_sensor::BinarySensor beeper_on_;
  text_sensor::TextSensor last_q1_;

  void SetUp() override {
    ups_.set_grid_voltage(&grid_voltage_);
    ups_.set_grid_fault_voltage(&grid_fault_voltage_);
    ups_.set_ac_output_voltage(&ac_output_voltage_);
    ups_.set_ac_output_load_percent(&ac_output_load_percent_);
    ups_.set_grid_frequency(&grid_frequency_);
    ups_.set_battery_voltage(&battery_voltage_);
    ups_.set_temperature(&temperature_);
    ups_.set_utility_fail(&utility_fail_);
    ups_.set_battery_low(&battery_low_);
    ups_.set_bypass_active(&bypass_active_);
    ups_.set_ups_failed(&ups_failed_);
    ups_.set_ups_type_standby(&ups_type_standby_);
    ups_.set_test_in_progress(&test_in_progress_);
    ups_.set_shutdown_active(&shutdown_active_);
    ups_.set_beeper_on(&beeper_on_);
    ups_.set_last_q1(&last_q1_);
  }
};

TEST_F(PowermustQ1DecodeTest, GridVoltage) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(grid_voltage_.state, 232.9f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, GridFaultVoltage) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(grid_fault_voltage_.state, 232.9f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, AcOutputVoltage) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(ac_output_voltage_.state, 232.9f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, AcOutputLoadPercent) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_FLOAT_EQ(ac_output_load_percent_.state, 3.0f);
}

TEST_F(PowermustQ1DecodeTest, GridFrequency) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(grid_frequency_.state, 49.9f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, BatteryVoltage) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(battery_voltage_.state, 13.4f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, Temperature) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_NEAR(temperature_.state, 25.0f, 0.05f);
}

TEST_F(PowermustQ1DecodeTest, UtilityFailFalse) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_FALSE(utility_fail_.state);
}

TEST_F(PowermustQ1DecodeTest, BatteryLowFalse) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_FALSE(battery_low_.state);
}

TEST_F(PowermustQ1DecodeTest, UpsTypeStandbyTrue) {
  // bit 5 of "00001000" = 1
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_TRUE(ups_type_standby_.state);
}

TEST_F(PowermustQ1DecodeTest, BeeperOnFalse) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_FALSE(beeper_on_.state);
}

TEST_F(PowermustQ1DecodeTest, UtilityFailTrue) {
  // bit 1 of "10001000" = 1 (on battery)
  ups_.decode_and_publish(POLLING_Q1, "(005.2 232.4 226.8 002 50.1 12.9 25.0 10001000\r");
  EXPECT_TRUE(utility_fail_.state);
}

TEST_F(PowermustQ1DecodeTest, LastQ1Raw) {
  ups_.decode_and_publish(POLLING_Q1, "(232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r");
  EXPECT_EQ(last_q1_.state.substr(0, 10), "(232.9 232");
}

class PowermustFDecodeTest : public ::testing::Test {
 protected:
  TestablePowermust ups_;
  sensor::Sensor ac_output_rating_voltage_;
  sensor::Sensor ac_output_rating_current_;
  sensor::Sensor battery_rating_voltage_;
  sensor::Sensor ac_output_rating_frequency_;
  text_sensor::TextSensor last_f_;

  void SetUp() override {
    ups_.set_ac_output_rating_voltage(&ac_output_rating_voltage_);
    ups_.set_ac_output_rating_current(&ac_output_rating_current_);
    ups_.set_battery_rating_voltage(&battery_rating_voltage_);
    ups_.set_ac_output_rating_frequency(&ac_output_rating_frequency_);
    ups_.set_last_f(&last_f_);
  }
};

TEST_F(PowermustFDecodeTest, AcOutputRatingVoltage) {
  ups_.decode_and_publish(POLLING_F, "#220.0 003 12.00 50.0\r");
  EXPECT_NEAR(ac_output_rating_voltage_.state, 220.0f, 0.05f);
}

TEST_F(PowermustFDecodeTest, AcOutputRatingCurrent) {
  ups_.decode_and_publish(POLLING_F, "#220.0 003 12.00 50.0\r");
  EXPECT_FLOAT_EQ(ac_output_rating_current_.state, 3.0f);
}

TEST_F(PowermustFDecodeTest, BatteryRatingVoltage) {
  ups_.decode_and_publish(POLLING_F, "#220.0 003 12.00 50.0\r");
  EXPECT_NEAR(battery_rating_voltage_.state, 12.0f, 0.05f);
}

TEST_F(PowermustFDecodeTest, AcOutputRatingFrequency) {
  ups_.decode_and_publish(POLLING_F, "#220.0 003 12.00 50.0\r");
  EXPECT_NEAR(ac_output_rating_frequency_.state, 50.0f, 0.05f);
}

}  // namespace esphome::powermust::testing
