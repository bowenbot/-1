#ifndef IMU_HPP
#define IMU_HPP

namespace io {

class Imu {
 public:
  void init();
  void update();
  float get_yaw_rad() const;
  float get_pitch_rad() const;
  float get_roll_rad() const;
  float get_temp_celsius() const;
};

}  // namespace io

#endif  // IMU_HPP