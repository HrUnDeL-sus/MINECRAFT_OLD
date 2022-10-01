/* zpipe.c: example of proper use of zlib's inflate() and deflate()
   Not copyrighted -- provided to the public domain
   Version 1.2  9 November 2004  Mark Adler */

/* Version history:
   1.0  30 Oct 2004  First version
   1.1   8 Nov 2004  Add void casting for unused return values
                     Use switch statement for inflate() return values
   1.2   9 Nov 2004  Add assertions to document zlib guarantees
   1.3   6 Apr 2005  Remove incorrect assertion in inf()
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#define CHUNK 16384

int decompress_one_file(char *infilename, char *outfilename)
 {
    gzFile infile = gzopen(infilename, "rb");
    FILE *outfile = fopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char buffer[128];
    int num_read = 0;
   while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
     fwrite(buffer, 1, num_read, outfile);
       }

    gzclose(infile);
   fclose(outfile);
 }

 int compress_one_file(char *infilename, char *outfilename)
 {

    FILE *infile = fopen(infilename, "r+b");

    gzFile outfile = gzopen(outfilename, "w+b");
       printf("\nSTART");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;

    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
             printf("\nT:%d",total_read);
       total_read += num_read;

    int t=gzwrite(outfile, inbuffer, num_read);
    printf("\nTOTAL:%d",t);
    }

    fclose(infile);
    gzclose(outfile);
 }

