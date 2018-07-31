/*********************************************************************************************************************
 *                                                      COMECA
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \author		tedie.cedric
 *  \date		25 juil. 2018
 *  \addtogroup	TODO
 *  \{
 ********************************************************************************************************************/
/**
 *********************************************************************************************************************
 *  \file		ptime.c
 *  
 *  \brief		TODO
 *
 *  \details	
 *
 ********************************************************************************************************************/

/* Includes --------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ptime.h"

/* Macro definition ------------------------------------------------------------------------------------------------*/
//#define IS_JANUARY(x)   ()

/* Constant definition ---------------------------------------------------------------------------------------------*/
#define YEAR_IN_SECOND  31557600U
#define DAY_IN_SECOND   86400U

uint16_t m_isLeapYear = 0;
static uint16_t TAB_MONTH[] =
{
 31, //JANUARY
 28, //FEBRUARY
 31, //MARCH
 30, //APRIL
 31, //MAY
 30, //JUNE
 31, //LULY
 31, //AUGUST
 30, //SEPTEMBRE
 31, //OCTOBER
 30, //NOVEMBER
 31  //DECEMBER
};

/* Type definition  ------------------------------------------------------------------------------------------------*/
/* Public variables ------------------------------------------------------------------------------------------------*/
uint64_t PTIME_timestamp_ms = 0;
/* Private variables -----------------------------------------------------------------------------------------------*/
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
void PTIME_init(uint64_t initTimestamp)
{
    if (initTimestamp != NULL)
    {
        PTIME_timestamp_ms = initTimestamp;
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
void PTIME_update(uint32_t updateVal_ms)
{
    PTIME_timestamp_ms += updateVal_ms;
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
inline uint64_t PTIME_get(void)
{
    return PTIME_timestamp_ms;
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
void PTIME_getDate(ptime_t *date)
{
    uint64_t s = PTIME_timestamp_ms / 1000;
    uint16_t days;
    uint16_t i;


    date->year = 1970 + (s / YEAR_IN_SECOND);
    s = s % YEAR_IN_SECOND;

    days = (s / DAY_IN_SECOND) + 1;
    s = s % DAY_IN_SECOND;

    date->hours = s / 3600;
    s = s % 3600;

    date->minutes = s / 60;
    s = s % 60;

    date->seconds = s;

    if(days > 57)
    {
        if(((date->year % 4 == 0) && (date->year % 100 != 0))
           ||((date->year % 400 == 0) && (date->year % 100 == 0)))
        {
            //Ann�e bissextile
            TAB_MONTH[1] = 29;
        }
        else
        {
            TAB_MONTH[1] = 28;
        }
    }
    //calc day and month
    for ( i = 0; i < 12; ++i)
    {
        if(days > TAB_MONTH[i])
        {
            days -= TAB_MONTH[i];
        }
        else
        {
            date->month = i + 1;
            date->day = days;
            break;
        }
    }

}

/** \} */
/******************************************************** EOF *******************************************************/
