/*
   unihex2carray - program to turn a GNU Unifont hex glyph page of 256 code
                points into a C array data.

   Synopsis: unihex2carray [-iin_file.hex] [-oout_file.bmp]
                [-f] [-phex_page_num]
   
   Copyright (C) 2016 Steve Hsu(steve@kunkua.com)

   LICENSE:

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 2 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 256

int
main (int argc, char *argv[])
{

   int i, j;                  /* loop variables                    */
   unsigned k0;               /* temp Unicode char variable        */
   unsigned swap;             /* temp variable for swapping values */
   char inbuf[256];           /* input buffer                      */
   unsigned filesize;         /* size of file in bytes             */
   unsigned thisunicode;         /* the current character  unicode       */
   unsigned char thischarbyte; /* unsigned char lowest byte of Unicode char */
   int thischarrow;           /* row 0..15 where this character belongs  */
   int thiscol;               /* column 0..15 where this character belongs */
   int toppixelrow;           /* pixel row, 0..16*32-1               */

   unsigned char charbits[16][2];  /* bitmap for one character, 2 bytes/row */

   char *infile="unifont-9.0.04.hex", *outfile="unifont.h";  /* names of input and output files */
   FILE *infp, *outfp;      /* file pointers of input and output files */

   if (argc > 1) {
      for (i = 1; i < argc; i++) {
         if (argv[i][0] == '-') {  /* this is an option argument */
            switch (argv[i][1]) {
               case 'i':  /* name of input file */
                  infile = &argv[i][2];
                  break;
               case 'o':  /* name of output file */
                  outfile = &argv[i][2];
                  break;
               case 'p':  /* specify a Unicode page other than default of 0 */
                  sscanf (&argv[i][2], "%x", &unipage); /* Get Unicode page */
                  break;
               default:   /* if unrecognized option, print list and exit */
                  fprintf (stderr, "\nSyntax:\n\n");
                  fprintf (stderr, "   %s -p<Unicode_Page> ", argv[0]);
                  fprintf (stderr, "-i<Input_File> -o<Output_File> -w\n\n");
                  fprintf (stderr, "   -w specifies .wbmp output instead of ");
                  fprintf (stderr, "default Windows .bmp output.\n\n");
                  fprintf (stderr, "   -p is followed by 1 to 6 ");
                  fprintf (stderr, "Unicode page hex digits ");
                  fprintf (stderr, "(default is Page 0).\n\n");
                  fprintf (stderr, "\nExample:\n\n");
                  fprintf (stderr, "   %s -p83 -iunifont.hex -ou83.bmp\n\n\n",
                         argv[0]);
                  exit (1);
            }
         }
      }
   }

   printf("input_file:%s\n",infile);
   /*
      Make sure we can open any I/O files that were specified before
      doing anything else.
   */
   if (strlen (infile) > 0) {
      if ((infp = fopen (infile, "r")) == NULL) {
         fprintf (stderr, "Error: can't open %s for input.\n", infile);
         exit (1);
      }
   }
   else {
      infp = stdin;
   }
   if (strlen (outfile) > 0) {
      if ((outfp = fopen (outfile, "w")) == NULL) {
         fprintf (stderr, "Error: can't open %s for output.\n", outfile);
         exit (1);
      }
   }
   else {
      outfp = stdout;
   }

   fprintf(outfp,"#pragma once\n\n");
   fprintf(outfp,"typedef struct font_index {\n");
   fprintf(outfp,"	u16 unicode;\n");
   fprintf(outfp,"	u8 width;\n");
   fprintf(outfp,"	u8 data[32];\n");
   fprintf(outfp,"}Font_Index;\n\n");
   fprintf(outfp,"Font_Index indies[] =  { \n");
	int width;
   /*
      Read in the characters in the page
   */
   while (fgets (inbuf, MAXBUF-1, infp) != NULL) {
      sscanf (inbuf, "%x", &thisunicode);
      printf("thisunicode:%x\n",thisunicode);

      for (k0=0; inbuf[k0] != ':'; k0++);
         k0++;
      width = getWidth(&inbuf[k0]);  /* convert hex string to 32*4 bitmap */
	  fprintf(outfp,"{0x%c%c%c%c,%d,",inbuf[0],inbuf[1],inbuf[2],inbuf[3],width/4);
	  fprintf(outfp,"{");
	  if(width == 0) {
	  	fprintf(outfp,"0");	 			  
	  } else {
	  for(j = width/2;j > 0;j--) {
		 if(j == 1)
 			fprintf(outfp,"0x%c%c",inbuf[k0],inbuf[k0+1]);
		 else 
	  	 	fprintf(outfp,"0x%c%c,",inbuf[k0],inbuf[k0+1]);
		 k0 += 2;
	  }
	  }
	  fprintf(outfp,"}},\n");	  
   }

   fprintf(outfp,"};");	  

   exit (0);
}

int
getWidth (char *instring)
{
   int width;
   if (strlen (instring) <= 34)  /* 32 + possible '\r', '\n' */
      width = 32;
   else if (strlen (instring) <= 66)  /* 64 + possible '\r', '\n' */
      width = 64;
   else
      return 0;
	
   return width;
}

