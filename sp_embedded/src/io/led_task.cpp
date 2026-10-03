#include "io/led_task.hpp"

extern "C" {
#include "gpio.h"
}

namespace io {

void LedTask::init() {
  this->all_off();
}

void LedTask::all_off() {
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_RESET);
}

void LedTask::flow_step() {
  this->all_off();

  switch (this->led_index_) {
    case 0:
      HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET);
      break;
    case 1:
      HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);
      break;
    case 2:
      HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);
      break;
    default:
      break;
  }

  this->led_index_ = (this->led_index_ + 1) % 3;
}

}  // namespace io