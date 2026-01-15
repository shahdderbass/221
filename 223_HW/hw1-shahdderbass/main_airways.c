#include <stdio.h>
	char *passenger = "Majed Khalil";
	extern char *destination;
	float ticket_price;
	int luggage_weight = 20;
	
	extern void addLuggageWeight (int);
int main() {
	printf("Passenger name: %s\n", passenger);
	printf("Destination: %s\n", destination);
	addLuggageWeight(5);
	printf("Luggage weight: %d\n", luggage_weight);
	
	return 0;
}
