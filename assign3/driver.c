/*
 * Source file for the MultiTron driver.  This is where you should implement the
 * user-facing API (mtron_*) for the MultiTron, using the tronctl() hardware
 * interface to communicate with the device.
 */
#include <stdio.h>

#include "mtron.h"
#include "driver.h"

/*
* This is the init function.
* It turns on the multitron and 
* recieves a code for determining
* the number of displays
*/
int mtron_init(struct multitron *mtron)
{
	//need a variable to hold the code
	//which will recieve instructions
	//from the multitron
	uint16_t getCode = 0x0;
	tronctl(MTRON_POWERON, &getCode);

	//doing bit-wise operations on the code
	//to extract the number of displays on
	//the multitron
	uint16_t displayRows = getCode << 8;
	displayRows = displayRows >> 8;

	uint16_t displayCols = getCode >> 8;
	

	mtron->maxDisplayColumns = displayCols;
	mtron->maxDisplayRows = displayRows;

	//It is possible to calculate the maximum number of rows
	//and columns from the number of displays
	//They are stored for later use
	mtron->maxRowNumber = ((mtron->maxDisplayRows * 128) - 1);
	mtron->maxColumnNumber = ((mtron->maxDisplayColumns * 256) - 1);

	return 0;
}


/*
* This is the destroy function.
* It turns off the multitron
*/
void mtron_destroy(struct multitron *mtron)
{
	//Passing in the code to turn off the multitron
	//The number below is the correct bit number in decimal form
	//I just calculated it by hand, since it is the same each time.
	mtron->op = 3968;
	uint16_t blank = 0x0;
	tronctl(mtron->op, &blank);
}

/*
* This is the getPixel function
* it receives information about a pixel color
* and stores this color code into the pointer color
*/

int mtron_getpixel(struct multitron *mtron, uint32_t x, uint32_t y,
		uint8_t *color)
{
	//need an array to recieve information
	uint8_t pixelInfo[256];

	//used to determine which screen is active
	int row = (y / 128);
	int column = (x / 256);

	//Storing some variables in the state
	mtron->currentRow = y;
	mtron->currentColumn = x;
	mtron->colorDepth = *color;

	//figureing out which is the current display that we
	//are working with
	mtron->display = ((mtron->maxDisplayColumns * row) + (column)); 

	//Checking bounds
	if(y > mtron->maxRowNumber || x > mtron->maxColumnNumber)
	{
		return 1; //fails since current pixel is out of bounds
	}

	//make sure pixels fit in a single screen
	uint32_t currentDisplayRow = y - (row * 128);
	uint32_t currentDisplayColumn = x -(column * 256);

	//Forming an op-code with bit-wise operations
	mtron->op = 0x0;
	uint32_t blank = 0x0;
	mtron->op = (mtron->op | (currentDisplayRow << 25));
	mtron->op = (mtron->op | (blank << 13));
	mtron->op = (mtron->op | (MTRON_READ_LINE << 7));
	mtron->op = (mtron->op | mtron->display);

	tronctl(mtron->op, pixelInfo);

	//Storing the correct color
	*color = pixelInfo[currentDisplayColumn];
	
	return 0;
}

/*
*This is the putPixel function
*It recieves info on what pixel needs to be changed
*and then modifies that pixel with the appropriate color code
*and then returns the information back to the multitron
*/

int mtron_putpixel(struct multitron *mtron, uint32_t x, uint32_t y,
		uint8_t color)
{
	//need an array to recieve information
	uint8_t pixelInfo[256];

	//used to determine which screen is active
	int row = (y / 128);
	int column = (x / 256);

	//Storing some variables in the state
	mtron->currentRow = y;
	mtron->currentColumn = x;
	mtron->colorDepth = color;

	//figureing out which is the current display that we
	//are working with
	mtron->display = ((mtron->maxDisplayColumns * row) + (column));

	//Checking bounds
	if(y > mtron->maxRowNumber || x > mtron->maxColumnNumber)
	{
		return 1; //fails since current pixel is out of bounds
	}

	//make sure pixels fit in a single screen
	uint32_t currentDisplayRow = y - (row * 128);
	uint32_t currentDisplayColumn = x -(column * 256);

	//Forming an op-code with bit-wise operations
	mtron->op = 0x0;
	uint32_t blank = 0x0;
	mtron->op = (mtron->op | (currentDisplayRow << 25));
	mtron->op = (mtron->op | (blank << 13));
	mtron->op = (mtron->op | (MTRON_READ_LINE << 7));
	mtron->op = (mtron->op | mtron->display);

	tronctl(mtron->op, pixelInfo);

	//Storing the correct color in the pixel
	pixelInfo[currentDisplayColumn] = color;

	//Forming an op-code with bit-wise operations
	mtron->op = 0x0;
	blank = 0x0;
	mtron->op = (mtron->op | (y << 25));
	mtron->op = (mtron->op | (blank << 13));
	mtron->op = (mtron->op | (MTRON_WRITE_LINE << 7));
	mtron->op = (mtron->op | mtron->display);

	//returning the pixel with the correct color info
	tronctl(mtron->op, pixelInfo);
	return 0;
}

/*
*This is the getRect function
*it recieves info about a certain "square" of pixels
*and stores these pixels (left to right, then top to bottom)
*in the data array;
*/

int mtron_getrect(struct multitron *mtron, uint32_t x, uint32_t y,
		uint32_t w, uint32_t h, uint8_t *data)
{
	//need an array to recieve information
	uint8_t pixelSquare[256];

	//Some counting variables for "for" loops
	int i = 0;
	int k = 0;
	int z = 0;
	int kount = 0;

	//used to determine which screen is active
	int row = (y / 128);
	int column = (x / 256);

	//Storing information in the state
	mtron->currentRow = y;
	mtron->currentColumn = x;

	//figureing out which is the current display that we
	//are working with
	mtron->display = ((mtron->maxDisplayColumns * row) + (column));

	//Cheching bounds
	if(y > mtron->maxRowNumber || x > mtron->maxColumnNumber 
		|| y + h > mtron->maxRowNumber || x + w > mtron->maxColumnNumber)
	{
		return 1; //fails since current pixel or last pixel is out of bounds
	}

	//make sure pixels fit in a single screen
	uint32_t currentDisplayRow = y - (row * 128);
	uint32_t currentDisplayColumn = x - (column * 256);

	//Another variable to help with the following loop
	//Helps with determing if the pixel is in the next screen
	int width = ((currentDisplayColumn + w) - 256);

	//This for loop goes until all rows are handled
	for(i = currentDisplayRow; i < (h + currentDisplayRow); i++)
	{
		//Forming an op-code with bit-wise operations
		mtron->op = 0x0;
		uint32_t blank = 0x0;
		mtron->op = (mtron->op | (i << 25));
		mtron->op = (mtron->op | (blank << 13));
		mtron->op = (mtron->op | (MTRON_READ_LINE << 7));
		mtron->op = (mtron->op | mtron->display);

		tronctl(mtron->op, pixelSquare);

		//This section attempts to determine if pixels go out of the current screen
		//and into the next one, but does not fully work
		//Only works if entire "square" of pixels is in one screen
		//partial credit would be awesome
		if(z==0)
		{	
			//This is the part that works.
			//It just moves the correct pixel info
			//into the correct spot in the data pointer
			for(k = currentDisplayColumn; k < (currentDisplayColumn + w); k++)
			{
				data[kount] = pixelSquare[k];
				kount++;
				if(k==255)
				{
					mtron->display++;
					i--;
					z = 1;
					break;
				}
			}//end for
		}//end if
		else if (z==1)
		{
			for(k = 0; k < width; k++)
			{
				data[kount] = pixelSquare[k];
				kount++;
				if(k == ((currentDisplayColumn + w) - 256))
				{
					mtron->display--;
					z = 0;
					break;
				}
			}//end for
		}//end else if
		if(i == 127 && z == 1)
		{
			mtron->display += mtron->maxDisplayColumns - 1;
		}
		else if(i == 127 && z == 0)
		{
			mtron->display += mtron->maxDisplayColumns;
		}
	}
	return 0;
}

/*
*This is the putRect function
*It recieves info on a certain "square" of pixels that need to be changed
*it then populates those with the appropriate pixels from the array data
*and then passes in the new "square," which is filled in with the correct data
*/
int mtron_putrect(struct multitron *mtron, uint32_t x, uint32_t y,
		uint32_t w, uint32_t h, uint8_t *data)
{
	//need an array to recieve information
	uint8_t pixelSquare[256];

	//Some counting variables for "for" loops
	int i = 0;
	int k = 0;
	int kount = 0;

	//used to determine which screen is active
	int row = (y / 128);
	int column = (x / 256);

	//Storing information in the state
	mtron->currentRow = y;
	mtron->currentColumn = x;

	//figureing out which is the current display that we
	//are working with
	mtron->display = ((mtron->maxDisplayColumns * row) + (column));

	//Checking bounds	
	if(y > mtron->maxRowNumber || x > mtron->maxColumnNumber 
		|| y + h > mtron->maxRowNumber || x + w > mtron->maxColumnNumber)
	{
		return 1; //fails since current pixel or last pixel is out of bounds
	}

	//make sure pixels fit in a single screen
	uint32_t currentDisplayRow = y - (row * 128);
	uint32_t currentDisplayColumn = x - (column * 256);

	//Initialize variables
	i = 0;
	k = 0;
	kount = 0;
	
	//The following for loop goes until all rows are covered
	for(i = currentDisplayRow; i < (h + currentDisplayRow); i++)
	{
		
		//Forming an op-code with bit-wise operations
		mtron->op = 0x0;
		uint32_t blank = 0x0;
		mtron->op = (mtron->op | (i << 25));
		mtron->op = (mtron->op | (blank << 13));
		mtron->op = (mtron->op | (MTRON_READ_LINE << 7));
		mtron->op = (mtron->op | mtron->display);

		tronctl(mtron->op, pixelSquare);

		//putting the correct pixes into the correct location
		for(k = currentDisplayColumn; (k < currentDisplayColumn + w); k++)
		{
				pixelSquare[k] = data[kount];
				kount++;
		}

		//Forming an op-code with bit-wise operations
		mtron->op = 0x0;
		blank = 0x0;
		mtron->op = (mtron->op | (i << 25));
		mtron->op = (mtron->op | (blank << 13));
		mtron->op = (mtron->op | (MTRON_WRITE_LINE << 7));
		mtron->op = (mtron->op | mtron->display);

		//sending back the completed square
		tronctl(mtron->op, pixelSquare);

	}
	return 0;
}