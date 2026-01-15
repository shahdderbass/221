#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX 1024

const char* classify_bmi(float  bmi, int age) {
    if (age < 18) { // Children
        if (bmi < 16.8) return "Underweight";
        if (bmi < 22.9) return "Healthy weight";
        if (bmi < 27.8) return "Over weight";
        return "Obese";
    } else if (age >= 18 && age <= 64) { // Adults
        if (bmi < 18.5) return "Underweight";
        if (bmi < 25.0) return "Healthy weight";
        if (bmi < 30.0) return "Over weight";
        return "Obese";
    } else { // Older Adults (>64)
        if (bmi < 22.0) return "Underweight";
        if (bmi < 28.0) return "Healthy weight";
        if (bmi < 30.0) return "Over weight";
        return "Obese";
    }
}

int main() {
	char content[MAX];
	char *qs = getenv("QUERY_STRING");  // get query string
    	if (!qs) {
        	printf("Content-Type: text/html\r\n\r\n");
        	printf("<html><body>");
		printf("<h1>Error!</h1>");
		printf("<p>Invalid Arguments.</p>");
		printf("</body></html>");
        	return 1;
    	}

    	float height = 0.0, weight = 0.0;
    	int age = 0;

    	sscanf(qs, "weight=%f&height=%f&age=%d", &weight, &height, &age);
	float height_m = height/100.0;
	float bmi = weight / (height_m * height_m);
	const char* category = classify_bmi(bmi, age);
	
	/*
    	sprintf(content, "BMI Calculator result: \n");
	sprintf(content, "%sWeight: %.2f\n",content, weight);
	sprintf(content, "%sHeight: %.2f\n", content, height);
	sprintf(content, "%sAge: %d\n", content, age);
	sprintf(content, "%sBMI: %.2f\n",content, bmi);
	sprintf(content, "%sYou are %s\n",content, category);
	
	printf("Content-length: %d\r\n", (int)strlen(content));;
	printf("Content-Type: text/html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);

	*/
	printf("Content-Type: text/html\r\n\r\n");
	printf("<html><body>");
	printf("<h1>BMI Calculator result</h1>");
	printf("<p>Weight: %.2f</p>", weight);
	printf("<p>Height: %.2f</p>", height);
	printf("<p>Age: %d</p>", age);
	printf("<p>BMI: %.2f</p>", bmi);
	printf("<p>%s</p>", category);
	printf("</body></html>");


    return 0;
}

