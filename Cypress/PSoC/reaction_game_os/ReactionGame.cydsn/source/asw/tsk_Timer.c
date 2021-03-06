/**
* \file <tsk_Timer.c>
* \author <Bharath Ramachandraiah>
* \date <13/11/2018>
*
* \brief <Symbolic File name>
*
* \copyright Copyright �2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "tsk_Timer.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static uint32_t TIMER__1ms_counter = 0;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * The Task declaration
 * The tsk_Timer is a cyclic task which is activated every 1ms to measure elapsed time.
 * The task is activated via an alarm.
 */
TASK(tsk_Timer)
{
    ++TIMER__1ms_counter;
    // Terminate the init task and let the schedular do its thing!
    TerminateTask();
}


/**
 * A function which triggers the time measurement.
 * @return RC_t     :   RC_SUCCESS on successful start of the timer.
 */
RC_t TIMER_StartTimeMeasurement(void)
{
    TIMER__1ms_counter = 0;
    
    // Set Alarms here
    SetRelAlarm(alrm_Tick1ms, 1, 1);
    
    return RC_SUCCESS;
}


/**
 * A function which measures the elapsed time from the start time.
 * @param uint32_t *MeasureReactionTime : OUT the value of measured time will be updated. 
 * @return RC_t                         : RC_SUCCESS on successful retrival of the time.
 */
RC_t TIMER_ElapsedTimeMeasurement(uint32_t *MeasureReactionTime)
{
    *MeasureReactionTime = TIMER__1ms_counter;
    
    // Cancel Alarms here
    CancelAlarm(alrm_Tick1ms);
    
    return RC_SUCCESS;
}