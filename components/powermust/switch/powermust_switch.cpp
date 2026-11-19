#include "powermust_switch.h"

namespace esphome::powermust {

void PowermustSwitch::write_state(bool state) {
  if (state) {
    if (!this->on_command_.empty()) {
      this->parent_->switch_command(this->on_command_);
    }
  } else {
    if (!this->off_command_.empty()) {
      this->parent_->switch_command(this->off_command_);
    }
  }
}

}  // namespace esphome::powermust
