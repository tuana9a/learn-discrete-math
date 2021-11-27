// cd Dropbox\vmware\cProgramming\discreteMath\mid-term-test\treeCompression


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <time.h>

int readNumberInFile(FILE *inputFile, char firstNotation){
	int result = firstNotation -'0';
	char c;
	while(isdigit(c = fgetc(inputFile))){
		result = result*10 + c - '0';
	};
	return result;
}

void printSideStorage(int apexNumber,int sideStorage[][2]){
	for(int i = 0;i<apexNumber-1;++i){
		printf("%d -- %d\n",sideStorage[i][0],sideStorage[i][1]);
	}
}


// ý tưởng mới, tức là mình sẽ sắp xếp được cái kho lưu trữ của mình đẹp như mình mong muốn khi đó sẽ xử lý rất nhanh
void copySide(int copied[][2], int origin[][2], int pos){
	copied[pos][0]=origin[pos][0];
	copied[pos][1]=origin[pos][1];
}

void reorganizeSideStorage(int apexNumber, int sideStorage[][2], int apexFrequency[]){

}
//hết ý tưởng mới


//version 0
void checkReduceFrequency(int apexNumber,int side,int apexFrequency[],int sideStorage[][2],int result[], int* pos_result){
	int oppositeSide;
	for(int i=0;i<apexNumber-1;++i){
		if(sideStorage[i][0] == side || sideStorage[i][1] == side) {
			oppositeSide = sideStorage[i][0] == side ? sideStorage[i][1] : sideStorage[i][0];
			//printf("%d -- %d\n",side,oppositeSide);
			sideStorage[i][0] = -1;
			sideStorage[i][1] = -1;
			break;
		}
	}

	result[(*pos_result)++] = oppositeSide;
	if(--apexFrequency[oppositeSide]==1 && oppositeSide < side && oppositeSide!=0) {
		checkReduceFrequency(apexNumber,oppositeSide,apexFrequency,sideStorage,result,pos_result);	
	};
}

int main(void){
	FILE *inputFile = fopen("input.txt","r");
	int apexNumber; //số đỉnh
	char c;
	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			apexNumber = readNumberInFile(inputFile,c);
			break;	
		}
	}

	int sideStorage[apexNumber-1][2];// mảng 2 chiều lưu cạnh
	
	int apexFrequency[apexNumber];// mảng 1 chiều lưu tần số của cạnh
	for(int i = 0;i<apexNumber;++i) apexFrequency[i] = 0;
	
	int step=0;
	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			//printf("%c\n",c);
			//printf("%d\n",readNumberInFile(inputFile,c));
			int temp =   readNumberInFile(inputFile,c);
			sideStorage[step/2][step++%2] = temp;
			apexFrequency[temp]++;	
		}
	}

	
	//printf("apexNumber: %d\nedgeStorage:\n",apexNumber);
	//printSideStorage(apexNumber,sideStorage);
	//for(int i = 0;i<apexNumber;++i) printf("side %d:%d\apexNumber",i,apexFrequency[i]);
	
	int result[apexNumber-2];
	step=0;
	for(int i = 1;i<apexNumber;++i){
		if(apexFrequency[i]==1){
			checkReduceFrequency(apexNumber,i,apexFrequency,sideStorage,result,&step);
		}
	}

	printf("Prufer Code: \n");
	for(int i=0;i<apexNumber-2;++i){
		printf("%d ", result[i]);
	}
	fclose(inputFile);
	return 0;
}