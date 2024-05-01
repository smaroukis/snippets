// Bare metal template for STM32F303xE based projects
// See Readme for details


#include <stdint.h>

// FPU warning, set compiler settings in Makefile
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// User code to blink LD2 on PA5 for Nucleo-F303RE board
#define USER_DELAY 1000

// Define GPIO and RCC registers
#define GPIOA_BASE_ADDR 0x48000000U
// GPIOA -> AHB2 
#define RCC_BASE_ADDR 0x40021000U
#define RCC_AHBENR (*((volatile uint32_t*)(RCC_BASE_ADDR + 0x14)))
#define GPIO_MODER (*((volatile uint32_t*)(GPIOA_BASE_ADDR + 0x00)))
#define GPIO_OTYPER (*((volatile uint32_t*)(GPIOA_BASE_ADDR + 0x04)))
#define GPIOA_ODR  (*((volatile uint32_t*)(GPIOA_BASE_ADDR + 0x14)))

void delay(int count);

int main(void)
{
  // Enable Clock to GPIOA
  RCC_AHBENR |= (1 << 17);

  // Set PA5 as Output
  GPIO_MODER &= ~(0b11 << 5 * 2);
  GPIO_MODER |= (0b01 << 5 * 2 );

  /* Loop forever */
  while(1) {
    GPIOA_ODR ^= (1 << 5);
    delay(USER_DELAY);
  }
}


void delay(int count) {
  while (count--) {
    volatile int i = 1000;
    while (i--) {
      __asm volatile ("NOP"); // do nothing
    }
  }
}
