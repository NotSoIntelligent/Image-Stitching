/* Program to stitch 2 YUV422 UYVY frame Vertically */

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include "stitch.h"

/* Macros */
#define OP_FILE_NAME "stitchedOutput.yuv"

/* Function Declarations */

int getImgFromFile (uint8_t** buf, char* filename, long int* fsize);
int writeToFile (uint8_t* buf, char* filename, long int fsize);
int processArgs (int argc, char *argv[], char **filename1, char **filename2);
void printUsage ();

int main (int argc, char *argv[])
{
	char* filename1 = NULL, * filename2 = NULL;

	uint8_t* buf1 = NULL, * buf2 = NULL, * buf3 = NULL;

	long int fsize1 = 0, fsize2 = 0;

	int ret = 0;

	/* Process CMD Line arguments and get Filename */
	ret = processArgs(argc, argv, &filename1, &filename2);
	if (ret) {
		printf ("Error in CMDLINE arguments\n");
		goto deinit;
	}

	/*get Image Buffers from File */
	ret = getImgFromFile (&buf1, filename1, &fsize1);
	if (ret) {
		printf ("Error in reading File : %s\n", filename1);
		goto deinit;
	}
	ret = getImgFromFile (&buf2, filename2, &fsize2);
	if (ret) {
		printf ("Error in reading File : %s\n", filename2);
		goto deinit;
	}

	/* basic check if Image sizes are equal (No brainer check) */
	if (fsize1 != fsize2) {
		ret = -1;
		printf ("\n%s and %s are of not same size!\n", filename1, filename2);
		goto deinit;
	}

	/* F A K E S T I T C H ? */
	ret = stitchFrames (&buf1, &buf2, &buf3, fsize1);
	if (ret) {
		printf ("Error in stitching\n");
		goto deinit;
	}

	/* Write buffer to file */
	ret = writeToFile (buf3, OP_FILE_NAME, fsize1 * 2);
	if (ret) {
		printf ("Error in Writing File : %s\n", OP_FILE_NAME);
		goto deinit;
	}


deinit:
	printf("\n== DONE ==\n");
	if (!buf1) {free(buf1); buf1 = NULL;}	
	if (!buf2) {free(buf2); buf2 = NULL;}	
	if (!buf3) {free(buf3); buf3 = NULL;}	
	return ret;
}

int processArgs (int argc, char *argv[], char **filename1, char **filename2)
{
	if (argc != 3) {
		printUsage();
		return -1;
	} else {
		*filename1 = argv[1];
		*filename2 = argv[2];
		return 0;
	}
}

void printUsage ()
{
	printf ("Program to stitch 2 YUV422 UYVY frame Vertically\n");
	printf ("USAGE: ./fakeStitch <filename1> <filename2>\n");
}

int getImgFromFile (uint8_t** buf, char* filename, long int* fsize)
{
	FILE *fp = NULL;
	int ret = 0;

	fp = fopen (filename, "rb");
	if (fp == NULL) {
		printf ("%s File Open error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

	fseek (fp, 0, SEEK_END);
	*fsize = ftell(fp);
	rewind (fp);

	printf ("Reading File [%s] : %ld bytes\n", filename, *fsize);

	*buf = (uint8_t*) malloc (sizeof(uint8_t) * (*fsize));
	if (!*buf) {
		printf ("%s malloc Failure!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

	if (fread (*buf, *fsize, 1, fp) != 1) {
		printf ("%s File read error!\n", __func__);
		ret = -1;
		goto endGetImage;
	}

endGetImage:
	if (!fp)   { fclose(fp); fp = NULL; }
	if (!*buf) { free(*buf); *buf = NULL; }
	return ret;
}

int writeToFile (uint8_t* buf, char* filename, long int fsize)
{
	FILE *fp = NULL;
	int ret = 0;

	fp = fopen (filename, "wb");
	if (!fp) {
		printf ("%s File Open error!\n", __func__);
		ret = -1;
		goto endSetImage;
	}

	if (fwrite (buf, 1, fsize, fp) != fsize) {
		printf ("%s File Write error!\n", __func__);
		ret = -1;
		goto endSetImage;
	}


endSetImage:
	if (!fp) { fclose(fp); fp = NULL; }
	return ret;
}
