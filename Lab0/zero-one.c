#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>


/*
 * This function computes and returns the decimal value of a
 * a string containing 1's and 0's 
 */
int ascii2decimal(char* zeroones)
{
	int power=0;        /* power of 2 used in the computation */
	int decimalVal=0;   /* value of the converted decimal */
	int len = strlen(zeroones);  /* lenth of zeroones */
	int i;              /* index to loop over characters of zeroones */
	
	for(i= (len-1); i > 0; i--)
	{
		if(zeroones[i] == '1')
		{
			// decimalVal = decimalVal + (2 ^ power)
			decimalVal += (int)pow(2,power);
		}
		// increment the power of 2 as we move from right to left
		power++;
	}
	
	return decimalVal;
}


/*
 * This function returns a string after padding count zeros
 * to the right of the string s 
 */
char* padzeros(char* s, int count)
{
	// get the length of the string s
	int len = strlen(s);  
	
	// allocate buffer to store s and count 0's
	char* result = (char*)malloc(sizeof(char) * (len + count));
	
	int i;
	// copy s into result
	for(i=0; i < len; i++)
	{
		result[i] = s[i];
	}
	// add the trailing 0's
	for(; i < (len+count); i++)
	{
		result[i] = '0';
	}
	// add the NULL terminator
	result[i] = '\0';
	
	// return the result string
	return result;
}

/*
 * Count and return the parity in the string zeroones
 */
int count_parity(char* zeroones)
{
	int parity = 0;
	// loop over all the characters
	while (*zeroones != '\0')
	{
		// update the parity from current character
		if(*zeroones == '1')
		{
			parity++;
		}
		zeroones++;
	}
	return parity;	
}


/*
 * Reads upto next 8 characters from file descriptor fd
 * and stores them in buff. Skips spaces and newlines.
 * Returns the number of characters stored in buff
 */
int read_next_set(int fd, char* buff)
{
	int numBytesStored = 0;
	int i=0;
	char ch;  // character read from file
	
	// read a single character/byte at a time from the file
	while( read(fd, &ch, 1) > 0)
	{
		if((ch != ' ') && (ch != '\n'))  // skip spaces and newlines
		{
			buff[i] = ch;
			i++;
			numBytesStored++;
		}
		
		if(numBytesStored == 8)  // stop after reading 8 characters
		{
			break;
		}
	}	
	// NULL terminate the buff
	buff[i] = '\0';
	
	// return the number of characters stored in buff
	return numBytesStored;
}

int main(int argc, char *argv[])
{
	int fd;                // file descriptor of input file
	char buff[9];          // buffer to read 8 characters from the file
	char inputFile[20];    // name of the input file
		
	if(argc > 1)  // command line argument exists
	{
		if(argv[1][0] != '-')  // command line argument does no start with '-'
		{
			strcpy(inputFile, argv[1]);  // save the filename from command line argument
		}		
		else
		{
			// prompt user to enter the filename through stdin (console/keyboard)
			printf("Enter input file name: ");
			// read the input file name from stdin
			scanf("%s", inputFile);
		}		
	}
	else    // there is no command line argument
	{
		// prompt user to enter the filename through stdin (console/keyboard)
		printf("Enter input file name: ");
		// read the input file name from stdin
		scanf("%s", inputFile);
	}
	
	// open the input file for reading, exit if there is any error
	fd = open(inputFile, O_RDONLY);
	if(fd < 0)
	{
		printf("\nerror: file %s could not be opened for reading!\n", inputFile);
		exit(0);
	}
	
	// display the header of the output
	printf("\nOriginal\tASCII\t\tDecimal\tParity\n");
	printf("\n--------\t-----\t\t-------\t------\n");
	
	
	// read 8 characters at a time from the file
	int bytesRead; 
	while( (bytesRead = read_next_set(fd, buff) ) > 0)
	{
		// padd buff with trailing 0's if its
		// length is less than 8 characters
		int len = strlen(buff);
		if(len < 8)
		{
			char* zeroPaddedStr = padzeros(buff, (8 - len));
			strcpy(buff,zeroPaddedStr);
		}
		
		// now buff contains 8 characters of 1's and 0's, we now process it for output
		
		// get the decimal value
		int decimalVal = ascii2decimal(buff);
		
		// display the original to output
		printf("%s\t", buff);
		
		// display the ASCII to output
		if(decimalVal < 128)
		{
			// this is a printable ASCII value
			printf("%c\t\t", decimalVal);
		}
		else
		{
			// this is a NON printable ASCII value
			printf("man ascii\t");
		}
		// display the decimal to output
		printf("%d\t", decimalVal);
		
		// display the parity
		int parity = count_parity(buff);
		if((parity % 2) == 0)  // EVEN value
		{
			printf("EVEN\n");
		}
		else  // ODD value
		{
			printf("ODD\n");
		}
	}
		
	// close the input file
	close(fd);
	
	return 0;
}
