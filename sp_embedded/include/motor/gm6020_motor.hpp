#ifndef GM6020_MOTOR_HPP
#define GM6020_MOTOR_HPP

#include <cstdint>

#include "motor/rm_motor/rm_motor.hpp"

namespace motor {

class Gm6020Motor {
 public:
  explicit Gm6020Motor(uint8_t motor_id);

  void set_current(float current);
  float get_angle_rad() const;
  float get_speed_rad_s() const;
  float get_torque_nm() const;
  uint8_t get_temperature() const;
  uint8_t motor_id() const;

  sp::RM_Motor& rm_motor();

 private:
  sp::RM_Motor rm_motor_;
};

void can_init();
void can_send_all();
void dispatch_can_rx(uint32_t std_id, uint8_t* data, uint32_t stamp_ms);

}  // namespace motor

#endif  // GM6020_MOTOR_HPP