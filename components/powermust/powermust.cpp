#include "powermust.h"
#include "esphome/core/log.h"

namespace esphome {
namespace powermust {

static const char *const TAG = "powermust";

void Powermust::setup() {
  this->state_ = STATE_IDLE;
  this->command_start_millis_ = 0;
}

void Powermust::empty_uart_buffer_() {
  uint8_t byte;
  while (this->available()) {
    this->read_byte(&byte);
  }
}

void Powermust::loop() {
  // Read message
  if (this->state_ == STATE_IDLE) {
    this->empty_uart_buffer_();
    switch (this->send_next_command_()) {
      case 0:
        // no command send (empty queue) time to poll
        if (millis() - this->last_poll_ > this->update_interval_) {
          this->send_next_poll_();
          this->last_poll_ = millis();
        }
        return;
        break;
      case 1:
        // command send
        return;
        break;
    }
  }
  if (this->state_ == STATE_COMMAND_COMPLETE) {
    if (this->read_pos_ > 0) {
      ESP_LOGE(TAG, "Command not successful");
    } else {
      ESP_LOGI(TAG, "Command successful");
    }
    this->command_queue_[this->command_queue_position_] = std::string("");
    this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
    this->state_ = STATE_IDLE;
  }

  if (this->state_ == STATE_POLL_DECODED) {
    std::string mode;
    switch (this->used_polling_commands_[this->last_polling_command_].identifier) {
      case POLLING_Q1:
        if (this->grid_voltage_) {
          this->grid_voltage_->publish_state(value_grid_voltage_);
        }
        if (this->grid_fault_voltage_) {
          this->grid_fault_voltage_->publish_state(value_grid_fault_voltage_);
        }
        if (this->ac_output_voltage_) {
          this->ac_output_voltage_->publish_state(value_ac_output_voltage_);
        }
        if (this->ac_output_load_percent_) {
          this->ac_output_load_percent_->publish_state(value_ac_output_load_percent_);
        }
        if (this->grid_frequency_) {
          this->grid_frequency_->publish_state(value_grid_frequency_);
        }
        if (this->battery_voltage_) {
          this->battery_voltage_->publish_state(value_battery_voltage_);
        }
        if (this->temperature_) {
          this->temperature_->publish_state(value_temperature_);
        }
        if (this->utility_fail_) {
          this->utility_fail_->publish_state(value_utility_fail_ == 1);
        }
        if (this->battery_low_) {
          this->battery_low_->publish_state(value_battery_low_ == 1);
        }
        if (this->bypass_active_) {
          this->bypass_active_->publish_state(value_bypass_active_ == 1);
        }
        if (this->ups_failed_) {
          this->ups_failed_->publish_state(value_ups_failed_ == 1);
        }
        if (this->ups_type_standby_) {
          this->ups_type_standby_->publish_state(value_ups_type_standby_ == 1);
        }
        if (this->test_in_progress_) {
          this->test_in_progress_->publish_state(value_test_in_progress_ == 1);
        }
        if (this->quick_test_switch_) {
          this->quick_test_switch_->publish_state(value_test_in_progress_ == 1);
        }
        if (this->deep_test_switch_) {
          this->deep_test_switch_->publish_state(value_test_in_progress_ == 1);
        }
        if (this->ten_minutes_test_switch_) {
          this->ten_minutes_test_switch_->publish_state(value_test_in_progress_ == 1);
        }
        if (this->shutdown_active_) {
          this->shutdown_active_->publish_state(value_shutdown_active_ == 1);
        }
        if (this->beeper_on_) {
          this->beeper_on_->publish_state(value_beeper_on_ == 1);
        }
        if (this->beeper_switch_) {
          this->beeper_switch_->publish_state(value_beeper_on_ == 1);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_F:
        if (this->ac_output_rating_voltage_) {
          this->ac_output_rating_voltage_->publish_state(value_ac_output_rating_voltage_);
        }
        if (this->ac_output_rating_current_) {
          this->ac_output_rating_current_->publish_state(value_ac_output_rating_current_);
        }
        if (this->battery_rating_voltage_) {
          this->battery_rating_voltage_->publish_state(value_battery_rating_voltage_);
        }
        if (this->ac_output_rating_frequency_) {
          this->ac_output_rating_frequency_->publish_state(value_ac_output_rating_frequency_);
        }
        this->state_ = STATE_IDLE;
        break;
    }
  }

  if (this->state_ == STATE_POLL_CHECKED) {
    std::string fc;
    char tmp[POWERMUST_READ_BUFFER_LENGTH];
    sprintf(tmp, "%s", this->read_buffer_);
    switch (this->used_polling_commands_[this->last_polling_command_].identifier) {
      case POLLING_Q1:
        ESP_LOGD(TAG, "Decode Q1");
        // (232.9 232.9 232.9 003 49.9 13.4 25.0 00001000\r
        // (005.2 232.4 226.8 002 50.1 12.9 25.0 10001000\r
        //                                 1  2  3  4  5  6  7  8
        sscanf(tmp, "(%f %f %f %d %f %f %f %1d%1d%1d%1d%1d%1d%1d%1d",  // NOLINT
               &value_grid_voltage_,                                   // NOLINT
               &value_grid_fault_voltage_,                             // NOLINT
               &value_ac_output_voltage_,                              // NOLINT
               &value_ac_output_load_percent_,                         // NOLINT
               &value_grid_frequency_,                                 // NOLINT
               &value_battery_voltage_,                                // NOLINT
               &value_temperature_,                                    // NOLINT
               &value_utility_fail_,                                   // NOLINT
               &value_battery_low_,                                    // NOLINT
               &value_bypass_active_,                                  // NOLINT
               &value_ups_failed_,                                     // NOLINT
               &value_ups_type_standby_,                               // NOLINT
               &value_test_in_progress_,                               // NOLINT
               &value_shutdown_active_,                                // NOLINT
               &value_beeper_on_);                                     // NOLINT
        if (this->last_q1_) {
          this->last_q1_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_F:
        ESP_LOGD(TAG, "Decode F");
        // "#220.0 003 12.00 50.0\r"
        sscanf(                                   // NOLINT
            tmp,                                  // NOLINT
            "#%f %d %f %f",                       // NOLINT
            &value_ac_output_rating_voltage_,     // NOLINT
            &value_ac_output_rating_current_,     // NOLINT
            &value_battery_rating_voltage_,       // NOLINT
            &value_ac_output_rating_frequency_);  // NOLINT
        if (this->last_f_) {
          this->last_f_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      default:
        this->state_ = STATE_IDLE;
        break;
    }
    return;
  }

  if (this->state_ == STATE_POLL_COMPLETE) {
    if (this->read_buffer_[0] == '(' && this->read_buffer_[1] == 'N' && this->read_buffer_[2] == 'A' &&
        this->read_buffer_[3] == 'K') {
      this->state_ = STATE_IDLE;
      return;
    }
    this->state_ = STATE_POLL_CHECKED;
    return;
  }

  if (this->state_ == STATE_COMMAND || this->state_ == STATE_POLL) {
    while (this->available()) {
      uint8_t byte;
      this->read_byte(&byte);

      if (this->read_pos_ == POWERMUST_READ_BUFFER_LENGTH) {
        this->read_pos_ = 0;
        this->empty_uart_buffer_();
      }
      this->read_buffer_[this->read_pos_] = byte;
      this->read_pos_++;

      // end of answer
      if (byte == 0x0D) {
        this->read_buffer_[this->read_pos_] = 0;
        this->empty_uart_buffer_();
        if (this->state_ == STATE_POLL) {
          this->state_ = STATE_POLL_COMPLETE;
        }
        if (this->state_ == STATE_COMMAND) {
          this->state_ = STATE_COMMAND_COMPLETE;
        }
      }
    }  // available
  }
  if (this->state_ == STATE_COMMAND) {
    // Wait for a negative response. On success the UPS doesn't respond
    if (millis() - this->command_start_millis_ > esphome::powermust::Powermust::COMMAND_TIMEOUT) {
      this->command_start_millis_ = millis();
      this->state_ = STATE_COMMAND_COMPLETE;
      return;
    }
  }
  if (this->state_ == STATE_POLL) {
    if (millis() - this->command_start_millis_ > esphome::powermust::Powermust::COMMAND_TIMEOUT) {
      // command timeout
      ESP_LOGD(TAG, "timeout command to poll: %s", this->used_polling_commands_[this->last_polling_command_].command);
      this->state_ = STATE_IDLE;
    }
  }
}

uint8_t Powermust::check_incoming_length_(uint8_t length) {
  if (this->read_pos_ - 3 == length) {
    return 1;
  }
  return 0;
}

// send next command used
uint8_t Powermust::send_next_command_() {
  if (this->command_queue_[this->command_queue_position_].length() != 0) {
    const char *command = this->command_queue_[this->command_queue_position_].c_str();
    uint8_t byte_command[16];
    uint8_t length = this->command_queue_[this->command_queue_position_].length();
    for (uint8_t i = 0; i < length; i++) {
      byte_command[i] = (uint8_t) this->command_queue_[this->command_queue_position_].at(i);
    }
    this->state_ = STATE_COMMAND;
    this->command_start_millis_ = millis();
    this->empty_uart_buffer_();
    this->read_pos_ = 0;
    this->write_str(command);
    // end Byte
    this->write(0x0D);
    ESP_LOGD(TAG, "Sending command from queue: %s with length %d", command, length);
    return 1;
  }
  return 0;
}

void Powermust::send_next_poll_() {
  this->last_polling_command_ = (this->last_polling_command_ + 1) % 15;
  if (this->used_polling_commands_[this->last_polling_command_].length == 0) {
    this->last_polling_command_ = 0;
  }
  if (this->used_polling_commands_[this->last_polling_command_].length == 0) {
    // no command specified
    return;
  }
  this->state_ = STATE_POLL;
  this->command_start_millis_ = millis();
  this->empty_uart_buffer_();
  this->read_pos_ = 0;
  this->write_array(this->used_polling_commands_[this->last_polling_command_].command,
                    this->used_polling_commands_[this->last_polling_command_].length);
  // end Byte
  this->write(0x0D);
  ESP_LOGD(TAG, "Sending polling command : %s with length %d",
           this->used_polling_commands_[this->last_polling_command_].command,
           this->used_polling_commands_[this->last_polling_command_].length);
}

void Powermust::queue_command_(const char *command, uint8_t length) {
  uint8_t next_position = command_queue_position_;
  for (uint8_t i = 0; i < COMMAND_QUEUE_LENGTH; i++) {
    uint8_t testposition = (next_position + i) % COMMAND_QUEUE_LENGTH;
    if (command_queue_[testposition].length() == 0) {
      command_queue_[testposition] = command;
      ESP_LOGD(TAG, "Command queued successfully: %s with length %u at position %d", command,
               command_queue_[testposition].length(), testposition);
      return;
    }
  }
  ESP_LOGD(TAG, "Command queue full dropping command: %s", command);
}

void Powermust::switch_command(const std::string &command) {
  ESP_LOGD(TAG, "got command: %s", command.c_str());
  queue_command_(command.c_str(), command.length());
}
void Powermust::dump_config() {
  ESP_LOGCONFIG(TAG, "Powermust:");
  ESP_LOGCONFIG(TAG, "used commands:");
  for (auto &used_polling_command : this->used_polling_commands_) {
    if (used_polling_command.length != 0) {
      ESP_LOGCONFIG(TAG, "%s", used_polling_command.command);
    }
  }
}
void Powermust::update() {}

void Powermust::add_polling_command_(const char *command, ENUMPollingCommand polling_command) {
  for (auto &used_polling_command : this->used_polling_commands_) {
    if (used_polling_command.length == strlen(command)) {
      uint8_t len = strlen(command);
      if (memcmp(used_polling_command.command, command, len) == 0) {
        return;
      }
    }
    if (used_polling_command.length == 0) {
      size_t length = strlen(command) + 1;
      const char *beg = command;
      const char *end = command + length;
      used_polling_command.command = new uint8_t[length];  // NOLINT(cppcoreguidelines-owning-memory)
      size_t i = 0;
      for (; beg != end; ++beg, ++i) {
        used_polling_command.command[i] = (uint8_t) (*beg);
      }
      used_polling_command.errors = 0;
      used_polling_command.identifier = polling_command;
      used_polling_command.length = length - 1;
      return;
    }
  }
}

}  // namespace powermust
}  // namespace esphome
