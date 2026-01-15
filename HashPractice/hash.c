
#include <stdio.h>
#include <string.h>

#if defined(__APPLE__)

#include <CommonCrypto/CommonDigest.h>
#define SHA256           CC_SHA256
#define SHA256_DIGEST_LENGTH CC_SHA256_DIGEST_LENGTH

#else
#include <openssl/sha.h>
#endif

#include <stdlib.h>
#define LEN 1024

int main() {
    	char *password = malloc(LEN);
    	unsigned char *hash = malloc(LEN);

    	printf("Enter password: ");
    	scanf("%1023s", password);

    	// Hash password using SHA-256
    	SHA256((unsigned char*)password, strlen(password), hash);

	FILE *fptr = fopen("users.txt", "a");

	fprintf(fptr, "%s", hash);

	fclose(fptr);
	
	free(password);
	free(hash);

    	printf("Hash saved successfully.\n");

	return 0;
}

