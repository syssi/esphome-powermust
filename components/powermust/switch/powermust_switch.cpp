#include "powermust_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace powermust {

static const char *const TAG = "powermust.switch";

void PowermustSwitch::dump_config() { LOG_SWITCH("", "Powermust Switch", this); }
void PowermustSwitch::write_state(bool state) {
  if (state) {
    if (this->on_command_.length() > 0) {
      this->parent_->switch_command(this->on_command_);
    }
  } else {
    if (this->off_command_.length() > 0) {
      this->parent_->switch_command(this->off_command_);
    }
  }
}

}  // namespace powermust
}  // namespace esphome
