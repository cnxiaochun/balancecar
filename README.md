智能平衡车
============


## 组件

- 铝合金底盘150mm * 83mm * 3mm
- 520电机(37mm), 带编码器
- STM32开发板(STM32F103C8T6)
- PS2遥控器
- 18650充电电池 * 3
- 65mm轮胎, 海绵内胆, 轮毂52mm, 轮宽27mm

## Features

|                |      支持          |
| -------------- | :----------------: |
| 四驱           |       __√__        |
| 2.4G遥控       |       __√__        |
| 3段速度        |       __√__        |
| PWM控制        |       __√__        |
| 飘移转弯       |       __√__        |
| 紧急停车       |       __√__        |
| 闭环控制       |       __√__        |

## 电机参数

|  项目          |      参数          |
| -------------- | :----------------: |
| 电机类型       | 永磁有刷           |
| 额定电压       | 12V                |
| 功率           | 4.8W               |
| 原始转速       | 11000rpm           |
| 减速比         | 1:30               |
| 堵转电流       | 2.8A               |
| 堵转扭矩       | 10Kgf.cm           |
| 额定电流       | 360mA              |
| 额定扭矩       | 1.0Kgf.cm          |
| 轴径           | 6mm D字型轴        |

接线方式:
    A    B    VCC    GND    M-    M+

## 霍尔编码器参数

|  项目          |      参数               |
| -------------- | :---------------------: |
| 类型           | AB相增量式              |
| 供电电压       | 5.0V                    |
| 线数           | 13                      |
| 线速           | 13 * 30 = 390           |
| 特色           | 自带上拉整形            |
| 接口类型       | PH2.0                   |


## TODO List

* ~~PS2遥控~~
* 超声波
* 红外寻迹
* 蓝牙
* 树莓派

## MPU管脚应用

| 管脚  | 功能      | 应用     | Remap |
| ----- | --------- | -------- | ----- |
| PA00  | GPIO      | LMOTOR A |       |           
| PA01  | GPIO      | LMOTOR B |       |           
| PA02  | TIM2 CH3  | PWM L    |       |           
| PA03  | TIM2 CH4  | PWM R    |       |           
| PA04  |           |          |       |           
| PA05  |           |          |       |           
| PA06  | TIM3 CH1  | Encoder  |       |           
| PA07  | TIM3 CH2  | Encoder  |       |           
| PA08  |           |          |       |           
| PA09  | USART1 TX |          |       |           
| PA10  | USART1 RX |          |       |           
| PA11  | USBDM     |          |       |           
| PA12  | USBDP     |          |       |           
| PA13  | SWD       | SWDIO    |       |           
| PA14  | SWD       | SWDCLK   |       |           
| PA15  | SPI1 NSS  | PS2X     | SPI1  |           
| PB00  | GPIO      | RMOTOR A |       |            
| PB01  | GPIO      | RMOTOR B |       |           
| PB02  | BOOT      | BOOT1    |       |
| PB03  | SPI1 SCK  | PS2X     | SPI1  |            
| PB04  | SPI1 MISO | PS2X     | SPI1  |           
| PB05  | SPI1 MOSI | PS2X     | SPI1  |
| PB06  | TIM4 CH1  | Encoder  |       |            
| PB07  | TIM4 CH2  | Encoder  |       |           
| PB08  | I2C1 SCL  | MPU6050  | I2C1  |
| PB09  | I2C1 SDA  | MPU6050  | I2C1  |
| PB10  | I2C2 SCL  | NRF24L01 |       |
| PB11  | I2C2 SDA  | NRF24L01 |       |
| PB12  | SPI2 NSS  | LED      |       |            
| PB13  | SPI2 SCK  |          |       |            
| PB14  | SPI2 MISO |          |       |           
| PB15  | SPI2 MOSI |          |       |


