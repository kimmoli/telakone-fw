/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Kimmoli's telakone STM32F407ZG board.
 */

/*
 * Board identifier.
 */
#define BOARD_KIMMOLI_TELAKONE
#define BOARD_NAME                  "Kimmoli's telakone STM32F407ZG"

/*
 * Use watchdog
 */

#define TK_USE_WDOG

/*
 * Simplelink CC3100 Firmware update
 * Uncomment this and update simplelink/hostprog headers
 * build, and run wifi prog command
 */
/*
#define TK_CC3100_PROGRAMMING
*/

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                25000000U
#endif

//#define STM32_HSE_BYPASS

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * Board specific analog stuff
 */
#define EXT_VREF                   ( (float)(2.500) )
#define ADC_MEAS12V_SCALE          ( (float)(EXT_VREF/4096*(5900+825)/825) )
#define ADC_MEAS48V_SCALE          ( (float)(EXT_VREF/4096*(19100+825)/825) )
#define ADC_MOTORCURR_SCALE        ( (float)(EXT_VREF/(4096*(0.01*(10+499)/10))) )

/*
 * IO pins assignments.
 */
#define GPIOA_PA0_MEAS12V           0U
#define GPIOA_PA1_NC                1U
#define GPIOA_PA2_CCHIBL            2U
#define GPIOA_PA3_CCSPICSL          3U
#define GPIOA_PA4_CCIRQ             4U
#define GPIOA_PA5_CCSPICLK          5U
#define GPIOA_PA6_CCSPIMISO         6U
#define GPIOA_PA7_CCSPIMOSI         7U
#define GPIOA_PA8_NC_MCO1           8U
#define GPIOA_PA9_USART1TX          9U
#define GPIOA_PA10_USART1RX         10U
#define GPIOA_PA11_USART1CTS        11U
#define GPIOA_PA12_USART1RTS        12U
#define GPIOA_PA13_SWDIO            13U
#define GPIOA_PA14_SWCLK            14U
#define GPIOA_PA15_NC               15U

#define GPIOB_PB0_NC                0U
#define GPIOB_PB1_NC                1U
#define GPIOB_PB2_BOOT1             2U
#define GPIOB_PB3_NC                3U
#define GPIOB_PB4_NC                4U
#define GPIOB_PB5_NC                5U
#define GPIOB_PB6_I2C1SCL           6U
#define GPIOB_PB7_I2C1SDA           7U
#define GPIOB_PB8_REDLED            8U
#define GPIOB_PB9_GREENLED          9U
#define GPIOB_PB10_D1BRAKE          10U
#define GPIOB_PB11_D1REVERSE        11U
#define GPIOB_PB12_D1SPICSL         12U
#define GPIOB_PB13_D1SPICLK         13U
#define GPIOB_PB14_D1SPIMISO        14U
#define GPIOB_PB15_D1SPIMOSI        15U

#define GPIOC_PC0_NC                0U
#define GPIOC_PC1_NC                1U
#define GPIOC_PC2_NC                2U
#define GPIOC_PC3_NC                3U
#define GPIOC_PC4_NC                4U
#define GPIOC_PC5_NC                5U
#define GPIOC_PC6_NC                6U
#define GPIOC_PC7_D2BRAKE           7U
#define GPIOC_PC8_D2REVERSE         8U
#define GPIOC_PC9_D2SPICSL          9U
#define GPIOC_PC10_D2SPICLK         10U
#define GPIOC_PC11_D2SPIMISO        11U
#define GPIOC_PC12_D2SPIMOSI        12U
#define GPIOC_PC13_NC               13U
#define GPIOC_PC14_NC               14U
#define GPIOC_PC15_NC               15U

#define GPIOD_PD0_NC                0U
#define GPIOD_PD1_NC                1U
#define GPIOD_PD2_NC                2U
#define GPIOD_PD3_NC                3U
#define GPIOD_PD4_ACCLINKTXE        4U
#define GPIOD_PD5_ACCLINKTX         5U
#define GPIOD_PD6_ACCLINKRX         6U
#define GPIOD_PD7_NC                7U
#define GPIOD_PD8_USART3TX          8U
#define GPIOD_PD9_USART3RX          9U
#define GPIOD_PD10_NC               10U
#define GPIOD_PD11_NC               11U
#define GPIOD_PD12_SPEED1           12U
#define GPIOD_PD13_SPEED2           13U
#define GPIOD_PD14_NC               14U
#define GPIOD_PD15_NC               15U

#define GPIOE_PE0_NC                0U
#define GPIOE_PE1_NC                1U
#define GPIOE_PE2_NC                2U
#define GPIOE_PE3_MOTORL1           3U
#define GPIOE_PE4_MOTORL2           4U
#define GPIOE_PE5_MOTORH1           5U
#define GPIOE_PE6_MOTORH2           6U
#define GPIOE_PE7_NC                7U
#define GPIOE_PE8_NC                8U
#define GPIOE_PE9_NC                9U
#define GPIOE_PE10_NC               10U
#define GPIOE_PE11_NC               11U
#define GPIOE_PE12_NC               12U
#define GPIOE_PE13_NC               13U
#define GPIOE_PE14_NC               14U
#define GPIOE_PE15_NC               15U

#define GPIOF_PF0_NC                0U
#define GPIOF_PF1_BUTTON1           1U
#define GPIOF_PF2_BUTTON2           2U
#define GPIOF_PF3_MOTORCURR         3U
#define GPIOF_PF4_POT1              4U
#define GPIOF_PF5_POT2              5U
#define GPIOF_PF6_OUT1              6U
#define GPIOF_PF7_OUT2              7U
#define GPIOF_PF8_OUT3              8U
#define GPIOF_PF9_OUT4              9U
#define GPIOF_PF10_NC               10U
#define GPIOF_PF11_NC               11U
#define GPIOF_PF12_NC               12U
#define GPIOF_PF13_NC               13U
#define GPIOF_PF14_NC               14U
#define GPIOF_PF15_NC               15U

#define GPIOG_PG0_NC                0U
#define GPIOG_PG1_NC                1U
#define GPIOG_PG2_NC                2U
#define GPIOG_PG3_NC                3U
#define GPIOG_PG4_NC                4U
#define GPIOG_PG5_NC                5U
#define GPIOG_PG6_NC                6U
#define GPIOG_PG7_NC                7U
#define GPIOG_PG8_NC                8U
#define GPIOG_PG9_NC                9U
#define GPIOG_PG10_NC               10U
#define GPIOG_PG11_NC               11U
#define GPIOG_PG12_NC               12U
#define GPIOG_PG13_NC               13U
#define GPIOG_PG14_NC               14U
#define GPIOG_PG15_NC               15U

#define GPIOH_PH0_OSC_IN            0U
#define GPIOH_PH1_OSC_OUT           1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

#define GPIOJ_PIN0                  0U
#define GPIOJ_PIN1                  1U
#define GPIOJ_PIN2                  2U
#define GPIOJ_PIN3                  3U
#define GPIOJ_PIN4                  4U
#define GPIOJ_PIN5                  5U
#define GPIOJ_PIN6                  6U
#define GPIOJ_PIN7                  7U
#define GPIOJ_PIN8                  8U
#define GPIOJ_PIN9                  9U
#define GPIOJ_PIN10                 10U
#define GPIOJ_PIN11                 11U
#define GPIOJ_PIN12                 12U
#define GPIOJ_PIN13                 13U
#define GPIOJ_PIN14                 14U
#define GPIOJ_PIN15                 15U

#define GPIOK_PIN0                  0U
#define GPIOK_PIN1                  1U
#define GPIOK_PIN2                  2U
#define GPIOK_PIN3                  3U
#define GPIOK_PIN4                  4U
#define GPIOK_PIN5                  5U
#define GPIOK_PIN6                  6U
#define GPIOK_PIN7                  7U
#define GPIOK_PIN8                  8U
#define GPIOK_PIN9                  9U
#define GPIOK_PIN10                 10U
#define GPIOK_PIN11                 11U
#define GPIOK_PIN12                 12U
#define GPIOK_PIN13                 13U
#define GPIOK_PIN14                 14U
#define GPIOK_PIN15                 15U

/*
 * IO lines assignments.
 */

#define LINE_MEAS12V                PAL_LINE(GPIOA, 0U)
#define LINE_CCHIBL                 PAL_LINE(GPIOA, 2U)
#define LINE_CCSPICSL               PAL_LINE(GPIOA, 3U)
#define LINE_CCIRQ                  PAL_LINE(GPIOA, 4U)
#define LINE_CCSPICLK               PAL_LINE(GPIOA, 5U)
#define LINE_CCSPIMISO              PAL_LINE(GPIOA, 6U)
#define LINE_CCSPIMOSI              PAL_LINE(GPIOA, 7U)
#define LINE_USART1TX               PAL_LINE(GPIOA, 9U)
#define LINE_USARTRX                PAL_LINE(GPIOA, 10U)
#define LINE_USART1CTS              PAL_LINE(GPIOA, 11U)
#define LINE_USART1RTS              PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_BOOT1                  PAL_LINE(GPIOB, 2U)
#define LINE_I2C1SCL                PAL_LINE(GPIOB, 6U)
#define LINE_I2C1SDA                PAL_LINE(GPIOB, 7U)
#define LINE_REDLED                 PAL_LINE(GPIOB, 8U)
#define LINE_GREENLED               PAL_LINE(GPIOB, 9U)
#define LINE_D1BRAKE                PAL_LINE(GPIOB, 10U)
#define LINE_D1REVERSE              PAL_LINE(GPIOB, 11U)
#define LINE_D1SPICSL               PAL_LINE(GPIOB, 12U)
#define LINE_D1SPICLK               PAL_LINE(GPIOB, 13U)
#define LINE_D1SPIMISO              PAL_LINE(GPIOB, 14U)
#define LINE_D1SPIMOSI              PAL_LINE(GPIOB, 15U)
#define LINE_D2BRAKE                PAL_LINE(GPIOC, 7U)
#define LINE_D2REVERSE              PAL_LINE(GPIOC, 8U)
#define LINE_D2SPICSL               PAL_LINE(GPIOC, 9U)
#define LINE_D2SPICLK               PAL_LINE(GPIOC, 10U)
#define LINE_D2SPIMISO              PAL_LINE(GPIOC, 11U)
#define LINE_D2SPIMOSI              PAL_LINE(GPIOC, 12U)
#define LINE_ACCLINKTXE             PAL_LINE(GPIOD, 4U)
#define LINE_ACCLINKTX              PAL_LINE(GPIOD, 5U)
#define LINE_ACCLINKRX              PAL_LINE(GPIOD, 6U)
#define LINE_USART3TX               PAL_LINE(GPIOD, 8U)
#define LINE_USART3RX               PAL_LINE(GPIOD, 9U)
#define LINE_SPEED1                 PAL_LINE(GPIOD, 12U)
#define LINE_SPEED2                 PAL_LINE(GPIOD, 13U)
#define LINE_MOTORL1                PAL_LINE(GPIOE, 3U)
#define LINE_MOTORL2                PAL_LINE(GPIOE, 4U)
#define LINE_MOTORH1                PAL_LINE(GPIOE, 5U)
#define LINE_MOTORH2                PAL_LINE(GPIOE, 6U)
#define LINE_BUTTON1                PAL_LINE(GPIOF, 1U)
#define LINE_BUTTON2                PAL_LINE(GPIOF, 2U)
#define LINE_MOTORCURR              PAL_LINE(GPIOF, 3U)
#define LINE_POT1                   PAL_LINE(GPIOF, 4U)
#define LINE_POT2                   PAL_LINE(GPIOF, 5U)
#define LINE_OUT1                   PAL_LINE(GPIOF, 6U)
#define LINE_OUT2                   PAL_LINE(GPIOF, 7U)
#define LINE_OUT3                   PAL_LINE(GPIOF, 8U)
#define LINE_OUT4                   PAL_LINE(GPIOF, 9U)

#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))



#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_PA0_MEAS12V) /* ADC3 IN0 */| \
                                     PIN_MODE_INPUT(GPIOA_PA1_NC) | \
                                     PIN_MODE_OUTPUT(GPIOA_PA2_CCHIBL) | \
                                     PIN_MODE_OUTPUT(GPIOA_PA3_CCSPICSL) | \
                                     PIN_MODE_INPUT(GPIOA_PA4_CCIRQ) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA5_CCSPICLK) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA6_CCSPIMISO) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA7_CCSPIMOSI) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA8_NC_MCO1) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA9_USART1TX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA10_USART1RX) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA11_USART1CTS) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA12_USART1RTS) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA13_SWDIO) | \
                                     PIN_MODE_ALTERNATE(GPIOA_PA14_SWCLK) | \
                                     PIN_MODE_INPUT(GPIOA_PA15_NC))

#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PA0_MEAS12V) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA1_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA2_CCHIBL) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA3_CCSPICSL) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA4_CCIRQ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA5_CCSPICLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA6_CCSPIMISO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA7_CCSPIMOSI) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA8_NC_MCO1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA9_USART1TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA10_USART1RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA11_USART1CTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA12_USART1RTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA13_SWDIO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA14_SWCLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PA15_NC))

#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_PA0_MEAS12V) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA1_NC) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA2_CCHIBL) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA3_CCSPICSL) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA4_CCIRQ) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA5_CCSPICLK) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA6_CCSPIMISO) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA7_CCSPIMOSI) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA8_NC_MCO1) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA9_USART1TX) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA10_USART1RX) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA11_USART1CTS) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA12_USART1RTS) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA13_SWDIO) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA14_SWCLK) | \
                                     PIN_OSPEED_HIGH(GPIOA_PA15_NC))

#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_PA0_MEAS12V) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA1_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA2_CCHIBL) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA3_CCSPICSL) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA4_CCIRQ) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA5_CCSPICLK) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA6_CCSPIMISO) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA7_CCSPIMOSI) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA8_NC_MCO1) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA9_USART1TX) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA10_USART1RX) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA11_USART1CTS) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA12_USART1RTS) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA13_SWDIO) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA14_SWCLK) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PA15_NC))

#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PA0_MEAS12V) | \
                                     PIN_ODR_HIGH(GPIOA_PA1_NC) | \
                                     PIN_ODR_LOW(GPIOA_PA2_CCHIBL) | \
                                     PIN_ODR_HIGH(GPIOA_PA3_CCSPICSL) | \
                                     PIN_ODR_HIGH(GPIOA_PA4_CCIRQ) | \
                                     PIN_ODR_HIGH(GPIOA_PA5_CCSPICLK) | \
                                     PIN_ODR_HIGH(GPIOA_PA6_CCSPIMISO) | \
                                     PIN_ODR_HIGH(GPIOA_PA7_CCSPIMOSI) | \
                                     PIN_ODR_HIGH(GPIOA_PA8_NC_MCO1) | \
                                     PIN_ODR_HIGH(GPIOA_PA9_USART1TX) | \
                                     PIN_ODR_HIGH(GPIOA_PA10_USART1RX) | \
                                     PIN_ODR_HIGH(GPIOA_PA11_USART1CTS) | \
                                     PIN_ODR_HIGH(GPIOA_PA12_USART1RTS) | \
                                     PIN_ODR_HIGH(GPIOA_PA13_SWDIO) | \
                                     PIN_ODR_HIGH(GPIOA_PA14_SWCLK) | \
                                     PIN_ODR_HIGH(GPIOA_PA15_NC))

#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PA0_MEAS12V, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA1_NC, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA2_CCHIBL, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA3_CCSPICSL, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA4_CCIRQ, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA5_CCSPICLK, 5) /* SPI1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA6_CCSPIMISO, 5) /* SPI1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA7_CCSPIMOSI, 5)) /* SPI1 */
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PA8_NC_MCO1, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA9_USART1TX, 7) /* USART1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA10_USART1RX, 7) /* USART1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA11_USART1CTS, 7) /* USART1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA12_USART1RTS, 7) /* USART1 */ | \
                                     PIN_AFIO_AF(GPIOA_PA13_SWDIO, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA14_SWCLK, 0) | \
                                     PIN_AFIO_AF(GPIOA_PA15_NC, 0))



#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PB0_NC) | \
                                     PIN_MODE_INPUT(GPIOB_PB1_NC) | \
                                     PIN_MODE_INPUT(GPIOB_PB2_BOOT1) | \
                                     PIN_MODE_INPUT(GPIOB_PB3_NC) | \
                                     PIN_MODE_INPUT(GPIOB_PB4_NC) | \
                                     PIN_MODE_INPUT(GPIOB_PB5_NC) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PB6_I2C1SCL) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PB7_I2C1SDA) | \
                                     PIN_MODE_OUTPUT(GPIOB_PB8_REDLED) | \
                                     PIN_MODE_OUTPUT(GPIOB_PB9_GREENLED) | \
                                     PIN_MODE_OUTPUT(GPIOB_PB10_D1BRAKE) | \
                                     PIN_MODE_OUTPUT(GPIOB_PB11_D1REVERSE) | \
                                     PIN_MODE_OUTPUT(GPIOB_PB12_D1SPICSL) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PB13_D1SPICLK) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PB14_D1SPIMISO) | \
                                     PIN_MODE_ALTERNATE(GPIOB_PB15_D1SPIMOSI))

#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PB0_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB1_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB2_BOOT1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB3_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB4_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB5_NC) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_PB6_I2C1SCL) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_PB7_I2C1SDA) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB8_REDLED) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB9_GREENLED) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB10_D1BRAKE) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB11_D1REVERSE) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB12_D1SPICSL) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB13_D1SPICLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB14_D1SPIMISO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PB15_D1SPIMOSI))

#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_PB0_NC) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB1_NC) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB2_BOOT1) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB3_NC) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB4_NC) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB5_NC) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB6_I2C1SCL) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB7_I2C1SDA) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB8_REDLED) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB9_GREENLED) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB10_D1BRAKE) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB11_D1REVERSE) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB12_D1SPICSL) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB13_D1SPICLK) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB14_D1SPIMISO) | \
                                     PIN_OSPEED_HIGH(GPIOB_PB15_D1SPIMOSI))

#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_PB0_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB1_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB2_BOOT1) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB3_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB4_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB5_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB6_I2C1SCL) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB7_I2C1SDA) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB8_REDLED) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB9_GREENLED) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB10_D1BRAKE) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB11_D1REVERSE) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB12_D1SPICSL) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB13_D1SPICLK) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB14_D1SPIMISO) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PB15_D1SPIMOSI))

#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PB0_NC) | \
                                     PIN_ODR_HIGH(GPIOB_PB1_NC) | \
                                     PIN_ODR_HIGH(GPIOB_PB2_BOOT1) | \
                                     PIN_ODR_HIGH(GPIOB_PB3_NC) | \
                                     PIN_ODR_HIGH(GPIOB_PB4_NC) | \
                                     PIN_ODR_HIGH(GPIOB_PB5_NC) | \
                                     PIN_ODR_HIGH(GPIOB_PB6_I2C1SCL) | \
                                     PIN_ODR_HIGH(GPIOB_PB7_I2C1SDA) | \
                                     PIN_ODR_HIGH(GPIOB_PB8_REDLED) | \
                                     PIN_ODR_LOW(GPIOB_PB9_GREENLED) | \
                                     PIN_ODR_HIGH(GPIOB_PB10_D1BRAKE) | \
                                     PIN_ODR_LOW(GPIOB_PB11_D1REVERSE) | \
                                     PIN_ODR_HIGH(GPIOB_PB12_D1SPICSL) | \
                                     PIN_ODR_HIGH(GPIOB_PB13_D1SPICLK) | \
                                     PIN_ODR_HIGH(GPIOB_PB14_D1SPIMISO) | \
                                     PIN_ODR_HIGH(GPIOB_PB15_D1SPIMOSI))

#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PB0_NC, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB1_NC, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB2_BOOT1, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB3_NC, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB4_NC, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB5_NC, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB6_I2C1SCL, 4) /* I2C1 */ | \
                                     PIN_AFIO_AF(GPIOB_PB7_I2C1SDA, 4)) /* I2C1 */
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PB8_REDLED, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB9_GREENLED, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB10_D1BRAKE, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB11_D1REVERSE, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB12_D1SPICSL, 0) | \
                                     PIN_AFIO_AF(GPIOB_PB13_D1SPICLK, 5) /* SPI2 */ | \
                                     PIN_AFIO_AF(GPIOB_PB14_D1SPIMISO, 5) /* SPI2 */ | \
                                     PIN_AFIO_AF(GPIOB_PB15_D1SPIMOSI, 5))  /* SPI2 */



#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PC0_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC1_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC2_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC3_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC4_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC5_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC6_NC) |  \
                                     PIN_MODE_OUTPUT(GPIOC_PC7_D2BRAKE) |  \
                                     PIN_MODE_OUTPUT(GPIOC_PC8_D2REVERSE) |  \
                                     PIN_MODE_OUTPUT(GPIOC_PC9_D2SPICSL) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_PC10_D2SPICLK) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_PC11_D2SPIMISO) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_PC12_D2SPIMOSI) |  \
                                     PIN_MODE_INPUT(GPIOC_PC13_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC14_NC) |  \
                                     PIN_MODE_INPUT(GPIOC_PC15_NC))

#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PC0_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC1_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC2_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC3_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC4_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC5_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC6_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC7_D2BRAKE) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC8_D2REVERSE) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC9_D2SPICSL) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC10_D2SPICLK) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC11_D2SPIMISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC12_D2SPIMOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC13_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC14_NC) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PC15_NC))

#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_PC0_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC1_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC2_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC3_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC4_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC5_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC6_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC7_D2BRAKE) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC8_D2REVERSE) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC9_D2SPICSL) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC10_D2SPICLK) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC11_D2SPIMISO) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC12_D2SPIMOSI) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC13_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC14_NC) |  \
                                     PIN_OSPEED_HIGH(GPIOC_PC15_NC))

#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PC0_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC1_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC2_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC3_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC4_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC5_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC6_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC7_D2BRAKE) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC8_D2REVERSE) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC9_D2SPICSL) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC10_D2SPICLK) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC11_D2SPIMISO) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC12_D2SPIMOSI) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC13_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC14_NC) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_PC15_NC))

#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PC0_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC1_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC2_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC3_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC4_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC5_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC6_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC7_D2BRAKE) |  \
                                     PIN_ODR_LOW(GPIOC_PC8_D2REVERSE) |  \
                                     PIN_ODR_HIGH(GPIOC_PC9_D2SPICSL) |  \
                                     PIN_ODR_HIGH(GPIOC_PC10_D2SPICLK) |  \
                                     PIN_ODR_HIGH(GPIOC_PC11_D2SPIMISO) |  \
                                     PIN_ODR_HIGH(GPIOC_PC12_D2SPIMOSI) |  \
                                     PIN_ODR_HIGH(GPIOC_PC13_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC14_NC) |  \
                                     PIN_ODR_HIGH(GPIOC_PC15_NC))

#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PC0_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC1_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC2_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC3_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC4_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC5_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC6_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC7_D2BRAKE, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PC8_D2REVERSE, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC9_D2SPICSL, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC10_D2SPICLK, 6) /* SPI3 */ |  \
                                     PIN_AFIO_AF(GPIOC_PC11_D2SPIMISO, 6) /* SPI3 */ |  \
                                     PIN_AFIO_AF(GPIOC_PC12_D2SPIMOSI, 6) /* SPI3 */ |  \
                                     PIN_AFIO_AF(GPIOC_PC13_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC14_NC, 0) |  \
                                     PIN_AFIO_AF(GPIOC_PC15_NC, 0))



#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PD0_NC) | \
                                     PIN_MODE_INPUT(GPIOD_PD1_NC) | \
                                     PIN_MODE_INPUT(GPIOD_PD2_NC) | \
                                     PIN_MODE_INPUT(GPIOD_PD3_NC) | \
                                     PIN_MODE_OUTPUT(GPIOD_PD4_ACCLINKTXE) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD5_ACCLINKTX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD6_ACCLINKRX) | \
                                     PIN_MODE_INPUT(GPIOD_PD7_NC) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD8_USART3TX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD9_USART3RX) | \
                                     PIN_MODE_INPUT(GPIOD_PD10_NC) | \
                                     PIN_MODE_INPUT(GPIOD_PD11_NC) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD12_SPEED1) | \
                                     PIN_MODE_ALTERNATE(GPIOD_PD13_SPEED2) | \
                                     PIN_MODE_INPUT(GPIOD_PD14_NC) | \
                                     PIN_MODE_INPUT(GPIOD_PD15_NC))

#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PD0_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD1_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD2_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD3_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD4_ACCLINKTXE) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD5_ACCLINKTX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD6_ACCLINKRX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD7_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD8_USART3TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD9_USART3RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD10_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD11_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD12_SPEED1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD13_SPEED2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD14_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PD15_NC))

#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_PD0_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD1_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD2_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD3_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD4_ACCLINKTXE) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD5_ACCLINKTX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD6_ACCLINKRX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD7_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD8_USART3TX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD9_USART3RX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD10_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD11_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD12_SPEED1) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD13_SPEED2) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD14_NC) | \
                                     PIN_OSPEED_HIGH(GPIOD_PD15_NC))

#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_PD0_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD1_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD2_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD3_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD4_ACCLINKTXE) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD5_ACCLINKTX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD6_ACCLINKRX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD7_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD8_USART3TX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD9_USART3RX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD10_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD11_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD12_SPEED1) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD13_SPEED2) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD14_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PD15_NC))

#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PD0_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD1_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD2_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD3_NC) | \
                                     PIN_ODR_LOW(GPIOD_PD4_ACCLINKTXE) | \
                                     PIN_ODR_HIGH(GPIOD_PD5_ACCLINKTX) | \
                                     PIN_ODR_HIGH(GPIOD_PD6_ACCLINKRX) | \
                                     PIN_ODR_HIGH(GPIOD_PD7_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD8_USART3TX) | \
                                     PIN_ODR_HIGH(GPIOD_PD9_USART3RX) | \
                                     PIN_ODR_HIGH(GPIOD_PD10_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD11_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD12_SPEED1) | \
                                     PIN_ODR_HIGH(GPIOD_PD13_SPEED2) | \
                                     PIN_ODR_HIGH(GPIOD_PD14_NC) | \
                                     PIN_ODR_HIGH(GPIOD_PD15_NC))

#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PD0_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD1_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD2_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD3_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD4_ACCLINKTXE, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD5_ACCLINKTX, 7) /* USART2 */ | \
                                     PIN_AFIO_AF(GPIOD_PD6_ACCLINKRX, 7) /* USART2 */ | \
                                     PIN_AFIO_AF(GPIOD_PD7_NC, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PD8_USART3TX, 7) /* USART3 */ | \
                                     PIN_AFIO_AF(GPIOD_PD9_USART3RX, 7) /* USART3 */ | \
                                     PIN_AFIO_AF(GPIOD_PD10_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD11_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD12_SPEED1, 2) /* TIM4 */ | \
                                     PIN_AFIO_AF(GPIOD_PD13_SPEED2, 2) /* TIM4 */ | \
                                     PIN_AFIO_AF(GPIOD_PD14_NC, 0) | \
                                     PIN_AFIO_AF(GPIOD_PD15_NC, 0))



#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PE0_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE1_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE2_NC) | \
                                     PIN_MODE_OUTPUT(GPIOE_PE3_MOTORL1) | \
                                     PIN_MODE_OUTPUT(GPIOE_PE4_MOTORL2) | \
                                     PIN_MODE_ALTERNATE(GPIOE_PE5_MOTORH1) | \
                                     PIN_MODE_ALTERNATE(GPIOE_PE6_MOTORH2) | \
                                     PIN_MODE_INPUT(GPIOE_PE7_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE8_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE9_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE10_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE11_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE12_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE13_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE14_NC) | \
                                     PIN_MODE_INPUT(GPIOE_PE15_NC))

#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PE0_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE1_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE2_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE3_MOTORL1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE4_MOTORL2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE5_MOTORH1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE6_MOTORH2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE7_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE8_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE9_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE10_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE11_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE12_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE13_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE14_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PE15_NC))

#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_PE0_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE1_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE2_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE3_MOTORL1) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE4_MOTORL2) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE5_MOTORH1) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE6_MOTORH2) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE7_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE8_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE9_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE10_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE11_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE12_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE13_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE14_NC) | \
                                     PIN_OSPEED_HIGH(GPIOE_PE15_NC))

#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PE0_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE1_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE2_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE3_MOTORL1) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE4_MOTORL2) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE5_MOTORH1) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE6_MOTORH2) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE7_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE8_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE9_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE10_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE11_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE12_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE13_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE14_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PE15_NC))

#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PE0_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE1_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE2_NC) | \
                                     PIN_ODR_LOW(GPIOE_PE3_MOTORL1) | \
                                     PIN_ODR_LOW(GPIOE_PE4_MOTORL2) | \
                                     PIN_ODR_LOW(GPIOE_PE5_MOTORH1) | \
                                     PIN_ODR_LOW(GPIOE_PE6_MOTORH2) | \
                                     PIN_ODR_HIGH(GPIOE_PE7_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE8_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE9_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE10_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE11_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE12_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE13_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE14_NC) | \
                                     PIN_ODR_HIGH(GPIOE_PE15_NC))

#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PE0_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE1_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE2_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE3_MOTORL1, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE4_MOTORL2, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE5_MOTORH1, 3) /* TIM9 */ | \
                                     PIN_AFIO_AF(GPIOE_PE6_MOTORH2, 3) /* TIM9 */ | \
                                     PIN_AFIO_AF(GPIOE_PE7_NC, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PE8_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE9_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE10_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE11_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE12_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE13_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE14_NC, 0) | \
                                     PIN_AFIO_AF(GPIOE_PE15_NC, 0))



#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PF0_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF1_BUTTON1) | \
                                     PIN_MODE_INPUT(GPIOF_PF2_BUTTON2) | \
                                     PIN_MODE_ANALOG(GPIOF_PF3_MOTORCURR) /* ADC3 IN9 */| \
                                     PIN_MODE_ANALOG(GPIOF_PF4_POT1) /* ADC3 IN14 */| \
                                     PIN_MODE_ANALOG(GPIOF_PF5_POT2) /* ADC3 IN15 */| \
                                     PIN_MODE_ALTERNATE(GPIOF_PF6_OUT1) | \
                                     PIN_MODE_ALTERNATE(GPIOF_PF7_OUT2) | \
                                     PIN_MODE_ALTERNATE(GPIOF_PF8_OUT3) | \
                                     PIN_MODE_ALTERNATE(GPIOF_PF9_OUT4) | \
                                     PIN_MODE_INPUT(GPIOF_PF10_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF11_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF12_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF13_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF14_NC) | \
                                     PIN_MODE_INPUT(GPIOF_PF15_NC))

#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PF0_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF1_BUTTON1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF2_BUTTON2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF3_MOTORCURR) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF4_POT1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF5_POT2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF6_OUT1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF7_OUT2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF8_OUT3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF9_OUT4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF10_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF11_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF12_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF13_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF14_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PF15_NC))

#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_PF0_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF1_BUTTON1) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF2_BUTTON2) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF3_MOTORCURR) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF4_POT1) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF5_POT2) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF6_OUT1) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF7_OUT2) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF8_OUT3) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF9_OUT4) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF10_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF11_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF12_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF13_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF14_NC) | \
                                     PIN_OSPEED_HIGH(GPIOF_PF15_NC))

#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLUP(GPIOF_PF0_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF1_BUTTON1) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF2_BUTTON2) | \
                                     PIN_PUPDR_FLOATING(GPIOF_PF3_MOTORCURR) | \
                                     PIN_PUPDR_FLOATING(GPIOF_PF4_POT1) | \
                                     PIN_PUPDR_FLOATING(GPIOF_PF5_POT2) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF6_OUT1) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF7_OUT2) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF8_OUT3) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF9_OUT4) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF10_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF11_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF12_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF13_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF14_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOF_PF15_NC))

#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PF0_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF1_BUTTON1) | \
                                     PIN_ODR_HIGH(GPIOF_PF2_BUTTON2) | \
                                     PIN_ODR_HIGH(GPIOF_PF3_MOTORCURR) | \
                                     PIN_ODR_HIGH(GPIOF_PF4_POT1) | \
                                     PIN_ODR_HIGH(GPIOF_PF5_POT2) | \
                                     PIN_ODR_LOW(GPIOF_PF6_OUT1) | \
                                     PIN_ODR_LOW(GPIOF_PF7_OUT2) | \
                                     PIN_ODR_LOW(GPIOF_PF8_OUT3) | \
                                     PIN_ODR_LOW(GPIOF_PF9_OUT4) | \
                                     PIN_ODR_HIGH(GPIOF_PF10_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF11_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF12_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF13_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF14_NC) | \
                                     PIN_ODR_HIGH(GPIOF_PF15_NC))

#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PF0_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF1_BUTTON1, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF2_BUTTON2, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF3_MOTORCURR, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF4_POT1, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF5_POT2, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF6_OUT1, 3) /* TIM10 */ | \
                                     PIN_AFIO_AF(GPIOF_PF7_OUT2, 3)) /* TIM11 */
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PF8_OUT3, 9) /* TIM13 */ | \
                                     PIN_AFIO_AF(GPIOF_PF9_OUT4, 9) /* TIM14 */ | \
                                     PIN_AFIO_AF(GPIOF_PF10_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF11_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF12_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF13_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF14_NC, 0) | \
                                     PIN_AFIO_AF(GPIOF_PF15_NC, 0))



#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PG0_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG1_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG2_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG3_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG4_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG5_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG6_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG7_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG8_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG9_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG10_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG11_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG12_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG13_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG14_NC) | \
                                     PIN_MODE_INPUT(GPIOG_PG15_NC))

#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PG0_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG1_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG2_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG3_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG4_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG5_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG6_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG7_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG8_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG9_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG10_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG11_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG12_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG13_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG14_NC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PG15_NC))

#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_HIGH(GPIOG_PG0_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG1_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG2_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG3_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG4_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG5_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG6_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG7_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG8_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG9_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG10_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG11_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG12_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG13_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG14_NC) | \
                                     PIN_OSPEED_HIGH(GPIOG_PG15_NC))

#define VAL_GPIOG_PUPDR             (PIN_PUPDR_PULLUP(GPIOG_PG0_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG1_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG2_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG3_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG4_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG5_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG6_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG7_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG8_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG9_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG10_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG11_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG12_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG13_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG14_NC) | \
                                     PIN_PUPDR_PULLUP(GPIOG_PG15_NC))

#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PG0_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG1_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG2_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG3_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG4_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG5_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG6_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG7_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG8_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG9_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG10_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG11_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG12_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG13_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG14_NC) | \
                                     PIN_ODR_HIGH(GPIOG_PG15_NC))

#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PG0_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG1_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG2_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG3_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG4_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG5_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG6_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG7_NC, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PG8_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG9_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG10_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG11_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG12_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG13_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG14_NC, 0) | \
                                     PIN_AFIO_AF(GPIOG_PG15_NC, 0))



#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_PH0_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_PH1_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))

#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_PH0_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PH1_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))

#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_PH0_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOH_PH1_OSC_OUT) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN15))

#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_PH0_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_PH1_OSC_OUT) |    \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOH_PIN15))

#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_PH0_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_PH1_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))

#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_PH0_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_PH1_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))



#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))

#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))

#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOI_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN15))

#define VAL_GPIOI_PUPDR             (PIN_PUPDR_PULLUP(GPIOI_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOI_PIN15))

#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))

#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))



#define VAL_GPIOJ_MODER             (PIN_MODE_INPUT(GPIOJ_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN15))

#define VAL_GPIOJ_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOJ_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN15))

#define VAL_GPIOJ_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOJ_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOJ_PIN15))

#define VAL_GPIOJ_PUPDR             (PIN_PUPDR_PULLUP(GPIOJ_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOJ_PIN15))

#define VAL_GPIOJ_ODR               (PIN_ODR_HIGH(GPIOJ_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOJ_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOJ_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOJ_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOJ_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOJ_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOJ_PIN15))

#define VAL_GPIOJ_AFRL              (PIN_AFIO_AF(GPIOJ_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN7, 0))
#define VAL_GPIOJ_AFRH              (PIN_AFIO_AF(GPIOJ_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOJ_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN15, 0))



#define VAL_GPIOK_MODER             (PIN_MODE_INPUT(GPIOK_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN15))

#define VAL_GPIOK_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOK_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN15))

#define VAL_GPIOK_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOK_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN15))

#define VAL_GPIOK_PUPDR             (PIN_PUPDR_PULLUP(GPIOK_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOK_PIN15))

#define VAL_GPIOK_ODR               (PIN_ODR_HIGH(GPIOK_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN15))

#define VAL_GPIOK_AFRL              (PIN_AFIO_AF(GPIOK_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN7, 0))
#define VAL_GPIOK_AFRH              (PIN_AFIO_AF(GPIOK_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOK_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOK_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOK_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOK_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOK_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOK_PIN15, 0))



#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
