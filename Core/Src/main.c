/* USER CODE END Header */
#include "main.h"
/* Function Call */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
uint8_t CheckWinner(void);
void ResetGame(void);
void UpdateLEDs(void);
void CelebrationPattern(uint8_t winner);
uint8_t ReadKeypad(void);
/* Game state variables */
uint8_t board[9] = { 0 };      // 0=empty, 1=Player1(X), 2=Player2(O)
uint8_t currentPlayer = 1;    // 1 or 2
uint8_t gameOver = 0;         // 0=playing, 1=won, 2=draw
uint8_t winner = 0;           // 0=none, 1=Player1, 2=Player2
uint32_t blinkCounter = 0;    // For blinking Player 2 LEDs
uint8_t lastKey = 0;
/* LED pin mapping [position][port][pin] */
const uint16_t LED_PINS[9] = {
GPIO_PIN_0,  // Position 1 - PA0
		GPIO_PIN_1,  // Position 2 - PA1
		GPIO_PIN_2,  // Position 3 - PA2
		GPIO_PIN_0,  // Position 4 - PB0
		GPIO_PIN_1,  // Position 5 - PB1
		GPIO_PIN_12, // Position 6 - PB12
		GPIO_PIN_13, // Position 7 - PB13
		GPIO_PIN_14, // Position 8 - PB14
		GPIO_PIN_15  // Position 9 - PB15
		};
const GPIO_TypeDef *LED_PORTS[9] = {
GPIOA, GPIOA, GPIOA,  // Positions 1-3
		GPIOB, GPIOB, GPIOB,  // Positions 4-6
		GPIOB, GPIOB, GPIOB   // Positions 7-9
		};
int main(void) {
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	while (1) {
		uint8_t key = ReadKeypad();
		// Handle key press
		if (key != 0 && key != lastKey) {
			if (key == 10) {  // * key = reset
				ResetGame();
				HAL_Delay(300);
			} else if (key >= 1 && key <= 9 && !gameOver) {
				uint8_t pos = key - 1;
				// Place mark if position is empty
				if (board[pos] == 0) {
					board[pos] = currentPlayer;
					// Check for winner,
					winner = CheckWinner();
					if (winner > 0) {
						gameOver = 1; // someone won =player1/player2
						CelebrationPattern(winner);
					} else {
						// Check for draw
						uint8_t full = 1;
						for (int i = 0; i < 9; i++) {
							if (board[i] == 0) {
								full = 0;
								break;
							}
						}
						if (full) {
							gameOver = 2; // Draw
						} else {
							// Switch player
							currentPlayer = (currentPlayer == 1) ? 2 : 1;
						}
					}
				}
				HAL_Delay(200);
			}
		}
		lastKey = key;
		// Update LED display
		blinkCounter++;
		UpdateLEDs();
		HAL_Delay(10);
	}
}
/* Read keypad and return key number (1-9, 10=*, 11=#, 0=none) */
uint8_t ReadKeypad(void) {
	// Row 0: 1, 2, 3, A
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(1);
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		return 1;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		return 2;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		return 3;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	// Row 1: 4, 5, 6, B
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(1);
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		return 4;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		return 5;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		return 6;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	// Row 2: 7, 8, 9, C
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_Delay(1);
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		return 7;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		return 8;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		return 9;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	// Row 3: *, 0, #, D
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(1);
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		return 10;  // * key for reset
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		return 0;
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		return 11;  // # key
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	return 0; // No key pressed
}
/* Update LED states based on board */
void UpdateLEDs(void) {
	for (int i = 0; i < 9; i++) {
		if (board[i] == 1) {
			// Player 1 (X): Solid ON
			HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
					GPIO_PIN_SET);
		} else if (board[i] == 2) {
			// Player 2 (O): Blinking (500ms cycle)
			if ((blinkCounter / 25) % 2 == 0) {
				HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
						GPIO_PIN_SET);
			} else {
				HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
						GPIO_PIN_RESET);
			}
		} else {
			// Empty: OFF
			HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
					GPIO_PIN_RESET);
		}
	}
}
/* Check for winner - returns 0=none, 1=Player1, 2=Player2 */
uint8_t CheckWinner(void) {
	// Win patterns: rows, columns, diagonals
	const uint8_t wins[8][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 },  // Rows
			{ 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 },  // Columns
			{ 0, 4, 8 }, { 2, 4, 6 }            // Diagonals
	};
	for (int i = 0; i < 8; i++) {
		uint8_t a = board[wins[i][0]];
		uint8_t b = board[wins[i][1]];
		uint8_t c = board[wins[i][2]];
		if (a != 0 && a == b && b == c) {
			return a; // Winner found
		}
	}
	return 0; // No winner
}
/* Celebration pattern when someone wins */
void CelebrationPattern(uint8_t winner) {
	// Flash all LEDs 3 times
	for (int flash = 0; flash < 3; flash++) {
		// All ON
		for (int i = 0; i < 9; i++) {
			HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
					GPIO_PIN_SET);
		}
		HAL_Delay(200);
		// All OFF
		for (int i = 0; i < 9; i++) {
			HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
					GPIO_PIN_RESET);
		}
		HAL_Delay(200);
	}

	HAL_Delay(500);
}
/* Reset game to initial state */
void ResetGame(void) {
	for (int i = 0; i < 9; i++) {
		board[i] = 0;
		HAL_GPIO_WritePin((GPIO_TypeDef*) LED_PORTS[i], LED_PINS[i],
				GPIO_PIN_RESET);
	}
	currentPlayer = 1;
	gameOver = 0;
	winner = 0;
	blinkCounter = 0;
}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12 |
	GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_12 |
	GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

void Error_Handler(void) {
	__disable_irq();
	while (1) {
	}
}
