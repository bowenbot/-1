#include "io/uart_logger.hpp"

#include <cstdarg>
#include <cstdio>

extern "C" {
#include "usart.h"
}

namespace io {

void UartLogger::init() {
  // 串口已在 CubeMX 初始化，这里不用做额外操作
}

void UartLogger::log(const char *format, ...) {
  char buffer[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  printf("%s", buffer);
}

}  // namespace io

// 重定向 printf 到 USART1
extern "C" int fputc(int ch, FILE *f) {
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}