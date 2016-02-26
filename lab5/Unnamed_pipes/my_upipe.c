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
	if (argc != 3) {
       fprintf(stderr, "Usage: ./my_upipe 'cat /etc/passwd' 'grep root'\n");
       exit(EXIT_FAILURE);
    }
	FILE * fp, * fp1;
	char buffer[BUFSIZ+1];

	fp = popen(argv[2], "w");
	fp1 = popen (argv[1], "r");

	if( fp != NULL && (fp1!=NULL))

	{
		//printf("%s = %s", argv[2], argv[1]);
		fread(buffer, sizeof(char), BUFSIZ, fp1);
		fwrite(buffer, sizeof(char), strlen(buffer), fp);
		pclose(fp);
		pclose(fp1);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}

