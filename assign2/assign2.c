/*
 * The main code file for CMPSC 311 assignment 2.
 *
 * Implement your main function here.  See the assignment for full instructions.
 * Wherever there is a ____, you need to fill in one or more things.
 */


#include <stdio.h>
#include "a2lib.h"
#define NUM_INPUTS 15


/*
 * Main function for the "assign2" program.  This function reads in an array of
 * numbers and does a variety of processing on it.
 *
 * Parameters: none
 *
 * Return value: 0 if successful, 1 otherwise
 */
int main(void)
{
	// Declare your variables here
	double inputs[NUM_INPUTS];
	double averageArrayValue = 0;
	int i;
	int roundedInputs[NUM_INPUTS];
	int parityInputs[NUM_INPUTS];

	// Read double-precision values into an array from standard input
	for (i = 0; i < NUM_INPUTS; i++) 
	{
		scanf("%lf", &inputs[i]);
	}

	// Display the inputs in a nice format
	printf("Input values:\n");
	print_array_dbl(inputs);

	//----------------------------------------------------------------------------------------
	
	// Calculate and display the average of the elements in the array
	averageArrayValue = array_average(inputs);
	printf("The average of the inputs is %.2f.\n", averageArrayValue);

	//----------------------------------------------------------------------------------------
	
	// Use the reduce_value_by function to subtract the average value from
	// each of the elements in the array (this will center the values around
	// zero).
	//double avgValue = averageArrayValue;
	for (i = 0; i < NUM_INPUTS; i++) 
	{
		inputs[i] = reduce_value_by(averageArrayValue, inputs[i]);
	}
	
	// Display the recentered array
	printf("Recentered input values:\n");
	print_array_dbl(inputs);


	//----------------------------------------------------------------------------------------

	
	// Compute the integer array.  Round each element up (away from zero) if
	// its decimal part is >= .75, down otherwise.
	for (i = 0; i < NUM_INPUTS; i++) 
	{
		roundedInputs[i] = round75(inputs[i]);
	} 

	// Display the rounded values
	printf("Rounded integers:\n");
	print_array_int(roundedInputs);

	
	//----------------------------------------------------------------------------------------


	// Attempt to reverse the digits of each value in the integer array
	
	for (i = 0; i < NUM_INPUTS; i++)
	{
		reverse_int(&roundedInputs[i]);
	}

	// And display the results
	printf("Reversed digits:\n");
	print_array_int(roundedInputs);


	//----------------------------------------------------------------------------------------


	// Move all the even numbers to the front of the integer array
	parity_sort(parityInputs, roundedInputs);

	// Display the reordered array
	printf("After parity sort:\n");
	print_array_int(parityInputs);


	//----------------------------------------------------------------------------------------
	
	
	// Display a vertical sum
	printf("Sum of values:\n");
	print_sum(parityInputs);
	
	// Program finished successfully
	return 0;
}
