/******************************************************************************
 * Copyright 2020 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/canbus/vehicle/devkit/protocol/brake_command_101.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace devkit {

using ::apollo::drivers::canbus::Byte;

const int32_t Brakecommand101::ID = 0x101;

// public
Brakecommand101::Brakecommand101() { Reset(); }

uint32_t Brakecommand101::GetPeriod() const {
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20 * 1000;
  return PERIOD;
}

void Brakecommand101::UpdateData(uint8_t* data) {
  set_p_brake_dec(data, brake_dec_);
  set_p_brake_pedal_target(data, brake_pedal_target_);
  set_p_brake_en_ctrl(data, brake_en_ctrl_);
  checksum_111_ =
      data[0] ^ data[1] ^ data[2] ^ data[3] ^ data[4] ^ data[5] ^ data[6];
  set_p_checksum_111(data, checksum_111_);
}

void Brakecommand101::Reset() {
  // TODO(All) :  you should check this manually
  brake_dec_ = 0.0;
  checksum_111_ = 0;
  brake_pedal_target_ = 0.0;
  brake_en_ctrl_ = Brake_command_101::BRAKE_EN_CTRL_DISABLE;
}

Brakecommand101* Brakecommand101::set_brake_dec(double brake_dec) {
  brake_dec_ = brake_dec;
  return this;
}

// config detail: {'name': 'Brake_Dec', 'offset': 0.0, 'precision': 0.01, 'len':
// 10, 'is_signed_var': False, 'physical_range': '[0|10]', 'bit': 15, 'type':
// 'double', 'order': 'motorola', 'physical_unit': 'm/s^2'}
void Brakecommand101::set_p_brake_dec(uint8_t* data, double brake_dec) {
  brake_dec = ProtocolData::BoundedValue(0.0, 10.0, brake_dec);
  int x = brake_dec / 0.010000;
  uint8_t t = 0;

  t = x & 0x3;
  Byte to_set0(data + 2);
  to_set0.set_value(t, 6, 2);
  x >>= 2;

  t = x & 0xFF;
  Byte to_set1(data + 1);
  to_set1.set_value(t, 0, 8);
}

Brakecommand101* Brakecommand101::set_checksum_111(int checksum_111) {
  checksum_111_ = checksum_111;
  return this;
}

// config detail: {'name': 'CheckSum_111', 'offset': 0.0, 'precision': 1.0,
// 'len': 8, 'is_signed_var': False, 'physical_range': '[0|255]', 'bit': 63,
// 'type': 'int', 'order': 'motorola', 'physical_unit': ''}
void Brakecommand101::set_p_checksum_111(uint8_t* data, int checksum_111) {
  checksum_111 = ProtocolData::BoundedValue(0, 255, checksum_111);
  int x = checksum_111;

  Byte to_set(data + 7);
  to_set.set_value(x, 0, 8);
}

Brakecommand101* Brakecommand101::set_brake_pedal_target(
    double brake_pedal_target) {
  brake_pedal_target_ = brake_pedal_target;
  return this;
}

// config detail: {'name': 'Brake_Pedal_Target', 'offset': 0.0, 'precision':
// 0.1, 'len': 16, 'is_signed_var': False, 'physical_range': '[0|100]', 'bit':
// 31, 'type': 'double', 'order': 'motorola', 'physical_unit': '%'}
void Brakecommand101::set_p_brake_pedal_target(uint8_t* data,
                                               double brake_pedal_target) {
  brake_pedal_target =
      ProtocolData::BoundedValue(0.0, 100.0, brake_pedal_target);
  int x = brake_pedal_target / 0.100000;
  uint8_t t = 0;

  t = x & 0xFF;
  Byte to_set0(data + 4);
  to_set0.set_value(t, 0, 8);
  x >>= 8;

  t = x & 0xFF;
  Byte to_set1(data + 3);
  to_set1.set_value(t, 0, 8);
}

Brakecommand101* Brakecommand101::set_brake_en_ctrl(
    Brake_command_101::Brake_en_ctrlType brake_en_ctrl) {
  brake_en_ctrl_ = brake_en_ctrl;
  return this;
}

// config detail: {'name': 'Brake_EN_CTRL', 'enum': {0: 'BRAKE_EN_CTRL_DISABLE',
// 1: 'BRAKE_EN_CTRL_ENABLE'}, 'precision': 1.0, 'len': 1, 'is_signed_var':
// False, 'offset': 0.0, 'physical_range': '[0|1]', 'bit': 0, 'type': 'enum',
// 'order': 'motorola', 'physical_unit': ''}
void Brakecommand101::set_p_brake_en_ctrl(
    uint8_t* data, Brake_command_101::Brake_en_ctrlType brake_en_ctrl) {
  int x = brake_en_ctrl;

  Byte to_set(data + 0);
  to_set.set_value(x, 0, 1);
}

}  // namespace devkit
}  // namespace canbus
}  // namespace apollo
