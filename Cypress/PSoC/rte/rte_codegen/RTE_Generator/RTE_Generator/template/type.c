/*
 * $Name/$_type.c
 *
 * Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm
 */


#include "project.h"
#include "global.h"

#include "$Name/$_type.h"

/*****************************************************************************************
 *************** Port Wrapper Implementation for $NAME/$ signal                     **************************
 *****************************************************************************************/


/**
 * Default IN driver API - may be deleted if not required
 */
inline RC_t $NAME/$_driverIn($NAME/$_data_t *const data)
{
#error "Add your code here"

	//Read data from the MCAL driver

	//Scale it to the application type

	return RC_SUCCESS;
}

/**
 * Default IN driver API - may be deleted if not required
 */
inline RC_t $NAME/$_driverOut(const $NAME/$_data_t  *const data)
{
#error "Add your code here"

	//Scale application data to drive format

	//Write scaled data to driver

	return RC_SUCCESS;
}