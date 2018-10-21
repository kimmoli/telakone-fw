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

#include <string.h>
#include "hal.h"
#include "platform.h"
#include "helpers.h"
#include "exti.h"

P_EVENT_HANDLER     pIrqEventHandler = 0;

void CC3100_IRQ_Callback(void *arg);

void CC3100_disable(void)
{
    palClearLine(LINE_CCHIBL);
}

void CC3100_enable(void)
{
    palSetLine(LINE_CCHIBL);
}

void CC3100_InterruptEnable(void)
{
    CC3100_Interrupt(true);
}

void CC3100_InterruptDisable(void)
{
    CC3100_Interrupt(false);
}

void CC3100_IRQ_Callback(void *arg)
{
    (void) arg;

    if ( NULL != pIrqEventHandler )
    {
        pIrqEventHandler(0);
    }
}

int registerInterruptHandler(P_EVENT_HANDLER InterruptHdl , void* pValue)
{
    (void) pValue;

    pIrqEventHandler = InterruptHdl;
    return 0;
}

void Delay(unsigned long delay)
{
    chThdSleepMilliseconds(delay);
}

msg_t chBSemObjectInitTK(binary_semaphore_t *bsp)
{
    chBSemObjectInit(bsp, FALSE);
    return MSG_OK;
}

msg_t chBSemSignalTK(binary_semaphore_t *bsp)
{
    chBSemSignal(bsp);

    return MSG_OK;
}

msg_t chBSemSignalITK(binary_semaphore_t *bsp)
{
    osalSysLockFromISR();
    chBSemSignalI(bsp);
    osalSysUnlockFromISR();

    return MSG_OK;
}

msg_t chBSemWaitTimeoutTK(binary_semaphore_t *bsp, systime_t time)
{
    return chBSemWaitTimeout(bsp, MS2ST(time));
}

msg_t chBSemDeleteTK(binary_semaphore_t *bsp)
{
    (void) bsp;

    return MSG_OK;
}

msg_t chMtxObjectInitTK(mutex_t *mp)
{
    chMtxObjectInit(mp);

    return MSG_OK;
}

msg_t chMtxLockTK(mutex_t *mp, systime_t timeout)
{
    msg_t res = MSG_OK;

    if (timeout == TIME_IMMEDIATE)
        res = (chMtxTryLock(mp) ? MSG_OK : MSG_RESET);
    else if (timeout == TIME_INFINITE)
        chMtxLock(mp);
    else
        res = MSG_RESET;

    return res;
}

msg_t chMtxUnlockTK(mutex_t *mp)
{
    chMtxUnlock(mp);

    return MSG_OK;
}

msg_t chMtxDeleteTK(mutex_t *mp)
{
    (void) mp;

    return MSG_OK;
}
