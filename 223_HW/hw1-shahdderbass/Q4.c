/*
Shahd Derbass
2231172085
HW1
Q4
*/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(){
	printf("Enter the length of one side of the cube: ");
	double s;
	scanf("%lf", &s);

	void *handle;
	double (*ptr)(double);
	char *error;

	handle = dlopen("./libQ4.so", RTLD_LAZY);
	if(!handle){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	ptr = dlsym(handle, "surface_area");
	if((error = dlerror()) != NULL){
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	
	printf("The surface area of the cube = %.2lf\n",ptr(s));

	ptr = dlsym(handle, "volume");
	if((error = dlerror()) != NULL){
                 fprintf(stderr, "%s\n", error);
                 exit(1);
        }
	printf("The volume of the cube = %.2lf\n",ptr(s));
	
	ptr = dlsym(handle, "diagonal");
	if((error = dlerror()) != NULL){
		fprintf(stderr, "%s\n", error);
 		exit(1);
 	}
	printf("The length of the diagonal in the cube = %.2lf\n",ptr(s));

	if(dlclose(handle) < 0){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	return 0;
}
