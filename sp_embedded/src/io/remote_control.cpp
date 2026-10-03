#include "io/remote_control.hpp"

extern "C" {
#include "main.h"
#include "usart.h"
}

namespace {

sp::DBus g_dbus(&huart3, false);

}  // namespace

namespace io {

void RemoteControl::init() {
  g_dbus.request();
}

sp::DBusSwitchMode RemoteControl::get_right_switch() const {
  return g_dbus.sw_r;
}

sp::DBusSwitchMode RemoteControl::get_left_switch() const {
  return g_dbus.sw_l;
}

float RemoteControl::get_right_horizontal() const {
  return g_dbus.ch_rh;
}

float RemoteControl::get_right_vertical() const {
  return g_dbus.ch_rv;
}

float RemoteControl::get_left_horizontal() const {
  return g_dbus.ch_lh;
}

float RemoteControl::get_left_vertical() const {
  return g_dbus.ch_lv;
}

float RemoteControl::get_left_wheel() const {
  return g_dbus.ch_lu;
}

bool RemoteControl::is_connected() const {
  return g_dbus.is_alive(HAL_GetTick());
}

}  // namespace io

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART3) {
    g_dbus.update(sp::DBUS_BUFF_SIZE, HAL_GetTick());
    g_dbus.request();
  }
}