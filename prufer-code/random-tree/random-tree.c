//      cd Dropbox/vmware/cProgramming/discreteMath/tree


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printTree(int a[], int maxRandom){
	for(int i=0; i< maxRandom;++i){
		printf("%ld ",a[i]);
	};
	printf("\n");
}

void outputGraphviz(int paraPruferCode[],int pruferCode[], int soCanh){
	printf("graph test {\n");
	for(int i=0;i<soCanh;i++){
		printf("%ld -- %ld\n",paraPruferCode[i],pruferCode[i]);
	}
	printf("}\n");
}

void recursiveFindMin(int paraPruferCode[],int pruferCode[],int currentLoopPos,int currentPos,int *currentMin){
	for(int i=0;i<=currentPos;++i){
		if(i <= currentLoopPos && currentLoopPos > 0){
			if(*currentMin == paraPruferCode[i]){
				++(*currentMin);
				recursiveFindMin(paraPruferCode,pruferCode,currentLoopPos,i,currentMin);
			}
		}
		if(i >= currentLoopPos){
			if(*currentMin == pruferCode[i]){
				++(*currentMin);
				recursiveFindMin(paraPruferCode,pruferCode,currentLoopPos,i,currentMin);
			}
		}
		
	}
}

int main(void){
	int maxRandom;
	//printf("Type in maxRandom:\n");
	scanf("%ld",&maxRandom);
	int soCanh = maxRandom +1;
	int pruferCode[soCanh];
	int paraPruferCode[soCanh];
	srand(time(NULL));

	pruferCode[soCanh - 1]=0;
	for(int i=0;i <= soCanh -2; ++i ){
		pruferCode[i]= rand() % (soCanh); 
	};


	for(int i=0;i<soCanh;++i){
		int min = 1;
		recursiveFindMin(paraPruferCode,pruferCode,i,soCanh-1,&min);
		paraPruferCode[i]=min;
	}
	//printTree(paraPruferCode,soCanh);
	//printTree(pruferCode,soCanh);
	outputGraphviz(paraPruferCode,pruferCode,soCanh);
	return 0;
}