#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
/*convert argv[1] char value to the unsigned int*/
unsigned int address = strtoul(argv[1],NULL,10);

/*Page size is 4kb, which is equivalent to 4096 bits*/
int pgsize = 4096;

/*results initialized to 0. For a 32 bit system, page number could be of max 20 bits and offset could be of max 12 bits. Taking both as unsiigned int. Unsigned int is 32 bit long*/
unsigned int pgnumber =0,offset =0;

/*Calculation for page number and offset*/
pgnumber = address/pgsize;
offset = address%pgsize;

/*printing the output*/
printf("The address %u contains :\npage number = %u\noffset = %u\n",address,pgnumber,offset);

return 0;
}
