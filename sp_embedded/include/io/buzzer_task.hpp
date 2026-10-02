#ifndef BUZZER_TASK_HPP
#define BUZZER_TASK_HPP

#include <cstdint>

namespace io {

class BuzzerTask {
 public:
  void init();
  void beep(uint32_t duration_ms);
  void stop();

 private:
  uint32_t duration_ms_{0};
};

}  // namespace io

#endif  // BUZZER_TASK_HPP