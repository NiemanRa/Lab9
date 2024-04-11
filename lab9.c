#include <stdio.h>
#include <stdlib.h>

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
} RecordType;

// Fill out this structure
typedef struct HashType // NODE
{
	struct RecordType *recordsptr;
	struct HashType *next;
} HashType;

// Compute the hash function
int hash(int x)
{
	return x % 30;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;

		}

		fclose(inFile);
	}

	return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format: index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
	{
		// If index is occupied with any records, print all
		printf("Hash ID: %d\n", i);

		if (pHashArray[i] == NULL)
		{
			printf("\tNothing here!\n");
			printf("\n");
			continue;
		}

		struct HashType* hashCurrent;
		struct HashType* hashPrevious;

		hashCurrent = pHashArray[i];

		while (hashCurrent != NULL)
		{
			printf("\t %d %c %d\n", hashCurrent->recordsptr->id, hashCurrent->recordsptr->name, hashCurrent->recordsptr->order);

			hashPrevious = hashCurrent;
			hashCurrent = hashCurrent->next;

			free(hashPrevious);
		}

		printf("\n");
	}

	free(pHashArray);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	struct HashType** hashArray; 
	hashArray = (HashType**) calloc(30, sizeof(HashType*));
	struct HashType* hashCurrent;
	struct HashType* hashPrevious;


	for (int i = 0; i < recordSz; i++)
	{
		int hashID = hash(pRecords[i].id);

		hashCurrent = hashArray[hashID];

		if (hashCurrent == NULL)
		{
			hashArray[hashID] = (HashType*) calloc(recordSz, sizeof(HashType));
			hashArray[hashID]->recordsptr = &pRecords[i];

			continue;
		}

		while (hashCurrent != NULL)
		{
			hashPrevious = hashCurrent;
			hashCurrent = hashCurrent->next;

		}

		hashCurrent = (HashType*) calloc(recordSz, sizeof(HashType));
		hashCurrent->recordsptr = &pRecords[i];
		hashPrevious->next = hashCurrent;
	}

	displayRecordsInHash(hashArray, 30);

	free(pRecords);

	return 1;
}