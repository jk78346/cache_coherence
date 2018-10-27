#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/types.h>

#include "bitmap.h"

struct Bitmap* newBitmap( __u32 size){


	
        struct Bitmap* bitmap = (struct Bitmap*) malloc( sizeof(struct Bitmap));
        bitmap->bitarray = (__u32*) malloc(sizeof(__u32)*((size+kBitsPerWord - 1)/kBitsPerWord));
	

      
    memset(bitmap->bitarray, 0, (sizeof(__u32)*((size+kBitsPerWord - 1)/kBitsPerWord)));
	bitmap->size =  size;
	bitmap->numSetBits =  0;

	return bitmap;
}


void freeBitmap( struct Bitmap* bitmap){

        free(bitmap->bitarray);
        free(bitmap);
	
}

void clearBitmap(struct Bitmap* bitmap){

	memset(bitmap->bitarray, 0, (sizeof(__u32)*((bitmap->size+kBitsPerWord - 1)/kBitsPerWord)));
	bitmap->numSetBits =  0;

}

void setBit(struct Bitmap* bitmap, __u32 pos){

	bitmap->bitarray[word_offset(pos)] |= (__u32) (1 << bit_offset(pos));

}

void setBitRange(struct Bitmap* bitmap, __u32 start,__u32 end){

 __u32 pos;

 for (pos = start; pos < end; ++pos)
 {
 	setBit(bitmap, pos);
 }

}



__u32 getBit(struct Bitmap* bitmap, __u32 pos){

	return (bitmap->bitarray[word_offset(pos)] >> bit_offset(pos)) & 1l;;

}


void clearBit(struct Bitmap* bitmap, __u32 pos){

	bitmap->bitarray[word_offset(pos)] &= ((__u32) (~(1l << bit_offset(pos))));

}




void swapBitmaps (struct Bitmap** bitmap1, struct Bitmap** bitmap2){

	
	struct Bitmap* temp_bitmap = *bitmap1;
	*bitmap1 = *bitmap2;
	*bitmap2 = temp_bitmap;

}



__u32 getNumOfSetBits (struct Bitmap* bitmap){

	__u32 i;
	__u32 numSetBits = 0;

	#pragma omp parallel for reduction(+:numSetBits) schedule(dynamic,256)
	for(i= 0 ; i < (bitmap->size); i++){
		if(getBit(bitmap, i))
			numSetBits++;
	}

	return numSetBits;
}

void printSetBits (struct Bitmap* bitmap){

	__u32 i;

	for(i= 0 ; i < (bitmap->size); i++){
		if(getBit(bitmap, i)){
			printf("**%u \n", i);
		}
	}

}


/***********************************
you need to implement this function
***********************************/
void setBitAtomic(struct Bitmap* bitmap, __u32 pos){

/*
	atomic operatoin here
*/
	
}