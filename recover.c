/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

/*

valgrind --leak-check=full ./
check50 2015.fall.pset4.recover recover.c

FILE* file = fopen("card.raw", "r");

the first three bytes of JPEGs are
0xff 0xd8 0xff

The fourth byte, meanwhile, is either 0xe0, 0xe1, 0xe2, 0xe3, 
0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 
0xed, 0xee, of 0xef. Put another way, 
the fourth byte’s first four bits are 1110.


FAT file system whose "block size" is 512 bytes


Your program should number the files it outputs by 
naming each ###.jpg, where ### is three-digit decimal
number from 000 on up.

(Befriend sprintf.)

use malloc, be sure to use free so as not to leak memory. Try using valgrind to check for any leaks!

*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // TODO
}
