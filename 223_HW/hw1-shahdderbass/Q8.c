/*
Shahd Derbass
2231172085
HW1
Q8
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

int a,b, result = 0;
jmp_buf buff;

void handler(){
	char buffer[40];
	const char msg[] = "Cannot divide by zero. Enter another value for b: ";
	while(b == 0){
		write(1, msg, strlen(msg));

		int size = read(0, buffer, sizeof(buffer));
		if(size > 0){
			buffer[size-1] = '\0';
			b = atoi(buffer);
		}
	}
	longjmp(buff,1);
}

int main(){
	signal(SIGFPE, handler);
	printf("Enter values of a and b: ");
	scanf("%d %d", &a, &b);

	if(setjmp(buff) == 0) {
		if(b == 0)
			handler();
		result = a/b;	
	}
	else{
		result = a/b;
	}

	printf("%d/%d = %d\n", a, b, result);
	
	return 0;
}
