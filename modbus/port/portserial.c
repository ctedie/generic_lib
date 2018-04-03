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
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

#include "drv_sci.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
drvSciNumber_t m_modbusSciPort;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

    DRV_SCI_Enable_RxINT(m_modbusSciPort, xRxEnable);
    DRV_SCI_Enable_TxINT(m_modbusSciPort, xTxEnable);
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

    if(ucPORT >= NB_SERIAL)
    {
        return FALSE;
    }

    m_modbusSciPort = (drvSciNumber_t)ucPORT;
//    config.baudrate = (drvSciSpeed_t)ulBaudRate;
//    config.dataSize = (drvSciDataSize_t)ucDataBits;
//    config.parity = (drvSciParity_t)eParity;

    if(DRV_SCI_BasicInit(m_modbusSciPort,
                      ulBaudRate,
                      (ucDataBits - 1),
                      (drvSciParity_t)eParity,
                      DRV_SCI_STOP_BIT_1,
                      &prvvUARTRxISR,
                      &prvvUARTTxReadyISR)
            != DRV_SCI_SUCCESS)
    {
        return FALSE;
    }


    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{

    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    if(DRV_SCI_WriteChar_NonBlocking(m_modbusSciPort, (uint16_t)ucByte) != DRV_SCI_SUCCESS)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL
xMBPortSerialGetByte(CHAR* pucByte)
{

    uint16_t *car = (uint16_t*)pucByte;
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack aftetmpCharr pxMBFrameCBByteReceived( ) has been called.
     */
    //TODO use byte macro for pucByte

    if(DRV_SCI_ReadChar_NonBlocking(0, car) != DRV_SCI_SUCCESS)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static __interrupt void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );

    DRV_SCI_ClearIT_Tx(m_modbusSciPort);
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static __interrupt void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
    DRV_SCI_ClearIT_Rx(m_modbusSciPort);
}
