/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cachelab.h"
#include <time.h>

trans_func_t func_list[MAX_TRANS_FUNCS];
int func_counter = 0;

/*************************************************************************
 Part A
**************************************************************************/
/*
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded.
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/*
 * getTag -
 */
unsigned long getTag(int address, int setBits, int offsetBits){

	unsigned long tempAdd = address;
	unsigned long tag =  tempAdd >> (setBits + offsetBits);

	return tag;
}

/*
 * getbitValue - returns the set index
 */
int getbitValue(int address, int setBits, int offsetBits, int type){

  int setIndex = 0;
  int mask;

	if(type == SET ){ //get set number

		mask =  ~((~0) << setBits);
		setIndex = address >> offsetBits;
		setIndex = setIndex & mask;

	}else if(type == OFFSET){// else get offset num

		mask = ~((~0) << offsetBits);
		setIndex = address;
		setIndex = setIndex & mask;
	}

	return setIndex;

}

/*
 * printOrder - prints the order of your array of lines!
 */
void printOrder(struct Set *currSet){
	int i = 0;
	printf("Current Order: ");
	for(i = 0; i < currSet->rows; i++){
		printf("%d ", currSet->orderOfAccess[i]);
	}
    printf("\n");
}

/*
 * getStrRep - returns the string value of which outcome
 */
char* getStrRep(int value){

	char *result;
	switch(value){
		case 1:
			result = "miss";
			break;
		case 2:
			result = "eviction";
			break;
		case 3:
			result = "miss eviction";
			break;
		case 4:
			result = "hit";
			break;
		case 5:
			result = "miss hit";
			break;
		case 7:
			result = "miss eviction hit";
			break;
		case 8:
			result = "hit hit";
			break;

	}
	return result;
}

/*
 *
 */
struct Set* createSet(int lines, int bSize,int mru_flag){

	struct Set *currSet = malloc(sizeof(struct Set));
	int i, j = 0;
	int numCols;
	if(bSize > 2){
		numCols = bSize/BYTES_4;
	}else{
		numCols = 2;
	}

	currSet->orderOfAccess = malloc(sizeof(int) * lines);
	currSet->rows = lines;
	currSet->cols = numCols;
	currSet->Lines = (unsigned long **)malloc(lines * numCols * sizeof(unsigned long));

	for(j = 0; j < lines; j ++){

		currSet->Lines[j] = (unsigned long *) malloc(numCols * sizeof(unsigned long));
    if(mru_flag){
      currSet->orderOfAccess[j] = DEFAULT_CACHE_VAL;
    }else{
      currSet->orderOfAccess[j] = j;
    }

	}

	for(i = 0; i < lines; i++){
		for(j = 0; j < numCols; j ++){
			 currSet->Lines[i][j] = DEFAULT_CACHE_VAL;
		}
	}
	return currSet;
}
/*
 *
 */
void freeSet(struct Set *currSet){

	int rows = currSet->rows;
	int i= 0;
	for(i = 0; i < rows; i++){
		free(currSet->Lines[i]);
	}
	free(currSet->Lines);
	free(currSet->orderOfAccess);
	//free(currSet);
}
/*
 *
 */
struct Cache* createCache( int sets, int lines, int bSize,int mru_flag){
	struct Cache *myCache = malloc(sizeof(struct Cache));

	myCache->numSets = sets;
	myCache->numLines = lines;
	myCache->blockSize = bSize;
	myCache->misses = 0;
	myCache->evictions = 0;
	myCache->hits = 0;

	myCache->mySets = malloc(sets * sizeof(struct Set));

	int i = 0;
	for(i = 0;i < sets; i++){

		struct Set *newSet = createSet(lines, bSize,mru_flag);
		myCache->mySets[i] = *newSet;
	}

	return myCache;
}
/*
 *
 */
void freeCache(struct Cache *myCache){

	int i = 0;
	int rows = myCache->numSets;

	for(i = 0; i < rows; i++){

		freeSet(&myCache->mySets[i]);
	}

	free(myCache->mySets);
	//free(myCache);
}
/*
 *
 */
void printSet(struct Set *currSet){

	int rows = currSet->rows;
	int cols = currSet->cols;
	int i, j = 0;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j ++){

			printf("%lx ", currSet->Lines[i][j]);
		}
		printf("\n");
	}
}
/*
 *
 */
void printCacheData(struct Cache *myCache){

	int i = 0;
	int rows = myCache->numSets;

	printf("Cache Info: \n");
	printf("#ofSets: %d\n",myCache->numSets);
	printf("#ofLines: %d\n",myCache->numLines);
    printf("blockSize: %d\n",myCache->blockSize);

	for(i = 0; i < rows; i++){

		printf("Set %d \n", i);
		printSet(&myCache->mySets[i]);

	}
}


void updateMEH(struct Cache *myCache,int result){

	switch(result){
		case MISS:
			myCache->misses += 1;
			break;
		case EVICTION + MISS:
			myCache->evictions += 1;
			myCache->misses += 1;
			break;
		case HIT:
			myCache->hits +=1;
			break;
	}

}

/*************************************************************************
 Part B
**************************************************************************/
/*
 * initMatrix - Initialize the given matrix
 */
void initMatrix(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
            B[j][i]=rand();
        }
    }
}

void randMatrix(int M, int N, int A[N][M]) {
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
        }
    }
}

/*
 * correctTrans - baseline transpose function used to evaluate correctness
 */
void correctTrans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}



/*
 * registerTransFunction - Add the given trans function into your list
 *     of functions to be tested
 */
void registerTransFunction(void (*trans)(int M, int N, int[N][M], int[M][N]),
                           char* desc)
{
    func_list[func_counter].func_ptr = trans;
    func_list[func_counter].description = desc;
    func_list[func_counter].correct = 0;
    func_list[func_counter].num_hits = 0;
    func_list[func_counter].num_misses = 0;
    func_list[func_counter].num_evictions =0;
    func_counter++;
}
