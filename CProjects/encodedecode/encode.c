//
// Torbert, 16 Sept 2015
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
typedef struct Node
{
	char symbol ;
	//
	int frequency ;
	//
	struct Node* left ;
	struct Node* right ;
	//
} TreeNode ;

char s[1024];
void Logger(char*);


// TreeNode* swap(TreeNode* array, int oneIndex, int twoIndex)
// {
// 	TreeNode temp = array[oneIndex];
// 	array[oneIndex] = array[twoIndex];
// 	array[twoIndex] = temp;
// 	return array;
// }

void heapify(TreeNode* arr[], int n, int i) 
{ 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < n && arr[l] -> frequency > arr[largest] ->frequency) 
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < n && arr[r] -> frequency > arr[largest] -> frequency) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
    	TreeNode* temp = arr[i];
    	arr[i] = arr[largest];
    	arr[largest] = temp;
  
        // Recursively heapify the affected sub-tree 
        heapify(arr, n, largest); 
    } 
} 
  
// main function to do heap sort 
void heapSort(TreeNode* arr[], int n) 
{ 
    // Build heap (rearrange array) 
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
  
    // One by one extract an element from heap 
    for (int i=n-1; i>=0; i--) 
    { 
        // Move current root to end 
        // swap(arr[0], arr[i]); 
        TreeNode* temp = arr[0];
    	arr[0] = arr[i];
    	arr[i] = temp;
  
        // call max heapify on the reduced heap 
        heapify(arr, i, 0); 
    } 
} 

void printer(TreeNode* array[], int arraySize)
{
	printf("printer<\n");
	for(int j = 0; j < arraySize; j++)
	{
		// printf("Index: %d Symbol: %c, Frequency: %d\n", i, array[i] -> symbol, array[i] -> frequency);
		printf("Index: %d\n", j);
		char sym = (*array[j]).symbol;
		int frequ = (*array[j]).frequency;
		printf("Symbol: %c\n", sym);
		printf("Frequency: %d\n", frequ);
	}
	printf("printer>\n");
}

void recur(TreeNode* currNode, char* encode[][256], char* curr[], int index)
{
	sprintf(s,"Line %d, encode[0]=%s", __LINE__, (char*)encode[0]);
	Logger(s);

	if(currNode -> symbol != 0){
		memcpy(encode[currNode -> symbol], curr, sizeof(encode[currNode -> symbol]));
	}
	if(currNode -> left != NULL){
		curr[index] = "0";
		recur(currNode -> left, encode, curr, index + 1);
	}
	if(currNode -> right != NULL){
		curr[index] = "1";
		recur(currNode -> right, encode, curr, index + 1);
	}
}

void Logger(char * s)
{
	int p = 1;
	if (p)
	{
		printf("%s\n", s);
	}
}

//
int main( int argc , char* argv[] )
{
	// TreeNode* t = NULL ;
	// //
	// t = (TreeNode*)malloc( sizeof(TreeNode) );
	// //
	// (*t).symbol = 'A' ;
	// t -> frequency = 7 ;
	// t -> left = NULL ;
	// t -> right = NULL ;
	// //
	// printf( "%c\n" , t->symbol ) ;
	// printf( "%d\n" , t->frequency ) ;
	
	FILE* fin = fopen( "test.txt" , "r" ) ;
	char ch;
	char* currentString[256];
	char* encodes[256][256];
	int n = 0;
	int totalFreq = 256;
	int numbytes;
	int freq[256] = {0};

	Logger("main() < ");

	//DO FREQ NUM FIRST THEN NODE* ARRAY freq size is 256
	// printer(freq, 128);
	while(1){
		numbytes = fread( &ch , sizeof(char) , 1 , fin ) ;
		if( numbytes == 0 ) break ;
		// printf("Symbefore: %d\n",freq[ch].symbol);
		if(freq[ch] == 0){
			// freq[ch] = new TreeNode;
			// freq[ch].symbol = ch;
			// printf("Index: %c Symbol: %c, Frequency: %d\n", ch, freq[ch].symbol, freq[ch].frequency);
			n++;
		}
		freq[ch]++;
		// printf("newFreq: %d\n",freq[ch].frequency);
		printf( "|%c|" , ch ) ;
	}
	fclose( fin ) ;
	printf( "\n" ) ;
	printf( "%d\n" , n ) ;
	printf( "\n" ) ;

	printf( "%d\n" , n ) ;
	TreeNode* nodes[n];
	n = 0;
	for(int i = 0; i < totalFreq; i++){
		if(freq[i] > 0){
			printf("%c\n", i);
			printf("%d\n",freq[i]);
			TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
			t -> symbol = i;
			t -> frequency = freq[i];
			t -> left = NULL;
			t -> right = NULL;
			nodes[n] = t;
			n++;
		}
	}
	printf( "%d\n" , n ) ;

	printer(nodes, n);
	heapSort(nodes, n);
	printf("NEW\n");
	printer(nodes, n);

	while(n > 1){
		TreeNode* t1 = nodes[0];
		TreeNode* t2 = nodes[1];
		TreeNode* combi = (TreeNode*)malloc(sizeof(TreeNode));
		combi -> frequency = t1 -> frequency + t2 -> frequency;
		combi -> left = t1;
		combi -> right = t2;
		nodes[0] = NULL;
		nodes[1] = combi;
		n--;
		TreeNode* newNodes[n];
		for(int i = 1; i < n + 1; i++){
			newNodes[i - 1] = nodes[i];
		}
		memcpy(nodes, newNodes, sizeof(nodes));
	}

	Logger("Before Lizzie is a bum!");

	printer(nodes, n);

	Logger("Lizzie is a bum!");

	recur(nodes[0], encodes, currentString, 0);

	Logger("I knew she is a bum!");

	FILE* fout = fopen("out.txt", "w");

	Logger("after fout");

	for(int i = 0; i < totalFreq; i++){
		sprintf(s,"Line %d, i=%d", __LINE__, i);
		Logger(s);
		sprintf(s,"Line %d, encodes[%d]=%s", __LINE__, i, (char*)encodes[i]);
		Logger(s);
		if(strcmp(encodes[i][0], "\0") == 0){
			sprintf(s,"Line %d, strcmp", __LINE__);
			Logger(s);
			sprintf(s,"Line %d, encodes[%d]=%s", __LINE__, i, (char*)encodes[i]);
			Logger(s);
			fprintf(fout, "%c", i);
			fprintf(fout, "%s\n", (char*)encodes[i]);
		}
	}
	fclose(fout);

	return 0 ;

	Logger("main() >");
}
//
// end of file
//
