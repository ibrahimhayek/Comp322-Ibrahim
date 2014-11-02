#include<stdio.h>
 
int main( int argc, char *argv[] )  

{

   int n=argv[1];
 
 /*  printf("Enter an address\n");
   scanf("%d",&n);*/
   
   int offset=0;
   offset=n%4096;
   printf("Offset = %d\n",offset);
   int divide=0;
   divide     = n / 4096;   //typecasting
    printf("page number = %d\n",divide);
 system("pause");
   return 0;
}
