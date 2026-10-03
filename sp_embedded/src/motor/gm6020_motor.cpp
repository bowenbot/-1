#include "motor/gm6020_motor.hpp"

extern "C" {
#include "main.h"
#include "can.h"
}

namespace motor {

namespace {

constexpr uint8_t MAX_MOTORS = 8;
Gm6020Motor* g_motors[MAX_MOTORS] = {nullptr};
uint8_t g_motor_count = 0;

constexpr uint16_t GM6020_FEEDBACK_BASE_ID = 0x205;
constexpr uint16_t GM6020_CMD_ID_1_TO_4 = 0x1FF;
constexpr uint16_t GM6020_CMD_ID_5_TO_7 = 0x2FF;

void can_send_frame(uint16_t std_id, uint8_t* data) {
  CAN_TxHeaderTypeDef tx_header = {0};
  tx_header.StdId = std_id;
  tx_header.IDE = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.DLC = 8;
  tx_header.TransmitGlobalTime = DISABLE;

  uint32_t mailbox = 0;
  if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0) {
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, data, &mailbox);
  }
}

}  // namespace

Gm6020Motor::Gm6020Motor(uint8_t motor_id)
    : rm_motor_(motor_id, sp::RM_Motors::GM6020, 1.0f, true) {
  if (g_motor_count < MAX_MOTORS) {
    g_motors[g_motor_count++] = this;
  }
}

void Gm6020Motor::set_current(float current) {
  this->rm_motor_.cmd(current);
}

float Gm6020Motor::get_angle_rad() const {
  return this->rm_motor_.angle;
}

float Gm6020Motor::get_speed_rad_s() const {
  return this->rm_motor_.speed;
}

float Gm6020Motor::get_torque_nm() const {
  return this->rm_motor_.torque;
}

uint8_t Gm6020Motor::get_temperature() const {
  return this->rm_motor_.temperature;
}

uint8_t Gm6020Motor::motor_id() const {
  return static_cast<uint8_t>(this->rm_motor_.rx_id - GM6020_FEEDBACK_BASE_ID);
}

sp::RM_Motor& Gm6020Motor::rm_motor() {
  return this->rm_motor_;
}

void can_init() {
  CAN_FilterTypeDef filter = {0};
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterIdHigh = 0x0000;
  filter.FilterIdLow = 0x0000;
  filter.FilterMaskIdHigh = 0x0000;
  filter.FilterMaskIdLow = 0x0000;
  filter.FilterFIFOAssignment = CAN_RX_FIFO0;
  filter.FilterActivation = ENABLE;
  filter.SlaveStartFilterBank = 14;

  HAL_CAN_ConfigFilter(&hcan1, &filter);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void can_send_all() {
  uint8_t data_1ff[8] = {0};
  uint8_t data_2ff[8] = {0};
  bool has_1ff = false;
  bool has_2ff = false;

  for (uint8_t i = 0; i < g_motor_count; i++) {
    Gm6020Motor* m = g_motors[i];
    uint8_t id = m->motor_id();

    if (id >= 1 && id <= 4) {
      m->rm_motor().write(data_1ff);
      has_1ff = true;
    } else if (id >= 5 && id <= 7) {
      m->rm_motor().write(data_2ff);
      has_2ff = true;
    }
  }

  if (has_1ff) {
    can_send_frame(GM6020_CMD_ID_1_TO_4, data_1ff);
  }
  if (has_2ff) {
    can_send_frame(GM6020_CMD_ID_5_TO_7, data_2ff);
  }
}

void dispatch_can_rx(uint32_t std_id, uint8_t* data, uint32_t stamp_ms) {
  for (uint8_t i = 0; i < g_motor_count; i++) {
    Gm6020Motor* m = g_motors[i];
    if (m->rm_motor().rx_id == std_id) {
      m->rm_motor().read(data, stamp_ms);
      return;
    }
  }
}

}  // namespace motor

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
  if (hcan->Instance == CAN1) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, data);
    motor::dispatch_can_rx(rx_header.StdId, data, HAL_GetTick());
  }
}