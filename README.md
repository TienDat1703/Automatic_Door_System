# Automatic_Door_System
*Course:* Electronic Devices and Circuits (CE124.Q14)

*Institution:* University of Information Technology (UIT) - VNU-HCM

*Instructor:* Dr. Tran Quang Nguyen

## 📖 Overview

This project implements an automatic door system using the STM32F407VET6 microcontroller.
The system detects objects approaching the door using an HC-SR501 sensor and automatically opens or closes the door using a DC motor controlled by an L298N dual full-bridge motor driver.

This project demonstrates an embedded system application that integrates electronic device analysis, hardware design, circuit simulation, and firmware development.

## 🛠 Tech Stack & Methodology

* *Design Methodology:*
    * *Circuit Analysis:* Calculating current/voltage requirements for the Motor and Driver.
    * *Component Selection:* Choosing the H-Bridge driver based on power dissipation and peak current ratings.
* *Hardware Components:*
    * *Controller:* STM32F407VET6 (ARM Cortex-M4).
    * *Power Stage:* L298N Dual Full-Bridge Driver (Bipolar tech).
    * *Sensor:* HC-SR501 (Passive infrared sensor).
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

### 3. Sensor Stage (HC-SR501 PIR Sensor)
* *Operating Voltage:* $4.5V - 20V$ DC (Powered via $5V$ supply).
* *Quiescent Current:* $< 50 \ \mu\text{A}$ (Low power consumption).
* *Output Logic Level:* $3.3V$ High / $0V$ Low (Directly compatible with STM32 GPIO).
* *Detection Angle:* $< 100^\circ$ cone angle.
* *Detection Range:* $3\text{m} - 7\text{m}$ (Adjustable via onboard potentiometer).
* *Delay Time:* $0.3\text{s} - 5\text{min}$ (Adjustable via onboard potentiometer).
* *Operating Principle:* Passive Infrared (PIR) sensing detects thermal radiation changes from moving human bodies.
  

## 💻 Firmware & Hardware Interface

Although this is a hardware-centric project, *Firmware* is essential to generate the correct *Electrical Control Signals*. The STM32 is programmed in *Embedded C* using the *HAL Library* to manage the state of the GPIO pins.

### Control Logic Implementation
The firmware implements a **Non-blocking Finite State Machine (FSM)**. Instead of using blocking delays (`HAL_Delay`), it uses `HAL_GetTick()` to track time while continuously monitoring sensors and limit switches for maximum safety and responsiveness.

/* LOGIC OVERVIEW FOR MOTOR CONTROL (L298N) */
// IN1=HIGH, IN2=LOW  -> Motor Opens
// IN1=LOW,  IN2=HIGH -> Motor Closes
// IN1=LOW,  IN2=LOW  -> Motor Stops

if (PIR_Motion_Detected) 
{
    // Reset the 3-second hold timer
    last_motion_time = HAL_GetTick(); 
    
    // Safety Priority: Open door until Open Limit Switch (LS2) is hit
    if (!Limit_Switch_Open) Open_Door();
    else Stop_Motor();
} 
else 
{
    // Non-blocking 3-second hold delay check
    if (HAL_GetTick() - last_motion_time < 3000) 
    {
        if (!Limit_Switch_Open) Open_Door();
        else Stop_Motor();
    } 
    else 
    {
        // Close door after timeout until Close Limit Switch (LS1) is hit
        if (!Limit_Switch_Close) Close_Door();
        else Stop_Motor();
    }
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
