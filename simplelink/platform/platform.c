/*
 * board.c - CC3100-STM32F4 platform configuration functions
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "hal.h"
#include "platform.h"
#include "helpers.h"

/**/
P_EVENT_HANDLER     pIrqEventHandler = 0;
thread_t *spawnedThreadRef;

void HAL_GPIO_EXTI_Callback(EXTDriver *extp, expchannel_t channel);

/*!
    \brief              This function disables CC3100 device
    \param              None
    \return             None
    \note
    \warning
*/
void CC3100_disable(void)
{
    palClearLine(LINE_CCHIBL);
}

/*!
    \brief              This function enables CC3100 device
    \param              None
    \return             None
    \note
    \warning
*/
void CC3100_enable(void)
{
    palSetLine(LINE_CCHIBL);
}

/*!
    \brief              This function enables waln IrQ pin
    \param              None
    \return             None
    \note
    \warning
*/
void CC3100_InterruptEnable(void)
{
    /* Configure EXTI Line4 (connected to PA4 pin) in interrupt mode */
    extChannelEnable(&EXTD1, GPIOA_PA4_CCIRQ);
}

/*!
    \brief              This function disables waln IrQ pin
    \param              None
    \return             None
    \note
    \warning
*/
void CC3100_InterruptDisable(void)
{
    extChannelDisable(&EXTD1, GPIOA_PA4_CCIRQ);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(EXTDriver *extp, expchannel_t channel)
{
    (void) extp;


    if ( (channel == GPIOA_PA4_CCIRQ) && (NULL != pIrqEventHandler) )
    {
        pIrqEventHandler(0);
    }
}

/**/
int registerInterruptHandler(P_EVENT_HANDLER InterruptHdl , void* pValue)
{
    (void) pValue;

    pIrqEventHandler = InterruptHdl;
    return 0;
}

/*!
    \brief              Induce delay in ms
    \param              delay: specifies the delay time length, in milliseconds.
    \return             None
    \note
    \warning
*/
void Delay(unsigned long delay)
{
    chThdSleepMilliseconds(delay);
}

msg_t spawnTK(void *pEntry, void *pValue, uint32_t flags)
{
    (void) flags;

    spawnedThreadRef = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(1024), "spawn", HIGHPRIO, (tfunc_t)pEntry, pValue);

    return MSG_OK;
}
