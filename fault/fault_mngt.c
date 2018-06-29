/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		13 juin 2018
 *  \addtogroup	FAULT
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		fault_mngt.c
 *  
 *  \brief		FAULT module source file
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "fault_mngt.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
/* Constant definition ---------------------------------------------------------------------------------------------*/
/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
uint64_t FAULT_InfoMask;
uint64_t FAULT_MinorMask;
uint64_t FAULT_MajorMask;

/* Private variables -----------------------------------------------------------------------------------------------*/
/* Private functions prototypes ------------------------------------------------------------------------------------*/
static void fctState_Inactive(void *pArg);
static void fctState_Activating(void *pArg);
static void fctState_Active(void *pArg);
static void fctState_Inactivating(void *pArg);

/* Private functions -----------------------------------------------------------------------------------------------*/

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
static void fctState_Inactive(void *pArg)
{
    fault_t *pFault = (fault_t*)pArg;

    if((pFault->appearenceCond != NULL) && (pFault->appearenceCond(NULL) == true))
    {
        //Detected Fault
        pFault->state = ACTIVE;
        pFault->fctState = fctState_Active;

        if(pFault->appearenceAction != NULL)
        {
            pFault->appearenceAction(NULL);
        }

        // Update fault mask
        switch (pFault->faultType) {
            case INFO:
                FAULT_InfoMask |= (1ULL << pFault->id);
                break;
            case MINOR:
                FAULT_MinorMask |= (1ULL << pFault->id);
                break;
            case MAJOR:
                FAULT_MajorMask |= (1ULL << pFault->id);
                break;
            default:
                break;
        }
    }
}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
static void fctState_Active(void *pArg)
{
    fault_t *pFault = (fault_t*)pArg;
    uint64_t mask = 0;

    if((pFault->disappearenceCond != NULL) && (pFault->disappearenceCond(NULL) == true))
    {
        //Disappeared Fault
        pFault->state = INACTIVE;
        pFault->fctState = fctState_Inactive;

        if(pFault->disappearenceAction != NULL)
        {
            pFault->disappearenceAction(NULL);
        }

        mask = (1 << pFault->id);

        switch (pFault->faultType)
        {
            case INFO:
                FAULT_InfoMask &= ~mask;
                break;
            case MINOR:
                FAULT_MinorMask &= ~mask;
                break;
            case MAJOR:
                FAULT_MajorMask &= ~mask;
                break;
            default:
                break;
        }
    }
    else
    {
        if(pFault->activeAction != NULL)
        {
            pFault->activeAction(NULL);
        }
    }

}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
static void fctState_Activating(void *pArg)
{
    fault_t *pFault = (fault_t*)pArg;


}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
static void fctState_Inactivating(void *pArg)
{
    fault_t *pFault = (fault_t*)pArg;


}


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
void FAULT_Init(void)
{
    uint32_t indexFault;
    fault_t *pFault;

    FAULT_InfoMask = 0;
    FAULT_MinorMask = 0;
    FAULT_MajorMask = 0;

    for (indexFault = 0; FAULT_tab[indexFault].pFault != NULL; ++indexFault)
    {
        pFault = FAULT_tab[indexFault].pFault;

        pFault->state = pFault->initState;
        switch (pFault->state)
        {
            case INACTIVE:
                pFault->fctState = fctState_Inactive;
                break;
            case ACTIVATING:
                pFault->fctState = fctState_Activating;
                break;
            case ACTIVE:
                pFault->fctState = fctState_Active;
                break;
            case INACTIVATING:
                pFault->fctState = fctState_Inactivating;
                break;
        }

        pFault->id = indexFault;

    }

}

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
void FAULT_Process(void)
{
    uint16_t faultId;

    fault_t *pFault = NULL;

    for (faultId = 0; FAULT_tab[faultId].pFault != NULL; ++faultId)
    {
        pFault = FAULT_tab[faultId].pFault;

        pFault->fctState(pFault);
    }
}

/** \} */
/******************************************************** EOF *******************************************************/
