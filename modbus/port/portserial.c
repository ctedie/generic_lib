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

#include <stdint.h>
#include <stdbool.h>

#include "port.h"

#include "F28x_Project.h"

#include "drv_sci.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


static volatile struct SCI_REGS *sci;

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
static bool setBaudRate(volatile struct SCI_REGS *pSci, drvSciSpeed_t speed);


uint16_t m_modbusSciPort;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    sci->SCIFFRX.bit.RXFFIENA = xRxEnable;
    sci->SCIFFTX.bit.TXFFIENA = xTxEnable;
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    bool ret = true;

    if(ucPORT >= 4)
    {
        return FALSE;
    }


    m_modbusSciPort = ucPORT;

//    config.baudrate = (drvSciSpeed_t)ulBaudRate;
//    config.dataSize = (drvSciDataSize_t)ucDataBits;
//    config.parity = (drvSciParity_t)eParity;



    //TODO DRV_SCI_GetInitState
//    if(pHandle->initOk)
//    {
//        return DRV_SCI_ALREADY_INIT;
//    }

    switch (ucPORT)
    {
        case SCI_A:
            sci = &SciaRegs;
            DINT;
            EALLOW;  // This is needed to write to EALLOW protected registers
            PieVectTable.SCIA_RX_INT = prvvUARTRxISR;
            PieVectTable.SCIA_TX_INT = prvvUARTTxReadyISR;
            EDIS;    // This is needed to disable write to EALLOW protected registers
            PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
            PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   // PIE Group 9, INT1 SCIA_RX
            PieCtrlRegs.PIEIER9.bit.INTx2 = 1;   // PIE Group 9, INT2 SCIA_TX
            IER |= M_INT9;                         // Enable CPU INT Group 9
            EINT;
            break;
        case SCI_B:
            sci = &ScibRegs;
            DINT;
            EALLOW;  // This is needed to write to EALLOW protected registers
            PieVectTable.SCIB_RX_INT = prvvUARTRxISR;
            PieVectTable.SCIB_TX_INT = prvvUARTTxReadyISR;
            EDIS;    // This is needed to disable write to EALLOW protected registers
            PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
            PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   // PIE Group 9, INT3 SCIB_RX
            PieCtrlRegs.PIEIER9.bit.INTx4 = 1;   // PIE Group 9, INT4 SCIB_TX
            IER |= M_INT9;                         // Enable CPU INT Group 9
            EINT;
            break;
        case SCI_C:
            sci = &ScicRegs;
            DINT;
            EALLOW;  // This is needed to write to EALLOW protected registers
            PieVectTable.SCIC_RX_INT = prvvUARTRxISR;
            PieVectTable.SCIC_TX_INT = prvvUARTTxReadyISR;
            EDIS;    // This is needed to disable write to EALLOW protected registers
            PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
            PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   // PIE Group 8, INT5 SCIC_RX
            PieCtrlRegs.PIEIER8.bit.INTx6 = 1;   // PIE Group 8, INT6 SCIC_TX
            IER |= M_INT8;                         // Enable CPU INT Group 8
            EINT;
            break;
        case SCI_D:
            sci = &ScidRegs;
            DINT;
            EALLOW;  // This is needed to write to EALLOW protected registers
            PieVectTable.SCID_RX_INT = prvvUARTRxISR;
            PieVectTable.SCID_TX_INT = prvvUARTTxReadyISR;
            EDIS;    // This is needed to disable write to EALLOW protected registers
            PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
            PieCtrlRegs.PIEIER8.bit.INTx7 = 1;   // PIE Group 8, INT1 SCID_RX
            PieCtrlRegs.PIEIER8.bit.INTx8 = 1;   // PIE Group 8, INT2 SCID_TX
            IER |= M_INT8;                         // Enable CPU INT Group 8
            EINT;
            break;
        default:
            return DRV_SCI_BAD_CONFIG;
    }



    /* Data size config */
    sci->SCICCR.bit.SCICHAR = (ucDataBits - 1);

    sci->SCICTL1.bit.SWRESET = 0;  //Set in reset state

    /* Parity Config */
    if (eParity != MB_PAR_NONE)
    {
        sci->SCICCR.bit.PARITYENA = 1;
        sci->SCICCR.bit.PARITY = eParity - 1;
    }

    /* Stop bit config */
    sci->SCICCR.bit.STOPBITS = 0;

    /* Baud Rate config */
    setBaudRate(sci, ulBaudRate);

    sci->SCICTL1.bit.RXENA = 1;
//    sci->SCICTL2.bit.RXBKINTENA;
    sci->SCICTL1.bit.TXENA = 1;

    sci->SCICTL2.bit.TXINTENA = 1;
    sci->SCICTL2.bit.RXBKINTENA = 1;

    sci->SCIFFTX.all = 0xC000; //Disable Fifo Tx Interrupts
    sci->SCIFFRX.all = 0x0021; //Enable Fifo Rx Interrupts
    sci->SCIFFCT.all = 0x00;

    sci->SCICTL1.bit.SWRESET = 1; //Release from reset state
    sci->SCIFFTX.bit.TXFIFORESET = 1;
    sci->SCIFFRX.bit.RXFIFORESET = 1;

    return ret;


}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    bool ret = false;

    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    if(sci->SCICTL2.bit.TXRDY)
    {
        sci->SCITXBUF.all = ucByte;
        ret = true;
    }

    return ret;

}

BOOL
xMBPortSerialGetByte(CHAR* pucByte)
{
    bool ret = false;
    uint16_t *pCar = (uint16_t*)pucByte;
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack aftetmpCharr pxMBFrameCBByteReceived( ) has been called.
     */
    if(sci->SCIFFRX.bit.RXFFST > 0)
    {
        *pCar = sci->SCIRXBUF.bit.SAR;
        ret = true;
    }

    return ret;
}

/**
 **********************************************************
 * \brief Set the baudrate with the choosen parameters
 *
 * \param [in]  pSci    A pointer to sci register
 * \param [in]  speed   One of the baudrate value of drvSciSpeed_t
 *
 * \return
 **********************************************************/
static bool setBaudRate(volatile struct SCI_REGS *pSci, uint32_t speed)
{
    uint16_t brr_Value;

    //TODO Get CPU clock instead of 200000000
    brr_Value = 200000000 / (speed * 8 * (ClkCfgRegs.LOSPCP.bit.LSPCLKDIV * 2));
    pSci->SCIHBAUD.all = (brr_Value >> 8) & 0xFF;
    pSci->SCILBAUD.all = (brr_Value & 0xFF);

    return true;
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

    sci->SCIFFTX.bit.TXFFINTCLR=1;

    if((sci == (&SciaRegs)) || (sci == (&ScibRegs)))
    {
        PieCtrlRegs.PIEACK.all|=0x100;
    }
    else if((sci == (&ScicRegs)) || (sci == (&ScidRegs)))
    {
        PieCtrlRegs.PIEACK.all|=0x80;
    }
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static __interrupt void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );

    sci->SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    sci->SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    if((sci == (&SciaRegs)) || (sci == (&ScibRegs)))
    {
        PieCtrlRegs.PIEACK.all|=0x100;
    }
    else if((sci == (&ScicRegs)) || (sci == (&ScidRegs)))
    {
        PieCtrlRegs.PIEACK.all|=0x80;
    }



}
