#include "io/buzzer_task.hpp"

extern "C" {
#include "tim.h"
}

namespace io {

void BuzzerTask::init() {
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void BuzzerTask::beep(uint32_t duration_ms) {
  this->duration_ms_ = duration_ms;
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 125);
  HAL_Delay(this->duration_ms_);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

void BuzzerTask::stop() {
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

}  // namespace io