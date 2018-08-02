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

#define SECOND_IN_DAY       86400U
#define SECOND_IN_MINUTE    60UL
#define SECOND_IN_HOUR      3600UL
#define SECOND_IN_YEAR  (365UL * 86400UL) //365days
#define SECOND_IN_LEAP_YEAR  (366UL * 86400UL) //366days
#define SECOND_IN_YEAR_JULLIAN  (31557600UL) //365.25days

// End of year 1970     31535999

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
void PTIME_getDate(ptime_t *pDate)
{
    uint64_t s = PTIME_timestamp_ms / 1000;
    uint64_t years = s;
    uint16_t days;
    uint16_t i;
    uint64_t  nbSecToSubtract;
    bool bContinue = true;


    pDate->year = 1970;
    while(bContinue)
    {
        nbSecToSubtract = (IS_LEAP(pDate->year)) ? SECOND_IN_LEAP_YEAR :SECOND_IN_YEAR;
        if(s >= nbSecToSubtract)
        {
            s -= nbSecToSubtract;
            pDate->year++;
        }
        else
        {
            bContinue = false;
        }
    }

    pDate->month = 1;
    bContinue = true;
    while(bContinue)
    {
        if((pDate->month == 2)&&(IS_LEAP(pDate->year)))
        {
            nbSecToSubtract = 29UL*SECOND_IN_DAY;
        }
        else
        {
            nbSecToSubtract = TAB_MONTH[pDate->month - 1] * SECOND_IN_DAY;
        }

        if(s >= nbSecToSubtract)
        {
            s -= nbSecToSubtract;
            pDate->month++;
        }
        else
        {
            bContinue = false;
        }
    }

    pDate->day = (uint16_t)(s/SECOND_IN_DAY);
    s -= ((uint64_t)pDate->day * SECOND_IN_DAY);
    pDate->day+=1;

    pDate->hours = (uint16_t)(s/SECOND_IN_HOUR);
    s -= ((uint64_t)pDate->hours * SECOND_IN_HOUR);

    pDate->minutes = (uint16_t)(s/SECOND_IN_MINUTE);
    s -= ((uint64_t)pDate->minutes * SECOND_IN_MINUTE);

    pDate->seconds = (uint16_t)s;
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
    bool isLeap;

    PTIME_timestamp_ms = 0;
    isLeap = IS_LEAP(date->year);

    for (i = 1970; i < date->year; ++i)
    {
        if(IS_LEAP(i))
        {
            PTIME_timestamp_ms += SECOND_IN_LEAP_YEAR;
        }
        else
        {
            PTIME_timestamp_ms += SECOND_IN_YEAR;
        }
    }

    for (i = 1; i < date->month; ++i)
    {
        if((i==2)&(isLeap))
        {
            PTIME_timestamp_ms += 29UL * SECOND_IN_DAY;
        }
        else
        {
            PTIME_timestamp_ms += (uint64_t)TAB_MONTH[i-1] * SECOND_IN_DAY;
        }
    }


    PTIME_timestamp_ms += (uint64_t)(date->day - 1) * SECOND_IN_DAY;
    PTIME_timestamp_ms += (uint64_t)date->hours * SECOND_IN_HOUR;
    PTIME_timestamp_ms += (uint64_t)date->minutes * SECOND_IN_MINUTE;
    PTIME_timestamp_ms += (uint64_t)date->seconds;

    PTIME_timestamp_ms = PTIME_timestamp_ms * 1000;


}

/** \} */
/******************************************************** EOF *******************************************************/
