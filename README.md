# Environment Monitoring 

## Overview
This repository contains project to monitors environment (temperature and pressure), readings are in degree C and PSI respectively. Serial communication provides communication between desktop and microcontroller. Project “” provides desktop UI to communicate to microcontroller via UART, offers control and graphs to display data. 

Project incorporates modular approach, where each component is within its own directory, this includes components like bmp218 driver and thread’s main functions.

Environment monitoring components: Core/EnvCore/ 


* IDE: STM32CubeIDE 1.10.1
* STM32 MCU: STM32F746NGHx (ARM Cortex-M7) 
* RTOS: AZURE RTOS – ThreadX 6.1.8
* Sensor: BMP218, controlled via I2C-1. GPIOs scl: PB8 and sda: PB9.
* Communication: UASRT-1, GPIOs tx: PA9 and rx: PB7

UART settings:
* Baud Rate: 230400 Bits/sec
* Word Length: 8 Bits
* Parity: None
* Stop Bits: 1

