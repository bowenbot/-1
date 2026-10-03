#include "app.hpp"

extern "C" {
#include "main.h"
}

#include "io/buzzer_task.hpp"
#include "io/led_task.hpp"
#include "io/uart_logger.hpp"
#include "io/imu.hpp"
#include "io/remote_control.hpp"
#include "control/gimbal_controller.hpp"
#include "motor/gm6020_motor.hpp"

io::BuzzerTask buzzer_task;
io::LedTask led_task;
io::UartLogger uart_logger;
io::Imu imu;
io::RemoteControl remote_control;
control::GimbalController gimbal_controller(imu, remote_control);

extern "C" void app_main() {
  buzzer_task.init();
  led_task.init();
  uart_logger.init();
  imu.init();
  remote_control.init();
  gimbal_controller.init();
  motor::can_init();

  buzzer_task.beep(200);
  uart_logger.log("System boot OK\r\n");

  uint32_t tick = 0;

  while (1) {
    imu.update();
    gimbal_controller.update();
    motor::can_send_all();

    if (tick % 100 == 0) {
      led_task.flow_step();
    }

    if (tick % 500 == 0) {
      uart_logger.log("yaw: %.3f, pitch: %.3f, roll: %.3f\r\n",
                      imu.get_yaw_rad(), imu.get_pitch_rad(), imu.get_roll_rad());
    }

    tick++;
    HAL_Delay(1);
  }
}