#include <stdio.h>
#include <math.h>
//
int main()
{
	FILE* fin = fopen( "out.txt" , "r" );
	char ch;
	char decoder[515] = {'\n'};
	char curr;
	char num[100];
	int index = 1;
	int decodeIndex = 1;
	int rowIndexes = 0;
	int n = 0;
	int numbytes;
	int numRowIndexes = 0;
	int firstLine = 1;
	float freq[256] = {0};
	float decodeBits = 0;
	float encodeBits = 0;
	float minBits = 0;
	while( 1 )
	{
		// printf( "%c" , ch );
		if(firstLine)
		{
			fscanf(fin, "%d", &numRowIndexes);
			firstLine = 0;
			// printf("%d\n", numRowIndexes);
			numbytes = fread( &ch , sizeof(char) , 1 , fin );
		}
		else
		{
			numbytes = fread( &ch , sizeof(char) , 1 , fin );
			if( numbytes == 0 ) break;
			n++;
			if(rowIndexes < numRowIndexes)
			{
				// printf("%c\n", ch);
				// printf("%s\n", "rowIndex<numRowIndexes");
				// printf("RowIndex: %d\n", rowIndexes);
				// printf("numRowIndexes: %d\n", numRowIndexes);
				if(ch == '\n')
				{
					decoder[index] = curr;
					rowIndexes++;
				}
				else if(ch =='0')
				{
					index = index * 2;
				}
				else if(ch == '1')
				{
					index = index * 2 + 1;
				}
				else
				{
					curr = ch;
					index = 1;
				}
			}
			else
			{
				encodeBits++;
				if(decoder[decodeIndex] != '\0')
				{
					// printf("%s\n", "LOLZ");
					printf("%c", decoder[decodeIndex]);
					freq[decoder[decodeIndex]]++;
					decodeIndex = 1;
					decodeBits++;
				}
				if(ch == '0')
				{
					decodeIndex = decodeIndex * 2;
				}
				else if(ch == '1')
				{
					decodeIndex = decodeIndex * 2 + 1;
				}
			}
		}
	}
	fclose( fin );
	printf( "\n" );
	printf( "%d\n" , n );
	printf( "\n" );
	float newDecodeBits = 8 * decodeBits;
	// printf("%f\n", newDecodeBits - encodeBits);
	// printf("encodeBits: %f\n", encodeBits);
	float ratio = (newDecodeBits-encodeBits)/newDecodeBits;
	printf("ratio: %f\n", ratio);
	int sum = 0;
	for(int i = 0; i < 256; i++)
	{
		if(freq[i] > 0)
		{
			sum += freq[i];
			minBits += freq[i] * -log2f(freq[i]/decodeBits);
		}
	}
	// printf( "%d\n" , sum);
	// printf( "%f\n" , decodeBits);
	printf( "minBits: %f\n" , minBits);

	return 0;
}
//
// end of file
//