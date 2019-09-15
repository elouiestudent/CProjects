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

// GLOBALS
char encodes2[256][256];

// FUNCTION DECLARATIONS
void printer(TreeNode* array[], int arraySize);

#define NOLOG 0
#define INFO 1
#define ENTRY 2

//int logLevel = NOLOG;
int logLevel = INFO;
//int logLevel = ENTRY + INFO;
char s[1024];

#define LOGENTRY if (logLevel & ENTRY) {printf("Line %d %s()<\n", __LINE__, __func__);}
#define LOGEXIT if (logLevel & ENTRY) {printf("Line %d %s()>\n", __LINE__, __func__);}
#define LOG(x) \
	if (logLevel & INFO) { \
		printf("Line %d %s(): %s\n", __LINE__, __func__, x); \
	}
#define LOG2(x,y) sprintf(s, x, y); LOG(s);
#define LOG3(x,y,z) sprintf(s, x, y, z); LOG(s);
#define LOG4(x,y,z,a) sprintf(s, x, y, z, a); LOG(s);
#define LOG5(x,y,z,a,b) sprintf(s, x, y, z, a, b); LOG(s);

// TreeNode* swap(TreeNode* array, int oneIndex, int twoIndex)
// {
// 	TreeNode temp = array[oneIndex];
// 	array[oneIndex] = array[twoIndex];
// 	array[twoIndex] = temp;
// 	return array;
// }

void heapify(TreeNode* arr[], int n, int i) 
{ 
	
	LOGENTRY; 
	LOG3("n=%d, i=%d", n, i);

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

	LOGEXIT;

} 
  
// main function to do heap sort 
void heapSort(TreeNode* arr[], int n) 
{ 
	LOGENTRY;

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

	LOGEXIT;
} 

/***
void printer(TreeNode* array[], int arraySize)
{
	LOGENTRY;

	for(int j = 0; j < arraySize; j++)
	{
		// printf("Index: %d Symbol: %c, Frequency: %d\n", i, array[i] -> symbol, array[i] -> frequency);
		// printf("Index: %d\n", j);
		char sym = (*array[j]).symbol;
		int frequ = (*array[j]).frequency;
		// printf("Symbol: %c\n", sym);
		// printf("Frequency: %d\n", frequ);
		sprintf(s, "Index: %d; Symbol: %c [%d]; Frequency: %d", j, sym, sym, frequ);
		LOG(s);
	}

	LOGEXIT;
}
***/

void recur(TreeNode* currNode, char* encode[][256], char* curr[], int index)
{
	LOGENTRY;

	sprintf(s,"currNode %d, symbol: %c [%d], freq %d, index: %d, curr: %s",
		(int)currNode, currNode->symbol, (int)currNode->symbol, 
		currNode->frequency, index, *curr);
	LOG(s);

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

	LOGEXIT;
}

void recur2(TreeNode* currNode, char* curr, int index)
{
	LOGENTRY;
	

	LOG4("currNode %d, curr: %s, index: %d", (int)currNode, curr, index);
	if (currNode->left != NULL) {
		LOG2("currNode->left: %d", (int)currNode->left);
	} else {
		LOG("currNode->left: NULL");
	}
	if (currNode->right != NULL) {
		LOG2("currNode->right: %d", (int)currNode->right);
	} else {
		LOG("currNode->right: NULL");
	}

//	if (currNode->symbol != 0) {
	if (currNode->left == NULL && currNode->right == NULL) {
		sprintf(s,"currNode %d, symbol: %c [%d], freq %d, curr: %s, index: %d",
			(int)currNode, currNode->symbol, (int)currNode->symbol, 
			currNode->frequency, curr, index);
		LOG(s);
	}


//	if(currNode -> symbol != 0){
	if (currNode->left == NULL && currNode->right == NULL) {
		memcpy(encodes2[currNode -> symbol], curr, sizeof(encodes2[currNode -> symbol]));
		sprintf(s,"currNode %d, symbol: %c [%d], freq %d, index: %d, curr: %s, encodes2[]: %s",
			(int)currNode, currNode->symbol, (int)currNode->symbol, 
			currNode->frequency, index, curr, encodes2[currNode->symbol]);
		LOG(s);
	}
	if(currNode -> left != NULL){
		curr[index] = '0';
		curr[index + 1] = '\0';
		LOG2("go left <<<, curr %s", curr);
		recur2(currNode -> left, curr, index + 1);
	}
	if(currNode -> right != NULL){
		curr[index] = '1';
		curr[index + 1] = '\0';
		LOG2("go right >>>, curr %s", curr);
		recur2(currNode -> right, curr, index + 1);
	}

	LOGEXIT;
}

char *readFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL)
        return NULL; //could not open file

    code = malloc(1000);

    while ((c = fgetc(file)) != EOF)
    {
    	if(c != '\n'){
        	code[n++] = (char) c;
    	}
    }

    // don't forget to terminate with the null character
    code[n] = '\0'; 
    fclose(file);       

    return code;
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
	
	// FILE* fin = fopen( "test.txt" , "r" ) ;
	char ch;
	char *currentString[256];
	char currentStr[256];
	char* encodes[256][256];
	char* passage = readFile("test.txt");
	int n = 0;
	int readCount = 0;
	int totalFreq = 256;
	int numbytes;
	int freq[256] = {0};

	LOGENTRY;

	while(1){
		char ch = passage[readCount];
		if(ch == '\0') break;
		if(freq[ch] == 0){
			// freq[ch] = new TreeNode;
			// freq[ch].symbol = ch;
			// printf("Index: %c Symbol: %c, Frequency: %d\n", ch, freq[ch].symbol, freq[ch].frequency);
			n++;
		}
		freq[ch]++;
		readCount++;
	}

	//DO FREQ NUM FIRST THEN NODE* ARRAY freq size is 256
	// printer(freq, 128);
	// while(1){
	// 	numbytes = fread( &ch , sizeof(char) , 1 , fin ) ;
	// 	if (ch == '\n') break;
	// 	if( numbytes == 0 ) break ;
	// 	// printf("Symbefore: %d\n",freq[ch].symbol);
	// 	if(freq[ch] == 0){
	// 		// freq[ch] = new TreeNode;
	// 		// freq[ch].symbol = ch;
	// 		// printf("Index: %c Symbol: %c, Frequency: %d\n", ch, freq[ch].symbol, freq[ch].frequency);
	// 		n++;
	// 	}
	// 	freq[ch]++;
	// 	// printf("newFreq: %d\n",freq[ch].frequency);
	// 	LOG2( "|%c|" , ch ) ;
	// }


	// fclose( fin ) ;
	//printf( "\n" ) ;
	//printf( "%d\n" , n ) ;
	//printf( "\n" ) ;

	//printf( "%d\n" , n ) ;

	LOG2("Number of char: %d", n);

	TreeNode* nodes[n];
	n = 0;
	for(int i = 0; i < totalFreq; i++){
		if(freq[i] > 0){
			//printf("%c\n", i);
			// printf("%d\n",freq[i]);
			LOG3("Counting: char %c, freq %d", i, freq[i]);

			TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
			t -> symbol = i;
			t -> frequency = freq[i];
			t -> left = NULL;
			t -> right = NULL;
			nodes[n] = t;
			n++;
		}
	}

	LOG2("Number of char %d" , n ) ;

	printer(nodes, n);
	heapSort(nodes, n);

	LOG("NEW");

	printer(nodes, n);

	int aNTotal = n;
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
		heapSort(newNodes, n);
		memcpy(nodes, newNodes, sizeof(nodes));
	}

	printer(nodes, n);

	currentStr[0] = '\0';
	LOG2("currentStr: %s", currentStr);

	//recur(nodes[0], encodes, currentString, 0);
	recur2(nodes[0], currentStr, 0);

	FILE* fout = fopen("out.txt", "w");
/***
	for(int i = 0; i < totalFreq; i++){
		sprintf(s,"i=%d",i);
		LOG(s);
		sprintf(s,"encodes[%d]=%s", i, (char*)encodes[i]);
		LOG(s);
		if(strcmp(encodes[i][0], "\0") == 0){
			LOG("strcmp");
			sprintf(s,"encodes[%d]=%s",i, (char*)encodes[i]);
			LOG(s);
			fprintf(fout, "%c", i);
			fprintf(fout, "%s\n", (char*)encodes[i]);
		}
	}
***/
	fprintf(fout, "%d\n", aNTotal);
	for(int i = 0; i < totalFreq; i++){
		//LOG3("encodes2[%d]=%s", i, encodes2[i]);

		if(strlen(encodes2[i])> 0){
			LOG3("encodes2[%d]=%s",i, encodes2[i]);
			fprintf(fout, "%c", i);
			fprintf(fout, "%s\n", encodes2[i]);
		}
	}

	readCount = 0;
	while(1){
		char ch = passage[readCount];
		if(ch == '\0') break;
		// LOG2("%s", encodes2[ch]);
		fprintf(fout, "%s", encodes2[ch]);
		readCount++;
	}
	fprintf(fout, "\n");

	fclose(fout);


	return 0 ;

	LOGEXIT;
}

void printer(TreeNode* array[], int arraySize)
{
	LOGENTRY;

	for(int j = 0; j < arraySize; j++)
	{
		// printf("Index: %d Symbol: %c, Frequency: %d\n", i, array[i] -> symbol, array[i] -> frequency);
		// printf("Index: %d\n", j);
		char sym = (*array[j]).symbol;
		int frequ = (*array[j]).frequency;
		// printf("Symbol: %c\n", sym);
		// printf("Frequency: %d\n", frequ);
		sprintf(s, "Index: %d; Symbol: %c [%d]; Frequency: %d", j, sym, sym, frequ);
		LOG(s);
	}

	LOGEXIT;
}
//
// end of file
//
