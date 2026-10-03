#ifndef GIMBAL_CONTROLLER_HPP
#define GIMBAL_CONTROLLER_HPP

#include "io/imu.hpp"
#include "io/remote_control.hpp"
#include "motor/gm6020_motor.hpp"
#include "tools/pid/pid.hpp"

namespace control {

class GimbalController {
 public:
  GimbalController(io::Imu& imu, io::RemoteControl& remote);

  void init();
  void update();

 private:
  void update_disable_mode();
  void update_link_mode();
  void update_reset_mode();
  void update_ratio_from_left_switch();

  io::Imu& imu_;
  io::RemoteControl& remote_;

  motor::Gm6020Motor motor_a_;
  motor::Gm6020Motor motor_b_;

  sp::PID pid_a_;
  sp::PID pid_b_;

  float offset_a_rad_{0.0f};
  float offset_b_rad_{0.0f};
  float ratio_{1.0f};
  float last_yaw_rad_{0.0f};
  bool inited_{false};
};

}  // namespace control

#endif  // GIMBAL_CONTROLLER_HPP