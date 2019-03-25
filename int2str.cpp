#include "string.h"
char *int2str(int integer, char string[], int length)
{
int i,j;
unsigned *digit;
digit=new unsigned[length];
for(i=0;i<length;i++)
{
	digit[length-i-1]=integer-(integer/10*10);
	integer/=10;
}
for(i=0;i<length;i++)
{
	string[i]='0'+digit[i];
}
return string;
}

