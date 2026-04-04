"""Schema structure tests for powermust ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.powermust as hub  # noqa: E402
from components.powermust import (  # noqa: E402
    binary_sensor,
    sensor,
    switch,
    text_sensor,
)


class TestHubConstants:
    def test_conf_id_defined(self):
        assert hub.CONF_POWERMUST_ID == "powermust_id"


class TestBinarySensorTypes:
    def test_types_list_completeness(self):
        assert binary_sensor.CONF_UTILITY_FAIL in binary_sensor.TYPES
        assert binary_sensor.CONF_BATTERY_LOW in binary_sensor.TYPES
        assert binary_sensor.CONF_BYPASS_ACTIVE in binary_sensor.TYPES
        assert len(binary_sensor.TYPES) == 8

    def test_type_values_are_strings(self):
        for t in binary_sensor.TYPES:
            assert isinstance(t, str)


class TestSensorTypes:
    def test_types_dict_completeness(self):
        assert sensor.CONF_GRID_VOLTAGE in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_VOLTAGE in sensor.TYPES
        assert len(sensor.TYPES) == 11


class TestTextSensorTypes:
    def test_types_list_completeness(self):
        assert text_sensor.CONF_LAST_Q1 in text_sensor.TYPES
        assert text_sensor.CONF_LAST_F in text_sensor.TYPES
        assert len(text_sensor.TYPES) == 2


class TestSwitchTypes:
    def test_types_dict_completeness(self):
        assert len(switch.TYPES) == 4
