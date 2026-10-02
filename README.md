# SuperPower 校内赛 嵌入式代码

## 一、主要任务
本仓库为同济大学 SuperPower 战队 2027 赛季校内赛嵌入式方向代码。
基于 RoboMaster C 型开发板，完成以下任务：

1. C 板基本功能：蜂鸣器控制、LED 流水灯、串口打印 IMU 三轴、DT7 遥控器通讯。
2. 姿态与电机联动：右拨杆下档失能、中档姿态联动、上档复位；
   左拨杆切换 B 电机比例（下档 0.5、中档 -1、上档 3）。
3. 自动折叠决策：使用红外测距模块测量隧道距离，自动折叠云台通过净高 400mm 隧道。
4. 24V 转 5V 降压模块：基于 MP4420A 设计原理图与 PCB。

## 二、硬件平台
- 主控：STM32F407IGH6（RoboMaster C 型开发板）
- IMU：BMI088（SPI1）
- 电机：RM6020 × 2 + C610 电调（CAN1）
- 遥控器：DT7（DBUS / UART3）
- 降压模块：MP4420A（24V 转 5V，自制）

## 三、软件架构
- 框架：STM32 HAL 库 + FreeRTOS（CMSIS_V1）
- 构建：CMake + ARM GCC
- 语言：C++（业务逻辑）+ C（HAL 底层）
- 编辑器：VSCode + CMake Tools + Cortex-Debug

## 四、目录结构
- `Core/` — CubeMX 生成的初始化代码
- `Drivers/` — ST HAL 库
- `Middlewares/sp_middleware/` — 同济 SuperPower 中间件（submodule）
- `include/` — 自写头文件
- `src/` — 自写源文件
- `app/` — 业务入口 `app_main()`
- `CMakeLists.txt` — 构建配置

## 五、操作注意事项
1. 右拨杆下档为失能模式，调试前必须验证全部电机无力。
2. 电机测试必须轮子离地或机构架空，电流限幅由小到大。
3. 24V 上电必须限流，首次通电前检查电源极性、线束绝缘、短路风险。
4. 烧录、接线、拆装前必须切断动力电源。
5. 出现异味、冒烟、失控、线束拉扯时立即断电。

## 六、第三方代码来源、许可证与修改内容

### 1. sp_middleware
- 来源：https://github.com/TongjiSuperPower/sp_middleware
- 用途：BMI088 驱动、CAN 通信、DBUS 解析、RM 电机控制、PID、数学工具等。
- 许可证：以原仓库声明为准。
- 修改内容：未修改源码，仅通过 CMake 选择性编译所需模块（bmi088、buzzer、can、dbus、led、rm_motor、crc、math_tools、pid、low_pass_filter、timer）。

### 2. FreeRTOS
- 来源：https://www.freertos.org/
- 用途：任务调度，为 CMSIS-RTOS 提供支持。
- 许可证：MIT License。
- 修改内容：未修改，由 CubeMX 自动集成。

### 3. CMSIS-RTOS
- 来源：ARM 官方，随 STM32CubeF4 固件包提供。
- 用途：FreeRTOS 的 CMSIS 封装接口。
- 许可证：Apache-2.0。
- 修改内容：未修改。

## 七、版本记录
- V1.0.0（初始版本）：CubeMX 工程、FreeRTOS、sp_middleware submodule、蜂鸣器模块。
（后续根据实际提交更新）

## 八、Git 提交规范
- 前缀：`feat` / `fix` / `docs` / `chore` / `build` / `refactor`
- 每完成一个模块提交一次，提交历史反映开发过程。
- 重要功能保留可回退的稳定版本。