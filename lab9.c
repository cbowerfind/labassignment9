#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// RecordNode
struct RecordNode
{
    struct RecordType* record;
    struct RecordNode* next;
};

// HashType
struct HashType
{
    struct RecordNode* record;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// Create a new RecordNode
struct RecordNode* createRecordNode(struct RecordType* record)
{
    struct RecordNode* node = (struct RecordNode*) malloc(sizeof(struct RecordNode));
    if (node == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    node->record = record;
    node->next = NULL;
    return node;
}

// Insert a new record into the hash table
void insert(struct RecordNode** ppRecordList, struct RecordType* record)
{
    struct RecordNode* node = createRecordNode(record);
    node->next = *ppRecordList;
    *ppRecordList = node;
}

// Free all nodes in the record list
void freeRecords(struct RecordNode* record)
{
    if (record != NULL)
    {
        freeRecords(record->next);
        free(record);
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i=0;i<hashSz;++i)
    {
        struct RecordNode* record = pHashArray[i].record;
        if (record != NULL)
        {
            printf("index %d -> ", i);
            while (record != NULL)
            {
                printf("%d, %c, %d -> ", record->record->id, record->record->name, record->record->order);
                record = record->next;
            }
            printf("\n");
        }
    }
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
int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSz = 100;
    struct HashType *pHashArray;
    int i, index;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Allocate memory for the hash table
    pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL) {
        printf("Cannot allocate memory\n");
        exit(-1);
    }

    // Initialize the hash table
    for (i = 0; i < hashSz; ++i) {
        pHashArray[i].record = NULL;
    }

    // Hash the records and insert into the hash table
    for (i = 0; i < recordSz; ++i) {
        index = hash(pRecords[i].id, hashSz) % hashSz;
        insert(&pHashArray[index].record, &pRecords[i]);
    }

    // Display the records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free memory
    for (i = 0; i < hashSz; ++i) {
        freeRecords(pHashArray[i].record);
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}


