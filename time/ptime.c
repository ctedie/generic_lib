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
#define IS_LEAP(x)   ((((((x) % 4 == 0) && ((x) % 100 != 0)) ||(((x) % 400 == 0) && ((x) % 100 == 0)))) ? true : false )

/* Constant definition ---------------------------------------------------------------------------------------------*/
//#define YEAR_IN_SECOND  31556926U

#define YEAR_IN_SECOND  31557600U //365.25
// End of year 1970     31535999
//#define YEAR_IN_SECOND  31556926U //365.24
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
    uint64_t years = s;
    uint16_t days;
    uint16_t i;


    date->year = 1970 + (years / YEAR_IN_SECOND);
    years = years % YEAR_IN_SECOND;

    date->__dayInYear = (years / DAY_IN_SECOND)+1;
    days = (years / DAY_IN_SECOND) + 1;
    years = years % DAY_IN_SECOND;

    date->hours = (s / 3600) % 24;
    s = s % 3600;

    date->minutes = s / 60;
    s = s % 60;

    date->seconds = s;

    if(days > 57)
    {
        if(((date->year % 4 == 0) && (date->year % 100 != 0))
           ||((date->year % 400 == 0) && (date->year % 100 == 0)))
        {
            //Année bissextile
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

/**
 *********************************************************
 * \brief
 *
 * \param [in]
 * \param [out]
 *
 * \return
 *********************************************************/
void PTIME_setDate(ptime_t *date)
{
    uint16_t i;
    uint16_t days = 0;

    PTIME_timestamp_ms = (uint64_t)((uint64_t)(date->year - 1970) * YEAR_IN_SECOND) * 1000;

    //check leap year
    if(IS_LEAP(date->year))
    {
        //Année bissextile
        TAB_MONTH[1] = 29;
    }
    else
    {
        TAB_MONTH[1] = 28;
    }


    for (i = 1; i < date->month; ++i)
    {
        days += TAB_MONTH[i-1];
    }

    PTIME_timestamp_ms += (uint64_t)((uint64_t)(days + (date->day - 1)) * 86400) * 1000;
    PTIME_timestamp_ms += (uint64_t)date->hours * 3600 * 1000;
    PTIME_timestamp_ms += (uint64_t)date->minutes * 60 * 1000;
    PTIME_timestamp_ms += (uint64_t)date->seconds * 1000;



}

/** \} */
/******************************************************** EOF *******************************************************/
