/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "testing/common/HIDState.h"

#include "HID-Settings.h"

#include "testing/common/fix-macros.h"

// TODO(epan): Add proper logging.
#include <iostream>
#define LOG(x) std::cout

namespace kaleidoscope {
namespace testing {

const std::vector<KeyboardReport>& HIDState::Keyboard() const {
  return keyboard_reports_;
}

const KeyboardReport& HIDState::Keyboard(size_t i) const {
  return keyboard_reports_.at(i);
}

const std::vector<SystemControlReport>& HIDState::SystemControl() const {
  return system_control_reports_;
}

const SystemControlReport& HIDState::SystemControl(size_t i) const {
  return system_control_reports_.at(i);
}

namespace internal {

// static
void HIDStateBuilder::ProcessHidReport(
    uint8_t id, const void* data, int len, int result) {
  switch (id) {
    case HID_REPORTID_MOUSE: {
      LOG(ERROR) << "Dropped MouseReport: unimplemented";
      break;
    }
    case HID_REPORTID_KEYBOARD: {
      LOG(ERROR) << "Dropped BootKeyboardReport: unimplemented";
      break;
    }
    case HID_REPORTID_GAMEPAD:
    case HID_REPORTID_CONSUMERCONTROL: {
      // TODO: React appropriately to these.
      LOG(INFO) << "Ignoring HID report with id = " << id;
      break;
    }
    case HID_REPORTID_SYSTEMCONTROL: {
      ProcessSystemControlReport(SystemControlReport{data});
      break;
    }
    case HID_REPORTID_MOUSE_ABSOLUTE: {
      LOG(ERROR) << "Dropped AbsoluteMouseReport: unimplemented";
      break;
    }
    case HID_REPORTID_NKRO_KEYBOARD: {
      ProcessKeyboardReport(KeyboardReport{data});
      break;
    }
    default:
      LOG(ERROR) << "Encountered unknown HID report with id = " << id;
  }
}

// static
std::unique_ptr<HIDState> HIDStateBuilder::Snapshot() {
  auto hid_state = std::make_unique<HIDState>();
  // Populate state.
  // TODO: Grab a copy of current instantaneous state, like:
  //  key states, layer stack, led states
  hid_state->keyboard_reports_ = std::move(keyboard_reports_);
  hid_state->system_control_reports_ = std::move(system_control_reports_);

  Clear();  // Clear global state.
  return hid_state;
}

// static
void HIDStateBuilder::Clear() {
  keyboard_reports_.clear();
  system_control_reports_.clear();
}

// static
void HIDStateBuilder::ProcessKeyboardReport(const KeyboardReport& report) {
  keyboard_reports_.push_back(report);
}

// static
void HIDStateBuilder::ProcessSystemControlReport(const SystemControlReport& report) {
  system_control_reports_.push_back(report);
}

// static
std::vector<KeyboardReport> HIDStateBuilder::keyboard_reports_;
// static
std::vector<SystemControlReport> HIDStateBuilder::system_control_reports_;

}  // namesapce internal
}  // namespace testing
}  // namespace kaleidoscope