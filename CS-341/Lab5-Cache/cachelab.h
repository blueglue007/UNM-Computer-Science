/*
 * cachelab.h - Prototypes for Cache Lab helper functions
 */

#ifndef CACHELAB_TOOLS_H
#define CACHELAB_TOOLS_H
#define MAX_TRANS_FUNCS 100
#define OFFSET 2
#define SET 1
#define MISS 1
#define EVICTION 2
#define HIT 4
#define DEFAULT_CACHE_VAL -1
#define BYTES_4 4



typedef struct trans_func{
  void (*func_ptr)(int M,int N,int[N][M],int[M][N]);
  char* description;
  char correct;
  unsigned int num_hits;
  unsigned int num_misses;
  unsigned int num_evictions;
} trans_func_t;

typedef struct Set{
	int rows;
	int cols;
	unsigned long **Lines;
	int *orderOfAccess;

}Set;

typedef struct Cache{
	int numSets;
	int numLines;
	int blockSize;
	int misses;
	int evictions;
	int hits;
	struct Set *mySets;

}Cache;
/*************************************************************************
 Part A
**************************************************************************/
/*
 * printSummary - This function provides a standard way for your cache
 * simulator * to display its final hit and miss statistics
 */
void printSummary(int hits,  /* number of  hits */
				  int misses, /* number of misses */
				  int evictions); /* number of evictions */

unsigned long getTag(int address, int setBits, int offsetBits);

int getbitValue(int address, int setBits, int offsetBits, int type);

void printOrder(struct Set *currSet);

char* getStrRep(int value);

struct Set* createSet(int lines, int bSize,int mru_flag);
void freeSet(struct Set *currSet);
struct Cache* createCache( int sets, int lines, int bSize,int mru_flag);
void freeCache(struct Cache *myCache);
void printSet(struct Set *currSet);
void printCacheData(struct Cache *myCache);
void updateMEH(struct Cache *myCache,int result);
/*************************************************************************
 Part B
**************************************************************************/
/* Fill the matrix with data */
void initMatrix(int M, int N, int A[N][M], int B[M][N]);

/* The baseline trans function that produces correct results. */
void correctTrans(int M, int N, int A[N][M], int B[M][N]);

/* Add the given function to the function list */
void registerTransFunction(
    void (*trans)(int M,int N,int[N][M],int[M][N]), char* desc);

#endif /* CACHELAB_TOOLS_H */
