// This is needed for certain functions to be available in #includes
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "driver.h"
#include "image.h"

/*
 * Opens the given file, reads the image data, and uses it to initialize an
 * image structure.  See the assignment writeup for a description of the 3II
 * image file format.
 */
int image_init(struct image *img, const char *fname)
{
	/*
	*
	*Some variables for various calculations throughout
	*the function. Their use should be know from their
	*name
	*
	*/

	uint32_t magic = 0;
	uint16_t width = 0;
	uint16_t height = 0;
	uint32_t imageOffset = 0;
	uint32_t stringOffset = 0;
	uint16_t stringLength = 0;
	uint16_t fMask = 0;

	uint16_t imageSize;


	//3 variables for opening the file with 
	//correct flags and mode
	int myFile;
	mode_t mode = 0;
	int flags = O_RDONLY;

	//these variables are useful for making sure 
	//that pread works correctly
	uint16_t offset;
	uint16_t numByte;
	int bytesRead;

	//a variable to ensure the closing of files
	//works properly
	int closeFile;


	//////////////////////Opening File////////////////////
	myFile = open(fname, flags, mode);
	if(myFile < 0)
	{
		perror("Open(Opening File Error)");
		return 1;
	}
	///////////////////////////////////////////////////////

	/*
	*
	*The next few sections will be optaining information from
	*the image header. These are the magic number, width, height,
	*imgage data, image comment, and image mask.
	*
	*In each of these sections, an offset and the number of bytes
	*to read will be set and then a pread will be done.
	*After, if statements will ensure that the read was done
	*properly.
	*
	*/

	//////////////////////Checking Magic Numbers///////////

	/*
	*
	*In this section, magic numbers are checked to ensure the
	*the file type is type 3ii 
	*
	*/

	offset = 0;
	numByte = 4;

	bytesRead = pread(myFile, &magic, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Magic Number)");
		return 1;
	}

	magic = ntohl(magic);
	if(magic != 0x43530311)
	{
		printf("Error: Magic Number is incorrect\n");
		return 1;
	}

	///////////////////////////////////////////////////////

	//////////////////////Getting Width////////////////////

	/*
	*
	*Here, the code is grabbing the width information
	*from the file header and storing it in the struct
	*
	*/

	offset = 4;
	numByte = 2;

	bytesRead = pread(myFile, &width, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Width Error)");
		return 1;
	}

	width = ntohs(width);
	img->w = width;
	///////////////////////////////////////////////////////

	//////////////////////Getting Height////////////////////

	/*
	*
	*Here the code is grabbing the height information from
	*the image header and stroring it in the struct
	*
	*/

	numByte = 4;
	offset = 6;
	bytesRead = pread(myFile, &height, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Height Error)");
		return 1;
	}

	height = ntohs(height);
	img->h = height;
	///////////////////////////////////////////////////////

	//////////////////////File Information////////////////////

	/*
	*
	*Here the code is obtaining the data offset from the image
	*header, and then parsing the file for the actual image data
	*and storing all of this into the struct.
	*
	*/

	imageSize = width * height;
	img->data = malloc(sizeof(uint8_t) * imageSize);//allocating size for the data

	offset = 8;
	numByte = 4;

	bytesRead = pread(myFile, &imageOffset, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Data Offset Error)");
		return 1;
	}

	imageOffset = ntohl(imageOffset);



	bytesRead = pread(myFile, img->data, imageSize, imageOffset);
	if(bytesRead != imageSize)
	{
		perror("Read(Image Data Error)");
		return 1;
	}

	
	///////////////////////////////////////////////////////

	//////////////////////Comment Information////////////////////

	/*
	*
	*Here the code is getting the comment offset from the image
	*header and then parsing the image file for the actual comment.
	*It then stores this infomation into the struct.
	*
	*/
	
	offset = 12;
	numByte = 4;

	bytesRead = pread(myFile, &stringOffset, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Comment Offset Error)");
		return 1;
	}

	stringOffset = ntohl(stringOffset);

	
	offset = 16;
	numByte = 2;

	bytesRead = pread(myFile, &stringLength, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(String Length Error)");
		return 1;
	}

	stringLength = ntohs(stringLength);

	img->comment = malloc((sizeof(char) * stringLength) + 1);//allocating size for 
															 //comment + 1 for the 
															 //the null terminator



	bytesRead = pread(myFile, img->comment, stringLength, stringOffset);
	if(bytesRead != stringLength)
	{
		perror("Read(Comment Data Error)");
		return 1;
	}

	/*
	*
	*The 3ii file format does not implicitly add a null terminator
	*so this code adds one to the image comment
	*
	*/

	img->comment[stringLength] = '\0';

	///////////////////////////////////////////////////////

	//////////////////////Mask Information////////////////////

	/*
	*
	*This section obtains the Feature Bits for the image mask, and
	*stores this information into the image struct
	*
	*/

	img->mask = malloc(sizeof(uint8_t) * imageSize);//allocating size for the image mask

	offset = 18;
	numByte = 2;

	bytesRead = pread(myFile, &fMask, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Read(Mask Offset Error)");
		return 1;
	}

	fMask = ntohs(fMask);

	/*
	*
	*Checking to make sure the feature mask is in the correct spot
	*and adding one if nothing is there
	*
	*/

	if((fMask & FEATURE_MASK) == FEATURE_MASK)
	{
		bytesRead = pread(myFile, img->mask, imageSize, (imageOffset + imageSize));
		if(bytesRead != imageSize)
		{
			perror("Read(Mask Data Error)");
			return 1;
		}
	}
	else
	{
		memset(img->mask, 0xFF, imageSize);
	}

	///////////////////////////////////////////////////////

	//////////////////////Closing File////////////////////

	/*
	*
	* This code closes the file
	*
	*/

	closeFile = close(myFile);
	if(closeFile != 0)
	{
		printf("Error: File did not close properly!\n");
		return 1;
	}
	else
	{
		myFile = -1;
	}
	///////////////////////////////////////////////////////	
	return 0;
}

/*
 * Destroys an image structure.  If any resources were allocated in image_init,
 * they should be released here.
 */
void image_destroy(struct image *img)
{

	/*
	*
	*This function just frees all allocated data
	*and sets all the pointers to null
	*
	*/

	free(img->data);
	free(img->comment);
	free(img->mask);
	img->data = NULL;
	img->comment = NULL;
	img->mask = NULL;

}

/*
 * Saves the image to the given file, using the 3II file format.  This should
 * overwrite and truncate the file if it already exists.
 */
int image_save(const struct image *img, const char *fname)
{

	/*
	*
	*Some variables for various calculations throughout
	*the function. Their use should be know from their
	*name
	*
	*/

	uint32_t magic = 0;
	uint16_t width = 0;
	uint16_t height = 0;
	uint32_t imageOffset = 0;
	uint32_t stringOffset = 0;
	uint16_t stringLength = 0;
	uint16_t fMask = 0;

	//These variables exist so I don't have to
	//re nothx() the variables after using them
	uint32_t imageOff = 0;
	uint32_t stringOff = 0;
	uint16_t fMaskOff = 0;

	uint16_t imageSize = 0;;

	//these variables are used to calculated offsets
	//and the number of bytes to read
	//as well as check to make sure pwrite worked
	//properly
	uint16_t offset;
	uint16_t numByte;
	int bytesRead;

	//This variables are for opening the file with the
	//appropriate flags and mode
	int myFile;
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP;
	int flags = O_WRONLY|O_CREAT|O_TRUNC;
	int closeFile;

	//////////////////////Opening File////////////////////

	/*
	*
	*This code opens the file and make sure it
	*opens correctly
	*
	*/

	myFile = open(fname, flags, mode);
	if(myFile < 0)
	{
		perror("open");
		return 1;
	}
	///////////////////////////////////////////////////////

	///////////////Doing Save Stuff////////////////////////

	/*
	*
	*This code sets the magic number of the 3ii format
	*to the new file
	*
	*/

	magic = htonl(0x43530311);
	numByte = 4;
	offset = 0;
	bytesRead = pwrite(myFile, &magic, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("Write(Magic Number)");
		return 1;
	}

	/*
	*
	*This code sets the correct width information
	*to the new file
	*
	*/

	width = htons(img->w);
	numByte = 2;
	offset = 4;
	bytesRead = pwrite(myFile, &width, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This sets the correct height infomrmation
	*to the new file
	*
	*/

	height = htons(img->h);
	numByte = 2;
	offset = 6;
	bytesRead = pwrite(myFile, &height, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	//setting the string length
	stringLength =  strlen(img->comment);

	/*
	*
	*This section calculates the image data offset
	*and writes that to the file header
	*
	*/
	
	numByte = 4;
	offset = 8;
	imageOffset = 20;
	imageOff = imageOffset;
	imageOffset = htonl(imageOffset);

	bytesRead = pwrite(myFile, &imageOffset, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section calculates the mask offset and 
	*writes that to the image file header
	*
	*/

	numByte = 2;
	offset = 18;
	imageSize = img->w * img->h;
	fMask = imageOff + imageSize;
	fMaskOff = fMask;
	fMask = htons(fMask);
	bytesRead = pwrite(myFile, &fMask, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section calculates the string offset and 
	*writes it into the image header
	*
	*/

	numByte = 4;
	offset = 12;
	stringOffset = fMaskOff + imageSize;
	stringOff = stringOffset;
	stringOffset = htonl(stringOffset);
	bytesRead = pwrite(myFile, &stringOffset, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section writes the comment lenght into the 
	*file header
	*
	*/

	stringLength = htons(stringLength);
	numByte = 2;
	offset = 16;
	bytesRead = pwrite(myFile, &stringLength, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section writes the actual image data into
	*the file
	*
	*/

	numByte = imageSize;
	offset = imageOff;
	bytesRead = pwrite(myFile, img->data, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section writes the image mask
	*into the file header
	*
	*/

	numByte = imageSize;
	offset = fMaskOff;
	bytesRead = pwrite(myFile, img->mask, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}

	/*
	*
	*This section writes the comment
	*into the image file
	*
	*/

	numByte = ntohs(stringLength);
	offset = stringOff;
	bytesRead = pwrite(myFile, img->comment, numByte, offset);
	if(bytesRead != numByte)
	{
		perror("write");
		return 1;
	}
	///////////////////////////////////////////////////////
	
	//////////////////////Closing File////////////////////

	/*
	*
	*This section closes the file and make sure it
	*closes properly
	*
	*/

	closeFile = close(myFile);
	if(closeFile != 0)
	{
		printf("Error: File did not close properly!\n");
		return 1;
	}
	else
	{
		myFile = -1;
	}
	///////////////////////////////////////////////////////
	return 0;
}

/*
 * Draws an image onto the MultiTron, using the appropriate bitwise masking
 * operations.  Bits which are 1 in the mask should have the corresponding data
 * drawn, and bits which are 0 in the mask should be left as they were.
 */
int image_draw(const struct image *img, struct multitron *mtron, int x, int y)
{	
	//some misc. variables for calculations
	int kount = 0;
	int functionCheck = 0;

	//setting the image size
	uint16_t imageSize = (img->w * img->h);

	//allocation data for reading and writing data
	uint8_t *oldData = malloc(imageSize * sizeof(uint8_t));
	uint8_t *newData = malloc(imageSize * sizeof(uint8_t));

	//getting data
	functionCheck =  mtron_getrect(mtron, x, y, img->w, img->h, oldData);
	if(functionCheck != 0)
	{
		perror("GetRect Fail");
		return 1;
	}

	//Putting the data into a new data variable with the image mask taken into account
	kount = 0;
	for(kount = 0; kount < imageSize; kount++)
	{
		newData[kount] = ((oldData[kount] & ~(img->mask[kount])) | (img->data[kount] & img->mask[kount]));
	}
	
	//putting the new data on the mtron
	functionCheck = mtron_putrect(mtron, x, y, img->w, img->h, newData);
	if(functionCheck != 0)
	{
		perror("PutRect Fail");
		return 1;
	}
	

	//freeing memory
	free(oldData);
	free(newData);
	oldData = NULL;
	newData = NULL;

	return 0;
}
/*
 * BONUS
 * Retrieves pixel data from the MultiTron into an existing image structure.
 * Only retrieves the bits which are part of the mask; the rest of the bits
 * in the data should be 0.
 */
int image_get(struct image *img, struct multitron *mtron, int x, int y)
{	
	//some misc. variables for calculations
	int kount = 0;
	int functionCheck = 0;

	//setting the image size
	uint16_t imageSize = (img->w * img->h);

	//allocation data for reading and writing data
	uint8_t *data = malloc(imageSize * sizeof(uint8_t));

	//getting data
	functionCheck =  mtron_getrect(mtron, x, y, img->w, img->h, data);
	if(functionCheck != 0)
	{
		perror("GetRect Fail");
		return 1;
	}

	//Putting the data into a new data variable with the image mask taken into account
	kount = 0;
	for(kount = 0; kount < imageSize; kount++)
	{
		data[kount] = (data[kount] & img->mask[kount]);
	}

	//storing the data
	img->data = data;

	//freeing allocated memory
	free(data);
	data = NULL;

	return 0;
}