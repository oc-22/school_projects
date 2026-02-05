// bitset.c
   // Řešení IJC-DU1, příklad b), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Program na dešifrování obrázku ppm

#include <stdio.h>
#include <stdlib.h>
#include "bitset.h"
#include "ppm.h"
#include "eratosthenes.h"
#include "error.h"

#define START_PRIME 101

/*
 * Markus Kuhn <http://www.cl.cam.ac.uk/~mgk25/> -- 2005-03-30
 * License: http://www.cl.cam.ac.uk/~mgk25/short-license.html
*/

#include <stdlib.h>

unsigned char *utf8_check(unsigned char *s)
{
  while (*s) {
    if (*s < 0x80)
      /* 0xxxxxxx */
      s++;
    else if ((s[0] & 0xe0) == 0xc0) {
      /* 110XXXXx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[0] & 0xfe) == 0xc0)                        /* overlong? */
	return s;
      else
	s += 2;
    } else if ((s[0] & 0xf0) == 0xe0) {
      /* 1110XXXX 10Xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||    /* surrogate? */
	  (s[0] == 0xef && s[1] == 0xbf &&
	   (s[2] & 0xfe) == 0xbe))                      /* U+FFFE or U+FFFF? */
	return s;
      else
	s += 3;
    } else if ((s[0] & 0xf8) == 0xf0) {
      /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[3] & 0xc0) != 0x80 ||
	  (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4) /* > U+10FFFF? */
	return s;
      else
	s += 4;
    } else
      return s;
  }

  return NULL;
}
//END OF MARKUS KUHN code

int main(int argc, char *argv[]){
    if(argc<2){
        error_exit("Invalid argument count");
    }
    struct ppm *image_data = ppm_read(argv[1]);
    if(image_data==NULL){
        ppm_free(image_data);
        error_exit("Image loading failed");
    }
    bitset_alloc(ppm_primes, (image_data->xsize*image_data->ysize*3));
    Eratosthenes(ppm_primes);

    unsigned char *message;
    int char_idx= 0;
    unsigned count = 0;

    for (unsigned idx = START_PRIME; idx<(image_data->xsize*image_data->ysize*3); idx++) {
        if (bitset_getbit(ppm_primes, idx)==1) {
            count++;
        }
    }
    message = (count%8==0) ? calloc((count/8),sizeof(char)) : calloc((count/8)+1,sizeof(unsigned char));

    if (message==NULL) {
        ppm_free(image_data);
        bitset_free(ppm_primes);
        error_exit("Memory allocation failed(message)");
    }
    count = 0;
    char nullT_check=0;
    for (unsigned idx = 101; idx<(image_data->xsize*image_data->ysize*3); idx++) {
        if (bitset_getbit(ppm_primes, idx)==1) {
            message[count] |= ((image_data->data[idx] & 1)<<char_idx);
            char_idx++;
            if (char_idx==8) {
                char_idx = 0;
                if (message[count]=='\0') {
                    nullT_check++;
                    break;
                }
                count++;
            }
        }
    }
    if(utf8_check(message)!=NULL || nullT_check==0){
        ppm_free(image_data);
        bitset_free(ppm_primes);
        free(message);
        error_exit("String is not UTF-8 or not correctly null terminated");
    }

    printf("%s\n",message);
    ppm_free(image_data);
    bitset_free(ppm_primes);
    free(message);
    return 0;
}
