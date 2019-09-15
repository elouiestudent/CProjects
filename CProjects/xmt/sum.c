#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define n 4

int main() {
	int A[n] = {3, 2, 1, 4};
	int B[n][n] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
	#pragma omp parallel for private(i)
	for(i = 0; i < n; i++) {//pardo
	    B[0][i] = A[i];
	    for(h = 0; h < n; h++) {
	        if (i =< n/(2^h)){
	            B[h+1][i] = B[h][2i-1] + B[h][2i]
	        }   
	    }
	}
}