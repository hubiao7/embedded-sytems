/**
*  file    PWM_signal.h
*
*  brief   signal base class for signal pwm
*
*  author  Autogenerated, Peter Fromm       <peter.fromm@h-da.de>
*
*
*/

#ifndef PWM_SIGNAL_H_INCLUDED
#define PWM_SIGNAL_H_INCLUDED

/*================================================[ configuration  ]================================================*/
#define PWM_ASYNCHRONOUS

/*================================================[ public includes  ]================================================*/
#include "global.h"
#include "project.h"
#include "rte.h"

#include "pwm_type.h"


/********************************************************************************************************************/
/* The driver API block																						*/
/********************************************************************************************************************/
                                                                                                                       
/**
 *  Definition of function types for driver interfaces.
 *  I-Port
 */
typedef RC_t (*PWM_driverIn_t)(PWM_data_t * const);

/**
 *  Definition of function types for driver interfaces.
 *  O-Port
 */
typedef RC_t (*PWM_driverOut_t)( const PWM_data_t * const);


/********************************************************************************************************************/
/* The signal configuration API block																				*/
/********************************************************************************************************************/

/**
 * Instances of the struct PWM_cfg_t represent the signal object configuration.
 */
struct PWM_cfg_s
{
    /**
     * Function pointer to the input driver
     */
    PWM_driverIn_t const inDriver;

    /**
     * Function pointer to the output driver table
     */
   PWM_driverOut_t const outDriver;
   
    /**
     * Event onUpdate
     */
    EventMaskType const evOnUpdate;

};                                                                                                                     
typedef const struct PWM_cfg_s PWM_cfg_t;
                              
/********************************************************************************************************************/
/* The data interface block																							*/
/********************************************************************************************************************/

                                                                                                                       
							  
/**
 * Instances of the struct PWM_t represent the signal objects.
 * A signal object contains the current application value 'value' as well as relevant meta data.
 */
struct PWM_s
{                                                                                                                      

	/**
	 * Data buffer for the application value
	 */
    PWM_data_t value;

    /**
     * Current signal status
     */
    RTE_signalStatus_t status;

    /**
     * Current age of the signal (in calling cycles)
     */
    uint32_t age;

    /**
     * Pointer to the signal configuration
     */
	const PWM_cfg_t * const    pSigCfg;

};                                                                                                                     
typedef struct PWM_s PWM_t;

/********************************************************************************************************************/
/* Signal initialisation   																							*/
/********************************************************************************************************************/

/**
 * Initializes a signal instance with a passed init value.
 * Depending on the configured conversion or scaling function the raw_value will be calculated.
 * Signal status will change from RTE_SIGNALSTATUS_STARTUP to RTE_SIGNALSTATUS_INITIALIZED.
 *
 * @param  PWM_t * const signal: Pointer to the signal object which will be initialised
 * @param const pwm_data_t * const value: Pointer to initial value of the application data
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
inline RC_t RTE_PWM_init( PWM_t * const signal, const PWM_data_t * const value )
{                                                                                                                      

   RC_t ret = RC_SUCCESS;

   //Set value and raw value
   signal->value  = *value;
   signal->age    = 0;
   signal->status = RTE_SIGNALSTATUS_INITIALIZED;

   return ret;
}

/********************************************************************************************************************/
/* Status and timeout control																						*/
/********************************************************************************************************************/

/**
 *  Returns the current signal status of the passed signal instance.
 *
 * @param  PWM_t * const signal: Pointer to the signal object
 * @return  sig_status_t: The current signal status
 *
 **/
inline RTE_signalStatus_t RTE_PWM_getStatus( const PWM_t * const signal )
{
   return signal->status;
}


/**
 *  Sets a new signal status of the passed signal instance by passing the new status.
 *
 * @param  PWM_t * const signal: Pointer to the signal object
 * @param  const sig_status_t status: New signal status
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 */
inline RC_t RTE_PWM_setStatus( PWM_t * const signal, const RTE_signalStatus_t status )
{
   signal->status = status;
   return RC_SUCCESS;
}


/**
 * Returns the count of refreshing trials (refresh, set or set_raw) that failed.
 * Prerquisite: The signal is refreshed cyclically
 *
 * @param  PWM_t * const signal: Pointer to the signal object
 * @return uint32_t: Number of cycles since the signal has been updated for the last time
 */
inline uint32_t RTE_PWM_getAge( const PWM_t * const signal )
{
   return signal->age;
}

/**
 * Should be called by application or driver if no new value can be provided.
 * @param  PWM_t * const signal: Pointer to the signal object
 * @param uint32_t increment: incrementvalue, typically in [ms]
 * @return void
 */
inline void RTE_PWM_incAge(  PWM_t * const signal , uint32_t increment)
{
	   //Hack....
	   if ( 0xFFFF0000 > (signal->age + increment) )
	   {
	      signal->age += increment;
	   }
}


/********************************************************************************************************************/
/* Application Data read/write Interface																			*/
/********************************************************************************************************************/

/**
 * Returns the current application value of the passed signal instance.
 *
 * @param  PWM_t * const signal: Pointer to the signal object
 * @return pwm_data_t: current value of the application data buffer
 *
 **/
inline PWM_data_t RTE_PWM_get( const PWM_t * const signal )
{                                                                                                                      
   return signal->value;                                                                                               
}                                                                                                                      
                                                                                                                       
                                                                                                                    
                                                                                                                       
/**
 * Sets a new application value to the passed signal instance by passing the new value.
 * The corresponding driver raw value will be calculated immediately by using the default conversion or calling the
 * signal specific scaling function.
 * The signal age will be set to 0 and the new status will be 'RTE_SIGNALSTATUS_VALID'.
 *
 * @param  PWM_t * const signal: Pointer to the signal object
 * @param  pwm_data_t const value: value for the application data buffer
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
inline RC_t RTE_PWM_set(PWM_t * const signal, const PWM_data_t value  )
{

   RC_t ret = RC_SUCCESS;


   //Todo: critical section
    signal->value  = value;
	signal->age    = 0;
	signal->status = RTE_SIGNALSTATUS_VALID;

#ifdef PWM_ASYNCHRONOUS
	SetEvent( tsk_EventDispatcher, signal->pSigCfg->evOnUpdate);
#endif                                                                                                              
   return ret;
}                                                                                                                      
                                                                                                                       
                                                                                                                      
/********************************************************************************************************************/
/* Call functions to the connected drivers																			*/
/********************************************************************************************************************/

                                                                                                                       
/**
 *  Calls the connected driver interface to get a new value.
 *
 * @param PWM_t * const signal: Pointer to the signal object
 * @return  RC_t: Return value of the driver call
 *
 */
inline RC_t RTE_PWM_pullPort( PWM_t * const signal )
{                                                                                                                      
   RC_t ret = RC_SUCCESS;

                                                                                                                       
   if ( NULL != signal->pSigCfg->inDriver )
   {
	  PWM_data_t value;
      ret = signal->pSigCfg->inDriver(&value);
                                                                                                                       
      if ( ret == RC_SUCCESS )                                                                                         
      {
    	  //Critical Section
    	  RTE_PWM_set(signal, value);
      }
      else
      {
    	  signal->status = RTE_SIGNALSTATUS_INVALID;
      }

   }                                                                                                                   
                                                                                                                       
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
/**
 *  Calls the connected driver interfaces to send a value. Value will not be sent if it is INVALID.
 *
 * @param PWM_t * const signal: Pointer to the signal object
 * @return  RC_t: Return value of the driver call
 *
 **/
inline RC_t RTE_PWM_pushPort( const PWM_t * const signal )
{                                                                                                                      
   RC_t ret = RC_ERROR;

   if (RTE_SIGNALSTATUS_VALID == signal->status)
   {
	   ret = signal->pSigCfg->outDriver(&(signal->value));
   }
   return ret;                                                                                                         
}                                                                                                                      

#endif /* include guard */
