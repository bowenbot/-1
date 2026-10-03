#include "app.hpp"

extern "C" {
#include "main.h"
}

#include "io/buzzer_task.hpp"
#include "io/led_task.hpp"
#include "io/uart_logger.hpp"

io::BuzzerTask buzzer_task;
io::LedTask led_task;
io::UartLogger uart_logger;

extern "C" void app_main() {
  buzzer_task.init();
  led_task.init();
  uart_logger.init();

  buzzer_task.beep(200);

  uart_logger.log("System boot OK\r\n");

  uint32_t counter = 0;

  while (1) {
    led_task.flow_step();
    uart_logger.log("counter: %lu\r\n", counter);
    counter++;
    HAL_Delay(100);
  }
}