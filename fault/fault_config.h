/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 juin 2018
 *  \addtogroup	FAULT
 *  \{
 *  \addtogroup	FAULT_CONFIG
 *  \{
 *  
 *  \brief The fault Configuration module
 *  
 *  \details 
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		fault_config.h
 *  
 *  \brief		FAULT_CONFIG header file
 *
 *  \details	
 *
 ********************************************************************************************************************/
#ifndef FAULT_CONFIG_H_
#define FAULT_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "fault_mngt.h"
#include "fault_config.h"

/* Public Macro definition -----------------------------------------------------------------------------------------*/
/* Public Constant definition --------------------------------------------------------------------------------------*/
/* Public Type definition  -----------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/

bool FAULT_CONFIG_CondApp_FirstFault(void *pArg);
void FAULT_CONFIG_ActApp_FirstFault(void *pArg);
bool FAULT_CONFIG_CondDis_FirstFault(void *pArg);
void FAULT_CONFIG_ActDis_FirstFault(void *pArg);
void FAULT_CONFIG_Active_FirstFault(void *pArg);

bool FAULT_CONFIG_CondApp_SecondFault(void *pArg);
void FAULT_CONFIG_ActApp_SecondFault(void *pArg);
bool FAULT_CONFIG_CondDis_SecondFault(void *pArg);
void FAULT_CONFIG_ActDis_SecondFault(void *pArg);
void FAULT_CONFIG_Active_SecondFault(void *pArg);
#endif /* FAULT_CONFIG_H_ */

#ifdef __cplusplus
}
#endif

/** \} */
/** \} */
/******************************************************** EOF *******************************************************/
