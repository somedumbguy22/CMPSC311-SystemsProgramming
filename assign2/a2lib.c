#define NUM_INPUTS 15
#include "a2lib.h"
#include <stdio.h>

//testing some stuff by adding this comments
/*
* This is one of two print functions in a2lib.c
* It prints out an array of double type elements
* in square brackets, where each element is seperated
* by a comma
* 
* Parameters: the double type array
*
* Return value: None, the array is manipulated directly
*/
void print_array_dbl(double *inputs)
{
	int k = 0;
	printf("[");

	for(k = 0; k < NUM_INPUTS; k++)
	{
		printf("%.2f", inputs[k]);
		if(k < NUM_INPUTS - 1)
		{
			printf(", ");
		}
	}

	printf("]\n");
}

/*
* This function takes a double type array
* and calcuates the average of all the elements
* 
* Parameters: the double type array
*
* Return value: it returns a double type value
* which is the average of all elements in the array
*/
double array_average(double *inputs)
{
	double average = 0;
	double sum = 0;
	int k = 0;
	for(k = 0; k < NUM_INPUTS; k++)
	{
		sum = sum + inputs[k];
	}

	average = sum / NUM_INPUTS;
	return (average);
}

/*
* This function subtracts the double type value 
* from a different double type value
* It modifies the original value
*
* Parameters: a double type value and a double type array
*
* Return value: the modified value
*/
double reduce_value_by(double averageArrayValue, double input)
{
	input = input - averageArrayValue;
	return input;
}

/*
* This function takes a double type value and rounds the number
* according to its decimal place. The round takes place when the 
* decimal is equal to or greater than 0.75
*
* Parameters: a double type number
*
* Return value: the rounded double type value
*/
int round75(double input)
{
	double trunk = 0;

	if (input >= 0)
	{
		trunk = input - (int)input;
		if(trunk >=.75)
		{
			return ((int)input + 1);
		}
		else
		{
			return ((int)input);
		}
	}
	else
	{ 
		trunk = ((-1) * (input - (int)input));

		if(trunk >=.75)
		{
			return ((int)input - 1);
		}
		else
		{
			return ((int)input);
		}

	}
}

/*
* This is the second of two print functions in a2lib.c
* It prints out an array of integer type elements
* in square brackets, where each element is seperated
* by a comma
* 
* Parameters: the integer type array
*
* Return value: None, the array is manipulated directly
*/
void print_array_int(int *roundedInputs)
{
	int k = 0;
	printf("[");
	for(k = 0; k < NUM_INPUTS; k++)
	{
		printf("%d", roundedInputs[k]);
		if(k < NUM_INPUTS - 1)
		{
			printf(", ");
		}
	}
	printf("]\n");
}

/*
* This function takes a pointer to a integer value
* and reverses each digit. If the number is negative,
* nothing is done to the element.
*
* Parameters: a pointer to a integer value
*
* Return value: 0 if the number is rounded
* 				1 if the number is not modified (negative values)
*/
int reverse_int(int *element)
{
		int reversedElement = 0;

		if(*element > 0)
		{
			while(*element > 0)
			{
				reversedElement = ((reversedElement * 10) + (*element % 10));
				*element = *element / 10 ;
			}
			*element = reversedElement;
			return 0;
		}
		else
		{
			
			return 1;
		}
}

/*
* This function takes two arrays. One is an array to be sorted,
* and the other is a target array for the sorted values to be
* inserted.
*
* Parameters: Two arrays. One to be sorted and the other a target
*
* Return value: None, the arrays are modified directly
*/
void parity_sort(int *parityInputs, int *reversedInputs)
{
	//three counting variables and a 
	//temp array for storage of non even numbers
	int i = 0;
	int k = 0;
	int j = 0;
	int tempArray[NUM_INPUTS];

	for (i = 0; i < NUM_INPUTS; i++)
	{
		if((reversedInputs[i] % 2) == 0)
		{
			parityInputs[k] = reversedInputs[i];
			k++;
		}
		else
		{
			tempArray[j] = reversedInputs[i];
			j++;
		}
	}

	for(i = 0; i < j ; i++)
	{
		parityInputs[k] = tempArray[i];
		k++;
	}
}

/*
* This function returns the length (of characters)
* in an integer value
*
* Parameters: a copy of the integer
*
* Return value: the length of said integer
*/
int get_num_length(int element)
{
	int length = 0;
	
	if(element > 0)
	{
		while (element > 0)
		{
			element = element / 10;
			length++;
		}
	}
	else
	{
		while (element < 0)
		{
			element = element / 10;
			length++;
		}
		//adding an extra length value for the negative
		length++;
	}

	return (length);
}

/*
* This function prints (in a special format) the values
* of an array, and then displays the sum of the values
*
* Parameters: The array to be displayed
*
* Return value: Nothing, this function displays to the screen
*/
void print_sum(int *parityInputs)
{
	int i = 0;
	int k = 0;
	int sum = 0;

	int maxLength = get_num_length(parityInputs[0]);

	for(i = 0; i < NUM_INPUTS; i++)
		if(get_num_length(parityInputs[i]) > maxLength)
		{
			maxLength = get_num_length(parityInputs[i]);
		}
	 
	
	for(i = 0; i < NUM_INPUTS; i++)
	{
		if(i == NUM_INPUTS - 1)
		{
			printf("+");
			maxLength--;
		}

		for(k = 0; k < maxLength - get_num_length(parityInputs[i]) + 2; k++)
		{
			printf(" ");
		}

		printf("%d\n", parityInputs[i]);
	}

	for(i = 0; i < maxLength + 3; i++)
	{
		printf("-");
	}
	
	for(i = 0; i < NUM_INPUTS; i++)
	{
		sum = sum + parityInputs[i];
	}
	printf("\n");

	for(i = 0; i < maxLength - get_num_length(parityInputs[i]) + 3; i++)
	{
		printf(" ");
	}
	printf("%d\n", sum);
}
