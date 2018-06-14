/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		13 juin 2018
 *  \addtogroup	MODULES
 *  \{
 *  \addtogroup	FAULT
 *  \{
 *  
 *  \brief 
 *  
 *  \details 
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		fault.h
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/
#ifndef FAULT_H_
#define FAULT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Public Macro definition -----------------------------------------------------------------------------------------*/
/* Public Constant definition --------------------------------------------------------------------------------------*/
#define FAULT_MAX_NB_FAULT  50

/* Public Type definition  -----------------------------------------------------------------------------------------*/
typedef enum
{
    INACTIVE = 0,
    ACTIVATING,
    ACTIVE,
    INACTIVATING
}faultState_t;

typedef enum
{
    INFO = 0,
    MINOR,
    MAJOR
}faultType_t;

typedef struct
{
    bool (*appearenceCond)(void* pArg);
    bool (*disappearenceCond)(void* pArg);
    void (*appearenceAction)(void* pArg);
    void (*disappearenceAction)(void* pArg);
    void (*activeAction)(void* pArg);

    faultType_t faultType;

    faultState_t initState;
    faultState_t state;
    uint64_t id;
    void (*fctState)(void *pArg);

}fault_t;

typedef struct
{
    fault_t *pFault;
}faultDesc_t;

/* Public variables ------------------------------------------------------------------------------------------------*/
extern uint64_t FAULT_InfoMask;
extern uint64_t FAULT_MinorMask;
extern uint64_t FAULT_MajorMask;

extern const faultDesc_t FAULT_tab[FAULT_MAX_NB_FAULT];

/* Public functions ------------------------------------------------------------------------------------------------*/
void FAULT_Init(void);
void FAULT_Process(void);

#endif /* FAULT_H_ */

#ifdef __cplusplus
}
#endif

/** \} */
/** \} */
/******************************************************** EOF *******************************************************/
