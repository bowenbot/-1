#include "app.hpp"

extern "C" {
#include "main.h"
}

#include "io/buzzer_task.hpp"
#include "io/led_task.hpp"

io::BuzzerTask buzzer_task;
io::LedTask led_task;

extern "C" void app_main() {
  buzzer_task.init();
  led_task.init();

  buzzer_task.beep(200);

  while (1) {
    led_task.flow_step();
    HAL_Delay(100);
  }
}