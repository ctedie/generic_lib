/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 juin 2018
 *  \addtogroup	FAULT_CONFIG
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		fault_config.c
 *  
 *  \brief		FAULT_CONFIG source file
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "fault_mngt.h"
#include "fault_config.h"
#include "fault_definition.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/

static bool ctediFault = false;
static uint32_t ctediCounter = 0;

static bool ctediFault2 = false;
static uint32_t ctediCounter2 = 0;

/* Private functions prototypes ------------------------------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------------------------------*/
/* Public functions ------------------------------------------------------------------------------------------------*/

/**
 *********************************************************
 * \brief 
 *
 * \param [in]  
 * \param [out]  
 *
 * \return
 *********************************************************/

bool FAULT_CONFIG_CondApp_FirstFault(void *pArg)
{

    return ctediFault;
}

void FAULT_CONFIG_ActApp_FirstFault(void *pArg)
{
}

bool FAULT_CONFIG_CondDis_FirstFault(void *pArg)
{
    return !ctediFault;
}

void FAULT_CONFIG_ActDis_FirstFault(void *pArg)
{
}

void FAULT_CONFIG_Active_FirstFault(void *pArg)
{
    ctediCounter++;
}


bool FAULT_CONFIG_CondApp_SecondFault(void *pArg)
{

    return ctediFault2;
}

void FAULT_CONFIG_ActApp_SecondFault(void *pArg)
{
}

bool FAULT_CONFIG_CondDis_SecondFault(void *pArg)
{
    return !ctediFault2;
}

void FAULT_CONFIG_ActDis_SecondFault(void *pArg)
{
}

void FAULT_CONFIG_Active_SecondFault(void *pArg)
{
    ctediCounter2++;
}

/** \} */
/******************************************************** EOF *******************************************************/
