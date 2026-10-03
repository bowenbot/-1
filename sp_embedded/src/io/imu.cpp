#include "io/imu.hpp"

extern "C" {
#include "main.h"
#include "spi.h"
}

#include "io/bmi088/bmi088.hpp"
#include "tools/mahony/mahony.hpp"

namespace {

// BMI088 原始坐标系{b}在机器人坐标系{a}下的旋转矩阵
// 默认配置：C板横着装在云台上，CAN一侧朝前
// 等板子到了，根据实际安装方向调整
const float kR_ab[3][3] = {
    {0.0f, -1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
};

sp::BMI088 g_bmi088(&hspi1, GPIOA, GPIO_PIN_4, GPIOB, GPIO_PIN_0, kR_ab);
sp::Mahony g_mahony(1e-3f);

}  // namespace

namespace io {

void Imu::init() {
  g_bmi088.init();
}

void Imu::update() {
  g_bmi088.update();
  g_mahony.update(g_bmi088.acc, g_bmi088.gyro);
}

float Imu::get_yaw_rad() const {
  return g_mahony.yaw;
}

float Imu::get_pitch_rad() const {
  return g_mahony.pitch;
}

float Imu::get_roll_rad() const {
  return g_mahony.roll;
}

float Imu::get_temp_celsius() const {
  return g_bmi088.temp;
}

}  // namespace io