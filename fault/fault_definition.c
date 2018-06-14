/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		14 juin 2018
 *  \addtogroup	FAULT_DEFINITION
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		fault_definition.c
 *  
 *  \brief		FAULT_DEFINITION source file
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "fault_mngt.h"
#include "fault_config.h"
#include "fault_definition.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------------------------*/
fault_t FLT_FirstFault = {
                         .appearenceCond = FAULT_CONFIG_CondApp_FirstFault,
                         .appearenceAction = FAULT_CONFIG_ActApp_FirstFault,
                         .disappearenceCond = FAULT_CONFIG_CondDis_FirstFault,
                         .disappearenceAction = FAULT_CONFIG_ActDis_FirstFault,
                         .activeAction = FAULT_CONFIG_Active_FirstFault,
                         .faultType = MAJOR,
                         .initState = INACTIVE
                         };

fault_t FLT_SecondFault = {
                         .appearenceCond = FAULT_CONFIG_CondApp_SecondFault,
                         .appearenceAction = FAULT_CONFIG_ActApp_SecondFault,
                         .disappearenceCond = FAULT_CONFIG_CondDis_SecondFault,
                         .disappearenceAction = FAULT_CONFIG_ActDis_SecondFault,
                         .activeAction = FAULT_CONFIG_Active_SecondFault,
                         .faultType = MAJOR,
                         .initState = INACTIVE
                         };





const faultDesc_t FAULT_tab[FAULT_MAX_NB_FAULT+1] =
{
 {.pFault = &FLT_FirstFault},
 {.pFault = &FLT_SecondFault},
 //The last must be null
 {NULL}
};
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


/** \} */
/******************************************************** EOF *******************************************************/
