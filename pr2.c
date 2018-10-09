/*
 * @author Vecheka Chhourn
 * I have tested both on the virtual machine and on the server site.
 *
 */

#include <stdio.h>
#include <string.h>
#define HEADER_SIZE 54

/* Height and Width of an image.*/
unsigned int height, width;

/*Functions Declarations.*/
void addFileExtension(char *);
void brightenImage(char[], unsigned char thePixels[][width * 3]);
void contrastImage(char[], unsigned char thePixels[][width * 3]);
void rotateImage180(
	char[],
	unsigned char newImage[][width * 3],
	unsigned char thePixels[][width * 3]);
void scaleDownRepeat(char[],  unsigned char thePixels[][width * 3]);


/**
 * Main class to execute the program. 
 **/
int main(void) {
	char fileName[HEADER_SIZE];

	printf("Enter a file name: ");
	scanf("%s", &fileName);
	addFileExtension(fileName);

	printf("Enter height and width (in pixels): ");
	scanf("%d %d", &height, &width);

	FILE *file = fopen(fileName, "rb");

	char header[HEADER_SIZE];
	unsigned char pixels[height][width * 3];
	unsigned char temp[height][width * 3];
	fread(header, 1, HEADER_SIZE, file);
	fread(pixels, 1, height * width * 3, file);

	fclose(file);

	memcpy(temp, pixels, sizeof(pixels));
	brightenImage(header, temp);

	memcpy(temp, pixels, sizeof(pixels));
	contrastImage(header, temp);

	rotateImage180(header, temp, pixels);

	memcpy(temp, pixels, sizeof(pixels));
	scaleDownRepeat(header, temp);

	printf("Done! Check the generated images.\n");

	return 0;
}

/**
 * Brighten up image, and write a copy of the image to a file.
 * @param theHeader image header
 * @param thePixels 2D-arrays of BGR color values
 **/
void brightenImage(char theHeader[], unsigned char thePixels[][width * 3]) {
	int row, col;
	int brightness = 50;
	FILE *outfile = fopen("copy1.bmp", "wb");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width * 3; col += 3) {
		  thePixels[row][col] = thePixels[row][col] + brightness > 255 ? 255: thePixels[row][col] + brightness;
		  thePixels[row][col + 1] = thePixels[row][col + 1] + brightness > 255 ? 255: thePixels[row][col + 1] + brightness;
		  thePixels[row][col + 2] = thePixels[row][col + 2] + brightness > 255 ? 255: thePixels[row][col + 2] + brightness;
		}
	}
	fwrite(theHeader, sizeof(char), HEADER_SIZE, outfile);
	fwrite(thePixels, sizeof(char), height * width * 3, outfile);
	fclose(outfile);
}

/**
 * Constrast image, and write a copy of the image to a file.
 * @param theHeader image header
 * @param thePixels 2D-arrays of BGR color values
 **/
void contrastImage(
	char theHeader[],
	unsigned char thePixels[height][width * 3]) {
	int row, col;
	FILE *outfile = fopen("copy2.bmp", "wb");
	float contrast_ratio = 2.9695;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width * 3; col += 3) {
			int blue = contrast_ratio * (thePixels[row][col] - 128) + 128;
			int green = contrast_ratio * (thePixels[row][col + 1] - 128) + 128;
			int red = contrast_ratio * (thePixels[row][col + 2] - 128) + 128;

			if (blue > 255)
				blue = 255;
			if (red > 255)
				red = 255;
			if (green > 255)
				green = 255;

			if (blue < 0)
				blue = 0;
			if (red < 0)
				red = 0;
			if (green < 0)
				green = 0;

			thePixels[row][col] = blue;
			thePixels[row][col + 1] = green;
			thePixels[row][col + 2] = red;
		}
	}
	fwrite(theHeader, sizeof(char), HEADER_SIZE, outfile);
	fwrite(thePixels, sizeof(char), height * width * 3, outfile);
	fclose(outfile);
}

/**
 * Rotate image 180 degree, and write a copy of the image to a file.
 * @param theHeader image header
 * @param newImage copy of the original image rotated
 * @param thePixels 2D-arrays of BGR color values
 **/
void rotateImage180(
	char theHeader[],
	unsigned char newImage[][width * 3],
	unsigned char thePixels[height][width * 3]) {
	int row, col;
	FILE *outfile = fopen("copy3.bmp", "wb");


	int pixelCol = width * 3 - 1;

	for (col = 0; col < width * 3; col += 3) {
	  int pixelRow = height - 1;
		for (row = 0; row < height; row++) {
			newImage[row][col + 2] = thePixels[pixelRow][pixelCol];
			newImage[row][col + 1] = thePixels[pixelRow][pixelCol - 1];
			newImage[row][col] = thePixels[pixelRow][pixelCol - 2];
			pixelRow--;
		}
		pixelCol -= 3;
	}

	fwrite(theHeader, sizeof(char), HEADER_SIZE, outfile);
	fwrite(newImage, sizeof(char), height * width * 3, outfile);
	fclose(outfile);
}

/**
 * Scale down and repeat an image, and write a copy of the image to a file.
 * @param theHeader image header
 * @param thePixels 2D-arrays of BGR color values
 **/
void scaleDownRepeat(
	char theHeader[],
	unsigned char thePixels[height][width * 3]) {
	  
	unsigned int row, col;
	FILE *outfile = fopen("copy4.bmp", "wb");

  // scaled down the image
	for (row = 0; row < height; row++) {
		for (col = 0; col < width * 3; col += 3) {
			thePixels[row / 2][col / 2] = thePixels[row][col + 1];
			thePixels[row / 2][(col + 1 ) / 2] = thePixels[row][col];
			thePixels[row / 2][(col + 2) / 2] = thePixels[row][col + 2];
		}
	}
	
	
	// copy the scaled down image to the 3 other portions of the image
	int r = height - 1 / 2;
	for (row = 0; row < height - 1 / 2; row++) {
	  for (col = 0; col < width * 3 / 2; col++) {
	    thePixels[r / 2][col] = thePixels[row / 2][col];
	  }
	  r++;
	}

	for (row = 0; row < height - 1 / 2; row++) {
	  int c = width * 3;
	  for (col = 0; col < width * 3; col++) {
	    thePixels[row][c / 2] = thePixels[row][col / 2];
	    c++;
	  }
	  
	}
	
	fwrite(theHeader, sizeof(char), HEADER_SIZE, outfile);
	fwrite(thePixels, sizeof(char), height * width * 3, outfile);
	fclose(outfile);
}

/**
 * Add file extension to the end of the file name.
 * @param theFileName name of the image file
 **/
void addFileExtension(char *theFileName) {
	char ext[5] = ".bmp";
	int i;
	// break when a null is found, and add an extension to the end of the file name
	for (i = 0; i < sizeof(theFileName); i++) {
		if (theFileName[i] == 0) {
			break;
		}
	}

	int j;
	for (j = 0; j < sizeof(ext); j++) {
		theFileName[i] = ext[j];
		i++;
	}
}
