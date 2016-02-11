/*
#
#		Santiago Larrain
#		slarrain@uchicago.edu
#
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE * fp;
	char buffer[BUFSIZ+1];
	sprintf(buffer, argv[1]);
	fp = popen(argv[2], "w");
	if( fp != NULL )
	{
		fwrite(buffer, sizeof(char), strlen(buffer), fp);
		pclose(fp);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}

