# Automatic_Door_System
*Course:* Electronic Devices and Circuits (CE124.Q14)

*Institution:* University of Information Technology (UIT) - VNU-HCM

*Instructor:* Dr. Tran Quang Nguyen

## 📖 Overview

This project implements an automatic door system using the STM32F407VET6 microcontroller.
The system detects objects approaching the door using an HC-SR04 ultrasonic sensor and automatically opens or closes the door using a DC motor controlled by an L298N dual full-bridge motor driver.

This project demonstrates an embedded system application that integrates electronic device analysis, hardware design, circuit simulation, and firmware development.

## 🛠 Tech Stack & Methodology

* *Design Methodology:*
    * *Circuit Analysis:* Calculating current/voltage requirements for the Motor and Driver.
    * *Component Selection:* Choosing the H-Bridge driver based on power dissipation and peak current ratings.
* *Hardware Components:*
    * *Controller:* STM32F407VET6 (ARM Cortex-M4).
    * *Power Stage:* L298N Dual Full-Bridge Driver (Bipolar tech).
    * *Sensor:* HC-SR04 (Ultrasonic Transducer).
* *Tools:*
    * *Altium Designer / Proteus:* For circuit design and simulation.
    * *STM32CubeIDE:* For firmware implementation.

## ⚡ Component Specifications & Analysis

The system consists of three main stages with specific electrical parameters:

### 1. Control Stage (STM32F407VET6)
* *Operating Voltage:* $3.3V$ DC.
* *Clock Frequency:* $168 \text{ MHz}$.
* *Logic Level High ($V_{OH}$):* $3.3V$ (Sufficient to drive L298N Logic Inputs).
* *GPIO Mode:* Push-Pull Output.
* *GPIO Output Current:* Max $25 \text{ mA}$ (Sufficient to drive L298N Logic Inputs).

### 2. Power Driver Stage (L298N H-Bridge)
* *Device Type:* Dual Full-Bridge Driver (Transistor Logic).
* *Supply Voltage ($V_s$):* Supports up to $46V$ (We utilize $12V$).
* *Peak Output Current ($I_o$):* $2A$ per channel.
* *Logic High Voltage ($V_{IH}$):* $2.3V \le V_{in} \le V_{ss}$ (Compatible with STM32 3.3V logic).
* *Power Dissipation:* Heatsink required for currents $> 1A$.

### 3. Sensor Stage (HC-SR04)
* *Operating Voltage:* $5V$ DC.
* *Operating Current:* $15 \text{ mA}$.
* *Frequency:* $40 \text{ kHz}$.
* *Range:* $2 \text{ cm} - 400 \text{ cm}$.

## 📊 System Operation Logic

The circuit operates by switching the transistor states within the L298N H-Bridge to control current flow direction through the DC Motor.

| Condition | Sensor Distance ($d$) | MCU Signal (PB0 / PB1) | H-Bridge State | Motor Action |
| :--- | :--- | :--- | :--- | :--- |
| *User Approaching* | $d < 15 \text{ cm}$ | HIGH / LOW | Q1, Q4 ON | *Forward (Open)* |
| *Holding Open* | (Delay 3s) | LOW / LOW | All OFF | *Stop (Coast)* |
| *User Away* | $d \ge 15 \text{ cm}$ | LOW / HIGH | Q2, Q3 ON | *Reverse (Close)* |

## 💻 Firmware & Hardware Interface

Although this is a hardware-centric project, *Firmware* is essential to generate the correct *Electrical Control Signals*. The STM32 is programmed in *Embedded C* using the *HAL Library* to manage the state of the GPIO pins.

### Control Logic Implementation
The firmware implements a *Finite State Machine (FSM)* to control the voltage levels at pins PB0 and PB1, determining the conduction state of the transistors inside the L298N.

/* LOGIC TABLE FOR L298N CONTROL */
// PB0 = High (3.3V), PB1 = Low (0V)  -> Current flows A to B -> Motor Opens
// PB0 = Low (0V),  PB1 = High (3.3V) -> Current flows B to A -> Motor Closes

if (Distance < 15) // Sensor Input
{
    // Action: OPEN DOOR
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // PB0 -> 3.3V
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // PB1 -> 0V
    
    HAL_Delay(3000); // Hold state for 3 seconds
}
else
{
    // Action: CLOSE DOOR
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // PB0 -> 0V
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // PB1 -> 3.3V
}

## ⚙️ Installation & Setup

1.  *Clone the Repository:*
    
    git clone https://github.com/TienDat1703/Automatic_Door_System
    
2.  *Hardware Wiring:*
    * Connect *12V Source* to L298N $V_{CC}$ and *GND* to common ground.
    * Connect *STM32 PB0, PB1* to L298N *IN1, IN2*.
    * Connect *STM32 PA1, PA6* to Sensor *Trig, Echo*.
    * *Caution:* Ensure common GND between 12V and 5V/3.3V sources.
3.  *Flash Firmware:*
    * Load the .elf file using STM32CubeIDE or ST-Link Utility.

## 👥 Contributors & Roles

| Student Name | ID | Role | Responsibilities |
| :--- | :--- | :--- | :--- |
| *Le Hung Phat* | 23521139 | *System Arch* | System integration, Firmware logic, Schematic design. |
| *Nguyen Thanh Hieu* | 23520486 | *Power Electronics Eng.* | L298N & Motor analysis, Power calculations, Thermal analysis. |
| *Tran Quang Nhat* | 23521102 | *Control Circuit Eng.* | STM32 interface design, Simulation (Proteus), Wiring. |
| *Nguyen Dinh Nhat Nguyen* | 23521043 | *Sensor & Mech. Eng.* | HC-SR04 signal analysis, Mechanical build, Calibration. |
| *Ngo Tien Dat* | 23520254 | *QA & Technical Writer* | Testing & Measurements, Debugging |
---
Ho Chi Minh City, January 2026
Status: Completed
