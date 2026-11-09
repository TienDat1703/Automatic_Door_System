# Automatic_Door_System
*Course:* Electronic Devices and Circuits (CE124.Q14)

*Institution:* University of Information Technology (UIT) - VNU-HCM

*Instructor:* Dr. Tran Quang Nguyen# Automatic_Door_System

*Course:* Electronic Devices and Circuits (CE124.Q14)

*Institution:* University of Information Technology (UIT) - VNU-HCM

*Instructor:* Dr. Tran Quang Nguyen

## 📖 Overview

This project implements an automatic door system using the STM32F407VET6 microcontroller.
The system detects people approaching the door using two HC-SR501 PIR (passive infrared) sensors and automatically opens or closes the door using a DC motor controlled by an L298N dual full-bridge motor driver. Two limit switches detect the fully closed and fully open positions and stop the motor at each end of travel, preventing door over-travel.

This project demonstrates an embedded system application that integrates electronic device analysis, hardware design, circuit simulation, and firmware development.

## 🛠 Tech Stack & Methodology

- *Design Methodology:*
  - *Circuit Analysis:* Calculating current/voltage requirements for the Motor and Driver.
  - *Component Selection:* Choosing the H-Bridge driver based on power dissipation and peak current ratings.
- *Hardware Components:*
  - *Controller:* STM32F407VET6 (ARM Cortex-M4).
  - *Power Stage:* L298N Dual Full-Bridge Driver (Bipolar tech).
  - *Sensors:* 2x HC-SR501 (Passive infrared sensor).
  - *Limit Switches:* 2x (LS1 = door fully closed, LS2 = door fully open).
- *Tools:*
  - *Altium Designer / Proteus:* For circuit design and simulation.
  - *STM32CubeIDE:* For firmware implementation.

## ⚡ Component Specifications & Analysis

The system consists of four main stages with specific electrical parameters:

### 1. Control Stage (STM32F407VET6)

- *Operating Voltage:* 3.3V DC.
- *Clock Frequency:* 168 MHz.
- *Logic Level High (V<sub>OH</sub>):* 3.3V (Sufficient to drive L298N Logic Inputs).
- *GPIO Mode:* Push-Pull Output (motor control), Input (sensors and limit switches).
- *GPIO Output Current:* Max 25 mA (Sufficient to drive L298N Logic Inputs).

### 2. Power Driver Stage (L298N H-Bridge)

- *Device Type:* Dual Full-Bridge Driver (Transistor Logic).
- *Supply Voltage (V<sub>s</sub>):* Supports up to 46V (We utilize 12V).
- *Peak Output Current (I<sub>o</sub>):* 2A per channel.
- *Logic High Voltage (V<sub>IH</sub>):* 2.3V ≤ V<sub>in</sub> ≤ V<sub>ss</sub> (Compatible with STM32 3.3V logic).
- *Power Dissipation:* Heatsink required for currents > 1A.

### 3. Sensor Stage (HC-SR501 PIR Sensor, x2)

- *Quantity:* 2 sensors (outputs on PA3 and PA4); motion detected by either sensor opens the door.
- *Operating Voltage:* 4.5V – 20V DC (Powered via 5V supply).
- *Quiescent Current:* < 50 μA (Low power consumption).
- *Output Logic Level:* 3.3V High / 0V Low (Directly compatible with STM32 GPIO).
- *Detection Angle:* < 100° cone angle.
- *Detection Range:* 3 m – 7 m (Adjustable via onboard potentiometer).
- *Delay Time:* 0.3 s – 5 min (Adjustable via onboard potentiometer).
- *Operating Principle:* Passive Infrared (PIR) sensing detects thermal radiation changes from moving human bodies.

### 4. Limit Switch Stage (x2)

- *Quantity:* 2 switches: LS1 (door fully closed, PA1) and LS2 (door fully open, PA2).
- *Input Mode:* GPIO input with internal pull-up (no external resistor required).
- *Active Level:* LOW when pressed (the switch connects the pin to GND), HIGH otherwise.
- *Function:* Stops the motor at each end of travel (LS2 stops opening, LS1 stops closing) to prevent door over-travel.

## 💻 Firmware & Hardware Interface

Although this is a hardware-centric project, *Firmware* is essential to generate the correct *Electrical Control Signals*. The STM32 is programmed in *Embedded C* using the *HAL Library* to manage the state of the GPIO pins.

### Control Logic Implementation

The firmware implements a *Non-blocking Finite State Machine (FSM)*. Instead of using blocking delays (`HAL_Delay`), it uses `HAL_GetTick()` to track time while continuously monitoring sensors and limit switches for maximum safety and responsiveness.

```c
/* LOGIC OVERVIEW FOR MOTOR CONTROL (L298N) */
// IN1=HIGH, IN2=LOW  -> Motor Opens
// IN1=LOW,  IN2=HIGH -> Motor Closes
// IN1=LOW,  IN2=LOW  -> Motor Stops

if (PIR_Motion_Detected) {
    // Reset the 3-second hold timer
    last_motion_time = HAL_GetTick();

    // Safety Priority: Open door until Open Limit Switch (LS2) is hit
    if (!Limit_Switch_Open) Open_Door();
    else Stop_Motor();
} else {
    // Non-blocking 3-second hold delay check
    if (HAL_GetTick() - last_motion_time < 3000) {
        if (!Limit_Switch_Open) Open_Door();
        else Stop_Motor();
    } else {
        // Close door after timeout until Close Limit Switch (LS1) is hit
        if (!Limit_Switch_Close) Close_Door();
        else Stop_Motor();
    }
}
```

## ⚙️ Installation & Setup

1. *Clone the Repository:*

    ```bash
    git clone https://github.com/TienDat1703/Automatic_Door_System
    ```

2. *Hardware Wiring:*

    *Signal connections:*

    | STM32 Pin | Connected To | Description |
    | --------- | ------------ | ----------- |
    | PB0 | L298N IN1 | HIGH = motor opens |
    | PB1 | L298N IN2 | HIGH = motor closes |
    | PA3 | PIR sensor #1 OUT | Motion input (3.3V logic) |
    | PA4 | PIR sensor #2 OUT | Motion input (3.3V logic) |
    | PA1 | Limit switch LS1 (fully closed) | Input, internal pull-up, active-low |
    | PA2 | Limit switch LS2 (fully open) | Input, internal pull-up, active-low |
    | PA6 | Status LED (optional) | Lights up while motion is detected |

    *Power and other connections:*

    - Connect *12V Source* to L298N *V<sub>CC</sub>* and *GND* to common ground.
    - Connect L298N *OUT1, OUT2* to the DC motor.
    - Connect *5V supply* to the PIR sensors' VCC.
    - Connect one terminal of each limit switch to its STM32 pin (PA1 / PA2) and the other terminal to *GND*. The internal pull-up keeps the pin HIGH until the switch is pressed.
    - *Caution:* Ensure common GND between the 12V source, the 5V supply, and the STM32.

3. *Flash Firmware:*

    - Load the .elf file using STM32CubeIDE or ST-Link Utility.

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


Ho Chi Minh City, January 2026
Status: Completed
