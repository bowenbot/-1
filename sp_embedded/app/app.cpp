#include "app.hpp"

extern "C" {
#include "main.h"
}

#include "io/buzzer_task.hpp"
#include "io/led_task.hpp"
#include "io/uart_logger.hpp"
#include "io/imu.hpp"

io::BuzzerTask buzzer_task;
io::LedTask led_task;
io::UartLogger uart_logger;
io::Imu imu;

extern "C" void app_main() {
  buzzer_task.init();
  led_task.init();
  uart_logger.init();
  imu.init();

  buzzer_task.beep(200);
  uart_logger.log("System boot OK\r\n");

  uint32_t tick = 0;

  while (1) {
    imu.update();

    // 每 100ms 走一步 LED 流水
    if (tick % 100 == 0) {
      led_task.flow_step();
    }

    // 每 500ms 打印一次 IMU 三轴
    if (tick % 500 == 0) {
      uart_logger.log("yaw: %.3f, pitch: %.3f, roll: %.3f\r\n",
                      imu.get_yaw_rad(), imu.get_pitch_rad(), imu.get_roll_rad());
    }

    tick++;
    HAL_Delay(1);
  }
}