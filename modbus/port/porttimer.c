/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "drv_timer.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void* pData);

/* ----------------------- Start implementation -----------------------------*/
drvTimerNumber_t m_modbusTimer = TIMER0;
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    // Init a 50us timer
    if(DRV_TIMER_Init(m_modbusTimer, usTim1Timerout50us * 50, TRUE, prvvTIMERExpiredISR, NULL) != DRV_TIMER_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}


void vMBPortTimersEnable( void )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    DRV_TIMER_Start(m_modbusTimer);

}

void vMBPortTimersDisable( void )
{
    /* Disable any pending timers. */
    DRV_TIMER_Stop(m_modbusTimer);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR(void* pData)
{

    ( void )pxMBPortCBTimerExpired(  );
    DRV_TIMER_CLEAR_IT_TIM0();
}

