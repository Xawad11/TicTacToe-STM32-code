# STM32 Tic-Tac-Toe

A two-player **Tic-Tac-Toe game** implemented using an **STM32F103C8T6 (Blue Pill)** microcontroller.  
Player input is taken through a **4×4 matrix keypad**, and the game state is displayed using a **3×3 LED grid**.

---

## Hardware Used
- STM32F103C8T6 (Blue Pill)
- 4×4 Matrix Keypad
- 9 LEDs (3×3 grid)
- Breadboard & jumper wires
- ST-Link programmer

---

## How It Works
- Players press keypad keys **1–9** to select board positions
- Player 1 → solid LED  
- Player 2 → blinking LED  
- Win and draw conditions are checked automatically
- Winning triggers a celebration LED pattern
- Press `*` on the keypad to reset the game

---

## Tools
- STM32CubeIDE
- STM32 HAL Library
- ST-Link

---

## Project Type
Embedded systems project for **CSE 331L (Fall 2025)**.
