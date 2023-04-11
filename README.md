# esphome-powermust

![GitHub actions](https://github.com/syssi/esphome-powermust/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-powermust)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-powermust)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-powermust)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor and control a Mustek PowerMust 800 UPS via RS232

## Supported devices

* Mustek PowerMust 800 USB

## Untested devices

* TBD.

## Requirements

* [ESPHome 2023.3.0 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32/ESP8266 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-powermust@main
```

or just use the `esp32-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-powermust.git
cd esphome-powermust

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
# If you use a esp8266 run the esp8266-examle.yaml
esphome run esp32-example.yaml

```

## Example response all sensors enabled

```
TBD.
```

## Protocol

See [https://networkupstools.org/protocols/megatec.html](networkupstools.org/protocols/megatec.html).

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to increase the log level to see details about the UART traffic:

```
logger:
  level: DEBUG

uart:
  - id: uart0
    baud_rate: 2400
    tx_pin: ${tx_pin}
    rx_pin: ${rx_pin}
    debug:
      direction: BOTH
      dummy_receiver: false
      after:
        delimiter: "\r"
      sequence:
        - lambda: UARTDebug::log_string(direction, bytes);
```

## References

* https://networkupstools.org/protocols/megatec.html
