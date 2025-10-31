# 基于weAct_750vb开发板构建zephyr的demo
[来源](https://github.com/wuhanstudio/stm32l475-zephyr-bsp)

## Demo 测试项
|文件|结果|
| -- | -- |
|prj-learn|pass|
|test_lvgl|fail|
|test_qspi_flash|pass|
|test_sdcard_fs|pass|
|test_st7735|pass|
|test_usb_cdc|pass|

## 注意事项
- 烧录程序时，jilnk的RST脚必须与实际的MCU NRST引脚相连接，否则会导致识别不到设备
 

