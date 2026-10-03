#ifndef LED_TASK_HPP
#define LED_TASK_HPP

#include <cstdint>

namespace io {

class LedTask {
 public:
  void init();
  void flow_step();
  void all_off();

 private:
  uint8_t led_index_{0};
};

}  // namespace io

#endif  // LED_TASK_HPP