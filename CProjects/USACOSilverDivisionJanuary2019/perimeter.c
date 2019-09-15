//
// Torbert, 8 November 2018
//
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char sym;
    int covered;
} IceTuple;

IceTuple ice[1000][1000] = {'\0'};;
int area[1000 * 1000] = {'\0'};;
int perimeter[1000 * 1000] = {'\0'};;
int N;

void floodfill(int i, int j, int curr){
	// printf("floodfill<\n");
	ice[i][j].covered = 1;
	area[curr] += 1;
	// printf("symbol: %c\n", ice[i][j].sym);
	// printf("area[curr]: %d\n", area[curr]);
	// printf("perimeter[curr]: %d\n", area[curr]);
	// printf("i: %d, j: %d\n", i, j);
	if(i + 1 < N){
		if(ice[i + 1][j].sym == '#'){
			if(ice[i + 1][j].covered == 0){
				floodfill(i + 1, j, curr);
			}
		}
		else{
			perimeter[curr] += 1;
		}
	}
	else{
		perimeter[curr] += 1;
	}
	if(i - 1 >= 0){
		if(ice[i - 1][j].sym == '#'){
			if(ice[i - 1][j].covered == 0){
				floodfill(i - 1, j, curr);
			}
		}
		else{
			perimeter[curr] += 1;
		}
	}
	else{
		perimeter[curr] += 1;
	}
	if(j + 1 < N){
		if(ice[i][j + 1].sym == '#'){
			if(ice[i][j + 1].covered == 0){
				floodfill(i, j + 1, curr);
			}
		}
		else{
			perimeter[curr] += 1;
		}
	}
	else{
		perimeter[curr] += 1;
	}
	if(j - 1 >= 0){
		if(ice[i][j - 1].sym == '#'){
			if(ice[i][j - 1].covered == 0){
				floodfill(i, j - 1, curr);
			}
		}
		else{
			perimeter[curr] += 1;
		}
	}
	else{
		perimeter[curr] += 1;
	}
	// printf("ENDPerim:%d\n", perimeter[curr]);
}

int main(){
	char * buffer = 0;
	long length;
	FILE * f = fopen ("perimeter.in", "rb");

	if (f)
	{
	  	fseek (f, 0, SEEK_END);
	  	length = ftell (f);
	  	fseek (f, 0, SEEK_SET);
	  	buffer = malloc (length);
	  	if (buffer)
	  	{
	    	fread (buffer, 1, length, f);
	  	}
	  	fclose (f);
	}

	if (buffer)
	{
		N = buffer[0] - '0'; //WRONGGGGGGGGGGGG MUST ACCESS REST OF NUMBERRRRR
		// printf("N:%d", N);
		int count = 1;
		int curr = 0;
		int i,j;
		for(i = 0; i < N; i++) {
        	for(j = 0; j < N; j++) {
        		if(buffer[count] == '\n'){
        			count += 1;
	         	}
            	IceTuple temp = {buffer[count], 0};
        		ice[i][j] = temp;
            	// printf("|%c|", buffer[count]);
	            count += 1;
        	}
    	}
    	
    	for(i = 0; i < N; i++) {
        	for(j = 0; j < N; j++) {
        		if(ice[i][j].covered == 0 && ice[i][j].sym =='#'){
	            	floodfill(i, j, curr);
	            	curr += 1;
	            }
        	}
    	}
    	int maxarea;
    	int maxperim;
    	for(i = 0; i < curr; i++){
    		if(area[i] > maxarea){
    			maxarea = area[i];
    			maxperim = perimeter[i];
    		}
    		else if(area[i] == maxarea){
    			if(perimeter[i] < maxperim){
    				maxperim = perimeter[i];
    			}
    		}
    		// printf("area[i]: %d, perimeter[i]%d\n", area[i], perimeter[i]);
    	}
    	FILE* fout = fopen("perimeter.out", "w");
    	fprintf(fout, "%d %d", maxarea, maxperim);
    	fclose(fout);
    	// printf("maxarea: %d, maxperim: %d\n", maxarea, maxperim);
	};
	return 0 ;
}

//
// end of file
//