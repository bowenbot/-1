#ifndef REMOTE_CONTROL_HPP
#define REMOTE_CONTROL_HPP

#include "io/dbus/dbus.hpp"

namespace io {

class RemoteControl {
 public:
  void init();

  sp::DBusSwitchMode get_right_switch() const;
  sp::DBusSwitchMode get_left_switch() const;

  float get_right_horizontal() const;
  float get_right_vertical() const;
  float get_left_horizontal() const;
  float get_left_vertical() const;
  float get_left_wheel() const;

  bool is_connected() const;
};

}  // namespace io

#endif  // REMOTE_CONTROL_HPP