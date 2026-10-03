#include "app.hpp"

extern "C" {
#include "main.h"
}

#include "io/buzzer_task.hpp"
#include "io/led_task.hpp"
#include "io/uart_logger.hpp"
#include "io/imu.hpp"
#include "io/remote_control.hpp"

io::BuzzerTask buzzer_task;
io::LedTask led_task;
io::UartLogger uart_logger;
io::Imu imu;
io::RemoteControl remote_control;

extern "C" void app_main() {
  buzzer_task.init();
  led_task.init();
  uart_logger.init();
  imu.init();
  remote_control.init();

  buzzer_task.beep(200);
  uart_logger.log("System boot OK\r\n");

  uint32_t tick = 0;

  while (1) {
    imu.update();

    if (tick % 100 == 0) {
      led_task.flow_step();
    }

    if (tick % 500 == 0) {
      uart_logger.log("yaw: %.3f, pitch: %.3f, roll: %.3f\r\n",
                      imu.get_yaw_rad(), imu.get_pitch_rad(), imu.get_roll_rad());
    }

    if (tick % 200 == 0) {
      const char *sw_r_str = "DOWN";
      const char *sw_l_str = "DOWN";

      if (remote_control.get_right_switch() == sp::DBusSwitchMode::UP) {
        sw_r_str = "UP";
      } else if (remote_control.get_right_switch() == sp::DBusSwitchMode::MID) {
        sw_r_str = "MID";
      }

      if (remote_control.get_left_switch() == sp::DBusSwitchMode::UP) {
        sw_l_str = "UP";
      } else if (remote_control.get_left_switch() == sp::DBusSwitchMode::MID) {
        sw_l_str = "MID";
      }

      uart_logger.log("sw_r: %s, sw_l: %s\r\n", sw_r_str, sw_l_str);
    }

    tick++;
    HAL_Delay(1);
  }
}