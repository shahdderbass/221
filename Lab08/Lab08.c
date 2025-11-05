#include<stdio.h>

int main(int argc, char *argv[])
{
	if(argc < 2){
		perror("Invalid number of arguments.\n");
		return 1;
	}
	if (freopen(argv[1], "w", stdout) == NULL) {
        	perror("freopen");
        	return 1;
    	}
	char c1,c2,c3;
    	scanf("%c%c%c",&c1,&c2,&c3);
    	printf("c1= %c , c2= %c , c3= %c \n",c1,c2,c3);

    return 0;

}
