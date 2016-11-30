#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXBUF 256

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#include "unifont.h"

void printFont(Font_Index* index) {
	int w = index->width;
	int col = w/8;
	u8* data = (u8*)malloc(w*2);
	memcpy(data,index->data,w*2);
	u8 mask;
	int i,j,k;
	u8 tmp;
	printf("unicode:0x%x;col:%d\n",index->unicode,col);
	for(i=0;i<16;i++) {
		for(j=0 ;j < col ;j++) {
			mask = 0x80;
			tmp = data[i*col + j];
			//printf("i:%d,data:%x\n",i*col + j,tmp);
			for(k=0;k<8;k++) {
				//printf("d:0x%x,",tmp&mask);
				if(tmp&mask) {
					printf("x");
				}else {
					printf(" ");
				}
				mask >>= 1;
			}
			
		}
		printf("\n");
	}
}

int
main (int argc, char *argv[])
{

   int i,j;                  /* loop variables                    */
  
   Font_Index index;
   for(i=0;i < 0xfffe; i++) {
		if(i>=0xd800 && i<0xf900) {
			continue;
		}else if(i >= 0xf900) {
			index = indies[i-0x2100];
		} else {
			index = indies[i];
		}
   	
		if(index.unicode == i) {
			printFont(&index);
		} else {
			fprintf(stderr,"error at %x\n",i);
			exit(0);
		}
   }
  
   exit (0);
}

