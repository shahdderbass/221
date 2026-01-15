/*
 * Your assignment will be to write a Y86-64 program to perform bubblesort. For
rieference, the following C function implements bubblesort using array referencing:
 Bubble sort: Array version */
/*
 * void bubble_a(long *data, long count) {
	long i, last;
	for (last = count-1; last > 0; last--) {
		for (i = 0; i < last; i++){
			if (data[i+1] < data[i]) {
			long t = data[i+1];
			data[i+1] = data[i];
			data[i] = t;
			}
		}
	}
 }

A. Write and test a C version that references the array elements with pointers,
rather than using array indexing.
*/

void bubble_a(long* data, long count){
       long* i, *last;
	for(last = data + count - 1; last > data; last--){
	 	for(i = data; i < last; i++){
			if(*(i+1) < *i){
				long t = *(i+1);
				*(i+1) = *i;
				*i = t;
			}
		}
	}
}

