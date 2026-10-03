#ifndef UART_LOGGER_HPP
#define UART_LOGGER_HPP

namespace io {

class UartLogger {
 public:
  void init();
  void log(const char *format, ...);
};

}  // namespace io

#endif  // UART_LOGGER_HPP