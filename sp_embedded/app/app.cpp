#include "app.hpp"

#include "io/buzzer_task.hpp"

io::BuzzerTask buzzer_task;

extern "C" void app_main() {
  buzzer_task.init();
  buzzer_task.beep(200);

  while (1) {
  }
}