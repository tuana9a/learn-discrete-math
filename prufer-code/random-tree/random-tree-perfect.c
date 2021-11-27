//      cd Dropbox/vmware/cProgramming/discreteMath/tree


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printTree(int a[], int maxRandom){
	for(int i=0; i< maxRandom;++i){
		printf("%3d ",a[i]);
	};
	printf("\n");
}

void outputGraphviz(int paraPruferCode[],int pruferCode[], int soCanh){
	printf("graph ditme {\n");
	for(int i=0;i<soCanh;i++){
		printf("%d -- %d\n",paraPruferCode[i],pruferCode[i]);
	}
	printf("}\n");
}



int main(void){
	int soCanh;
	scanf("%d",&soCanh);
	int soDinh = soCanh +1;

	int *pruferCode=(int*)malloc(soCanh*sizeof(int));
	int *paraPruferCode=(int*)malloc(soCanh*sizeof(int));

	srand(time(NULL));
	for(int i=0;i < soCanh-1; ++i ){
		pruferCode[i]= rand() % soDinh; 
	};
	pruferCode[soCanh - 1]=0;
	
	int *freq=(int*)calloc((soDinh),sizeof(int));
	
	for(int i=0;i<soCanh;++i){
		++freq[pruferCode[i]];
	}

	int zeroCount=0;
	for(int i=0;i<soDinh;++i){
		if(freq[i]==0)
			++zeroCount;
	}

	int *peakZero = (int*)calloc(zeroCount+1,sizeof(int));
	int j = 0;
	for(int i=0;i<soDinh;++i){
		if(freq[i]==0)
			peakZero[j++]=i;//peakZero[j] will have value which is the peak that have freqency 0
	}
	peakZero[j]=soDinh+1;

	//printf("prufer:   ");printTree(pruferCode,soCanh);
	//printf("freq:     ");printTree(freq,soDinh);
	//printf("peakZero: ");printTree(peakZero,zeroCount+1);

	int k=0;
	for(int i = 0;i<soCanh;++i){
		int min = peakZero[k];
		freq[min]=1;
		freq[pruferCode[i]]-=1;
		if(freq[pruferCode[i]]==0){
			if(pruferCode[i] < peakZero[k+1]){
				peakZero[k]=pruferCode[i];
			} else {
				peakZero[k] = peakZero[k+1];
				peakZero[k+1]=pruferCode[i];
				
			}
		} else {
			k++;
		};
		//pruferCode[i]=min;
		paraPruferCode[i]=min;
	}


	//printf("para:     ");printTree(paraPruferCode,soCanh);
	//printf("prufer:   ");printTree(pruferCode,soCanh);
	outputGraphviz(paraPruferCode,pruferCode,soCanh);

	free(freq);
	free(peakZero);
	free(pruferCode);
	free(paraPruferCode);
	return 0;
}