/*
 * Name: Sihan Xu
 * UNMID: 101851563
 * CS 341
 * Cache Lab - Part A
 */
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int VERBOSE_FLAG = 0;
int LRU_FLAG = 0;
int MRU_FLAG = 0;
int DEBUG = 0;

/*************************************************************************
 Your work will go in the below block!
**************************************************************************/
void updateOrder_LRU(struct Set *currSet, int recentlyAccessed){
	int numLines = currSet->rows;
	int i,j;
    for(i=0; i<numLines; i++){
        if(currSet -> orderOfAccess[i] == recentlyAccessed){
            for(j=i; j>=0; j--){
                currSet -> orderOfAccess[j] = currSet -> orderOfAccess[j-1];
            }
            currSet -> orderOfAccess[0] = recentlyAccessed;
            return;
        }
    }

}

void updateOrder_MRU(struct Set *currSet, int recentlyAccessed){
	int numLines = currSet->rows;
	int i,j,temp;
    for(i=(sizeof(currSet ->orderOfAccess)/sizeof(currSet ->orderOfAccess[0])); i > 0; i--){
        if(currSet -> orderOfAccess[i] != -1){
            for(j=i; j>=0; j--){
 
                temp = currSet -> orderOfAccess[j];
 
                if(j+1 < sizeof(currSet->orderOfAccess)/sizeof(currSet->orderOfAccess[0])){
                    currSet -> orderOfAccess[j+1] = temp;
                }
            }
            currSet -> orderOfAccess[0] = recentlyAccessed;
            return;
        }
    }
	for(i=0; i<numLines; i++){
        if(currSet -> orderOfAccess[i] == recentlyAccessed){
            for(j=i; j>=0; j--){
                currSet -> orderOfAccess[j] = currSet -> orderOfAccess[j-1];
            }
            currSet -> orderOfAccess[0] = recentlyAccessed;
            return;
        }
    }

}
/*************************************************************************
 Your work will go in the above block!
**************************************************************************/

int checkTag_LRU(int setIndex, int offset, unsigned long tag, struct Cache *myCache){

	struct Set *currSet = &myCache->mySets[setIndex];
	int i = 0;
	int lastAccessed;
	int numLines = currSet->rows;

	if(DEBUG){
		printf("Current Set: %d\n",setIndex );
		printf("Current offset: %d\n",offset);
		printf("Current tag: %ld\n",tag);
	}

	for(i = 0 ; i < numLines; i++){

		if(currSet->Lines[currSet->orderOfAccess[i]][0] == DEFAULT_CACHE_VAL){

			if(DEBUG){

				printf("Cache is empty: \n Before re-order:\n");
				printOrder(currSet);
			}

			updateOrder_LRU(currSet,currSet->orderOfAccess[i]);

			if(DEBUG){
				printf("After re-order:\n");
				printOrder(currSet);
			}

			currSet->Lines[i][0] = tag;
			return MISS;
		}else if(currSet->Lines[currSet->orderOfAccess[i]][0] == tag){
			if(DEBUG){
				printf("Tags match: \nBefore re-order:\n");
				printOrder(currSet);
			}
			updateOrder_LRU(currSet,currSet->orderOfAccess[i]);
			if(DEBUG){
				printf("After re-order:\n");
				printOrder(currSet);
			}
			return HIT;
		}
	}

	lastAccessed = currSet->orderOfAccess[numLines-1];
	currSet->Lines[lastAccessed][0] = tag;
	updateOrder_LRU(currSet,lastAccessed);

	return EVICTION + MISS;
}

int checkTag_MRU(int setIndex, int offset, unsigned long tag, struct Cache *myCache){

	struct Set *currSet = &myCache->mySets[setIndex];
	int i = 0;
	int mostRecentlyUsed;
	int numLines = currSet->rows;

	if(DEBUG){
		printf("Current Set: %d\n",setIndex );
		printf("Current offset: %d\n",offset);
		printf("Current tag: %ld\n",tag);
	}


	for(i = 0 ; i < numLines; i++){

		if(currSet->Lines[i][0] == DEFAULT_CACHE_VAL){

			if(DEBUG){

				printf("Cache is empty: \n Before re-order:\n");
				printOrder(currSet);
			}

			updateOrder_MRU(currSet,i);

			if(DEBUG){
				printf("After re-order:\n");
				printOrder(currSet);
			}

			currSet->Lines[i][0] = tag;
			return MISS;
		}else if(currSet->Lines[i][0] == tag){
			if(DEBUG){
				printf("Tags match: \nBefore re-order:\n");
				printOrder(currSet);
			}
			updateOrder_MRU(currSet,i);
			if(DEBUG){
				printf("After re-order:\n");
				printOrder(currSet);
			}
			return HIT;
		}
	}

	mostRecentlyUsed = currSet->orderOfAccess[0];
	currSet->Lines[mostRecentlyUsed][0] = tag;
	updateOrder_MRU(currSet,mostRecentlyUsed);

	return EVICTION + MISS;
}

int determineHitsAndMisses(struct Cache *myCache, char type,
														unsigned long address, int setBits, int offsetBits){

	int i = 0;
	int result = 0;
	int currSet  = 0;
	int iterations = 1;
	unsigned long currTag = 0;

	currSet = getbitValue(address, setBits, offsetBits,SET);
	currTag = getTag(address, setBits, offsetBits);

	if(strcmp(&type,"M") == 0 ){
		iterations = 2;
	}

	for(i = 0; i < iterations; i++){

		if(LRU_FLAG){
			result = checkTag_LRU(currSet, 0, currTag, myCache);
		}else{

			result = checkTag_MRU(currSet, 0, currTag, myCache);
		}
		updateMEH(myCache, result);
	}

	return result;
}


int main(int argc, char *argv[])
{
	char type;
	int i = 0;
	int s = 0;
	int b = 0;

	int numSets = 0;
	int numLines = 0;
	int blockSize = 0;
	int policyNum = 0;
	int traceValue = 0;
	unsigned long address;
	unsigned long traceSize = 0;
	char *strVal;
	FILE* stackTrace;

		/*Get arguments here!*/
    for(i = 0; i < argc; i++){

			if(strcmp(argv[i],"-h") == 0){
				printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t >tracefile>");
				return 0;
			}else if (strcmp(argv[i],"-v") == 0){
				VERBOSE_FLAG = 1;
			}else if (strcmp(argv[i],"-s") == 0){
				//set index bits
				s = atoi(argv[i+1]);
				numSets = 1 << s;
			}else if (strcmp(argv[i],"-E") == 0){
				//lines per set
				numLines = atoi(argv[i+1]);
			}else if (strcmp(argv[i],"-b") == 0){
				//Block size bits
				b = atoi(argv[i+1]);
				blockSize = 1 << b;
			}else if (strcmp(argv[i],"-p") == 0){
				//get policy number
				policyNum = atoi(argv[i+1]);
				if(policyNum == 1){
					LRU_FLAG = 1;
				}else if(policyNum == 2) {
					MRU_FLAG = 1;
				}
	    }else if (strcmp(argv[i],"-t") == 0){
				stackTrace = fopen(argv[i+1], "r");
			}
	}

	if (DEBUG){
		printf("Verbose: %d\n",VERBOSE_FLAG);
		printf("LRU Flag: %d\n",LRU_FLAG);
		printf("MRU Flag: %d\n",MRU_FLAG);
		printf("Number of sets: %d\n",numSets);
		printf("Number of lines: %d\n",numLines);
		printf("Block size: %d\n",blockSize);
	}

	/*Lets build an empty cache*/
	struct Cache *myCache = createCache(numSets,  numLines,  blockSize , MRU_FLAG);

	if(numSets && numLines && blockSize && (LRU_FLAG || MRU_FLAG)){



		while(fscanf(stackTrace, "%s %lx, %lx \n", &type, &address, &traceSize) != EOF){

			traceValue = determineHitsAndMisses(myCache, type, address, s , b);
			strVal = getStrRep(traceValue);

			if(VERBOSE_FLAG){
				printf("%c %lx,%lx %s \n", type, address, traceSize, strVal);
			}

		}
		fclose(stackTrace);

	}

  printSummary(myCache->hits,myCache->misses,myCache->evictions);
	freeCache(myCache);
	free(myCache);
  return 0;
}
