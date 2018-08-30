#include <stdio.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	char c;
	c = fgetc(stdin);
	while(c != EOF){
		if(isalnum(c))
			printf("%c", c);
		c = fgetc(stdin);
	}
	return 1;
}
