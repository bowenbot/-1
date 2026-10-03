#include "control/gimbal_controller.hpp"

#include <cmath>

extern "C" {
#include "main.h"
}

namespace control {

namespace {

constexpr float kRatioDown = 0.5f;
constexpr float kRatioMid = -1.0f;
constexpr float kRatioUp = 3.0f;

// 手动转动检测阈值
constexpr float kManualThresholdRad = 0.15f;
constexpr float kYawStableThresholdRad = 0.02f;

// PID 参数
constexpr float kPidKp = 30.0f;
constexpr float kPidKi = 0.0f;
constexpr float kPidKd = 0.5f;
constexpr float kPidMaxOut = 2.0f;
constexpr float kPidMaxIout = 0.0f;
constexpr float kPidAlpha = 0.7f;
constexpr float kDt = 1e-3f;

// 复位目标偏移（等板子到了根据机械箭头对齐标定）
constexpr float kResetOffsetA = 0.0f;
constexpr float kResetOffsetB = 0.0f;

}  // namespace

GimbalController::GimbalController(io::Imu& imu, io::RemoteControl& remote)
    : imu_(imu),
      remote_(remote),
      motor_a_(1),
      motor_b_(2),
      pid_a_(kDt, kPidKp, kPidKi, kPidKd, kPidMaxOut, kPidMaxIout, kPidAlpha),
      pid_b_(kDt, kPidKp, kPidKi, kPidKd, kPidMaxOut, kPidMaxIout, kPidAlpha) {}

void GimbalController::init() {
  this->inited_ = false;
}

void GimbalController::update_ratio_from_left_switch() {
  switch (this->remote_.get_left_switch()) {
    case sp::DBusSwitchMode::DOWN:
      this->ratio_ = kRatioDown;
      break;
    case sp::DBusSwitchMode::MID:
      this->ratio_ = kRatioMid;
      break;
    case sp::DBusSwitchMode::UP:
      this->ratio_ = kRatioUp;
      break;
  }
}

void GimbalController::update_disable_mode() {
  this->motor_a_.set_current(0.0f);
  this->motor_b_.set_current(0.0f);
  this->inited_ = false;
}

void GimbalController::update_link_mode() {
  const float yaw = this->imu_.get_yaw_rad();
  const float a_angle = this->motor_a_.get_angle_rad();
  const float b_angle = this->motor_b_.get_angle_rad();

  if (!this->inited_) {
    this->offset_a_rad_ = a_angle - yaw;
    this->offset_b_rad_ = b_angle - this->ratio_ * a_angle;
    this->last_yaw_rad_ = yaw;
    this->inited_ = true;
  }

  const float yaw_delta = yaw - this->last_yaw_rad_;

  // 检测手动转 A
  const float target_a_before = yaw + this->offset_a_rad_;
  if (std::fabs(a_angle - target_a_before) > kManualThresholdRad &&
      std::fabs(yaw_delta) < kYawStableThresholdRad) {
    this->offset_a_rad_ = a_angle - yaw;
  }

  // 检测手动转 B
  const float target_b_before = this->ratio_ * target_a_before + this->offset_b_rad_;
  if (std::fabs(b_angle - target_b_before) > kManualThresholdRad &&
      std::fabs(yaw_delta) < kYawStableThresholdRad) {
    const float target_a_new = (b_angle - this->offset_b_rad_) / this->ratio_;
    this->offset_a_rad_ = target_a_new - yaw;
  }

  const float target_a = yaw + this->offset_a_rad_;
  const float target_b = this->ratio_ * target_a + this->offset_b_rad_;

  this->pid_a_.calc(target_a, a_angle);
  this->pid_b_.calc(target_b, b_angle);

  this->motor_a_.set_current(this->pid_a_.out);
  this->motor_b_.set_current(this->pid_b_.out);

  this->last_yaw_rad_ = yaw;
}

void GimbalController::update_reset_mode() {
  const float yaw = this->imu_.get_yaw_rad();
  const float a_angle = this->motor_a_.get_angle_rad();
  const float b_angle = this->motor_b_.get_angle_rad();

  const float target_a = yaw + kResetOffsetA;
  const float target_b = yaw + kResetOffsetB;

  this->pid_a_.calc(target_a, a_angle);
  this->pid_b_.calc(target_b, b_angle);

  this->motor_a_.set_current(this->pid_a_.out);
  this->motor_b_.set_current(this->pid_b_.out);

  this->inited_ = false;
}

void GimbalController::update() {
  this->update_ratio_from_left_switch();

  switch (this->remote_.get_right_switch()) {
    case sp::DBusSwitchMode::DOWN:
      this->update_disable_mode();
      break;
    case sp::DBusSwitchMode::MID:
      this->update_link_mode();
      break;
    case sp::DBusSwitchMode::UP:
      this->update_reset_mode();
      break;
  }
}

}  // namespace control