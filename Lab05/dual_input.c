#include <stdio.h>
#include <signal.h>

int main(){	

	sigset_t mask, oldmask;
	int a,b;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_BLOCK,&mask, &oldmask);

	printf("Enter a: ");
	scanf("%d", &a);

	printf("You entered: %d\n", a);

	sigprocmask(SIG_SETMASK,&oldmask, NULL);

	printf("Enter b: ");
	scanf("%d", &b);

	printf("You entered %d\n", b);

	return 0;
}
