#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMG_WIDTH 	1920
#define IMG_HEIGHT	1080
#define BPP 		2

int stitchFrames (uint8_t** buf1, uint8_t** buf2, uint8_t** buf3, long int size)
{
	int ret = 0;

	*buf3 = (uint8_t*) malloc (sizeof(uint8_t) * (size<<1));
	if (!*buf3) {
		printf ("%s malloc Failure!\n", __func__);
		ret = -1;
		goto endStitch;
	}
#ifdef HOR_STITCH
	memcpy (*buf3, *buf1, size);
	memcpy (*buf3+size, *buf2, size);
#else
	int i = 0;
	uint8_t* ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL;
	ptr1 = *buf1;
	ptr2 = *buf2;
	ptr3 = *buf3;
	for (i = 0; i < IMG_HEIGHT; i++) {
		memcpy (ptr3, ptr1, IMG_WIDTH * BPP);
		ptr1 = ptr1 + (IMG_WIDTH * BPP);
		ptr3 = ptr3 + (IMG_WIDTH * BPP);
		memcpy (ptr3, ptr2, IMG_WIDTH * BPP);
		ptr2 = ptr2 + (IMG_WIDTH * BPP);
		ptr3 = ptr3 + (IMG_WIDTH * BPP);
	}
#endif

endStitch:
	if (!*buf3) { free(*buf3); *buf3 = NULL; }
	return ret;
}
