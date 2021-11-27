// cd Dropbox\vmware\cProgramming\discreteMath\maxFlowNetwork



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <string.h>
#include <math.h>














int apexNumber, sideNumber;
int startApex, endApex;
int maxWeightTotal;


//NOTE các hàm chỉ dùng để input đầu váo
int readNumberInFile(FILE *inputFile, char firstNotation){
	int result = firstNotation -'0';
	char c;
	while(isdigit(c = fgetc(inputFile))){
		result = result*10 + c - '0';
	};
	return result;
}
int getOneIntFromFile(FILE* inputFile){
	int result=0;
	char c;
	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			return readNumberInFile(inputFile,c);
		}
	}
	return EOF;
}
void readGraphFromFile(FILE* inputGraph,int sideStorage[][2], int maxWeightStorage[][apexNumber], int neighborStorage[][apexNumber]){
	maxWeightTotal = 0;
	for(int i=0;i<sideNumber;++i) {
		int temp;

		temp = getOneIntFromFile(inputGraph);
		sideStorage[i][0] = temp;

		temp = getOneIntFromFile(inputGraph);
		sideStorage[i][1] = temp;

		temp = getOneIntFromFile(inputGraph);
		maxWeightStorage[sideStorage[i][0]][sideStorage[i][1]] = temp;
		//maxWeightStorage[sideStorage[i][1]][sideStorage[i][0]] = temp;

		maxWeightTotal = maxWeightTotal>temp?maxWeightTotal:temp;

		neighborStorage[sideStorage[i][0]][sideStorage[i][1]] = 1;
	}
}



void copyOneDimArray(int length,int copied[],int origin[]) {
	for(int i=0;i<length;++i) {
		copied[i] = origin[i];
	}
}
void copyTwoDimArray(int length,int height,int copied[][length],int origin[][length]) {
	for(int i=0;i<height;++i) for(int j=0;j<length;++j) copied[i][j] = origin[i][j];
}
void initOneDimArray(int length, int a[]) {
	for(int i=0;i<length;++i) a[i] = 0;
}
void initTwoDimArray(int length, int heigth, int a[][length]) {
	for(int i=0;i<length;++i) for(int j=0;j<length;++j) a[i][j] = 0;
}


void printSideStorage(int sideStorage[][2]){
	printf("\nsideStorage {\n");
	for(int i = 0;i<sideNumber;++i){
		printf("   %d -- %d\n",sideStorage[i][0],sideStorage[i][1]);
	}
	printf("}\n");
}
void printWeightStorage(int weightStorage[][apexNumber]) {
	printf("\nweightStorage {\n");
	for(int i=0;i<apexNumber;++i) printf(i==0?"     %2d ":" %2d ",i);
	printf("\n");
	for(int i=0;i<apexNumber;++i) printf(i==0?"      | ":"  | ");
	printf("\n");
	for(int i=0;i<apexNumber;++i) {
		printf("%2d - ",i);
		for(int j=0;j<apexNumber;++j) printf(j==0?"%2d ":" %2d ",weightStorage[i][j]);
			printf("\n");
	}
	printf("}\n");
}
void printGraph(int sideStorage[][2],int weightStorage[][apexNumber]) {
	printf("\nGraph {\n");
	for(int i=0;i<sideNumber;++i) {
		printf("  %d--%d [%d]\n",sideStorage[i][0],sideStorage[i][1],weightStorage[sideStorage[i][0]][sideStorage[i][1]]);
	}
	printf("}\n");
}
void printArray(int a[], int length) {
	for(int i=0;i<length;++i) {
		printf(i==length-1?"[%d]:%2d":"[%d]:%2d ",i,a[i]);
	}
	printf("\n");
}

struct nodeQueue {
	int apex;
	struct nodeQueue* next;
};
typedef struct nodeQueue nodeQueue;

nodeQueue* newNodeQueue(int newApex) {
	nodeQueue* new = (nodeQueue*)malloc(sizeof(nodeQueue));
	(*new).apex = newApex;
	(*new).next = NULL;
	return new;
}
void insertToQueue(nodeQueue* startQueue, int newApex){
	nodeQueue* temp = startQueue;
	while((*temp).next != NULL) temp = (*temp).next;
	(*temp).next = newNodeQueue(newApex);
}
void popQueueStart(nodeQueue** startQueue) {
	//phải dùng con trỏ cấp 2 vì mình đang thay đổi giá trị của nó, trỏ sang vùng nhớ khác :V
	nodeQueue* temp = (*startQueue);
	*startQueue = (**startQueue).next;
	free(temp);
}
void popQueueEnd(nodeQueue* startQueue) {
	nodeQueue* temp = startQueue;
	while(temp!= NULL) {
		nodeQueue* nextTemp = (*temp).next;
		if((*nextTemp).next == NULL) {
			free(nextTemp);
			(*temp).next = NULL;
			return;
		}
	}
}
void popQueueSpecific(nodeQueue* startQueue, int pos) {
	int i = 0;
	nodeQueue* temp = startQueue;
	nodeQueue* prevNode = startQueue;
	while((*temp).next != NULL && i < pos) {
		prevNode = temp;
		temp = (*temp).next;
		++i;
	}
	(*prevNode).next = (*temp).next;
	free(temp);
}
int checkExistInQueue(nodeQueue* startQueue, int whichApex) {
	nodeQueue* temp = startQueue;
	while(temp != NULL) {
		if((*temp).apex == whichApex) return 1;
		temp = (*temp).next;
	}
	return 0;
}
void printQueue(nodeQueue* startQueue) {
	nodeQueue* temp = startQueue;
	printf("InQueue |");
	do{
		printf((*temp).next==NULL?"%d":"%d -> ",(*temp).apex);
		temp = (*temp).next;
	}
	while(temp!=NULL);
	printf("|\n");
}



void maxFlowNetwork(int currentApex, int step, nodeQueue* startQueue, int tempWeightStorage[][apexNumber]){
	//printf("step:%d  apex:%d  \n",step,currentApex);

	printf("running\n");

	if(currentApex != startApex) insertToQueue(startQueue,currentApex);
	//printQueue(startQueue);

	if(currentApex == endApex) {
		nodeQueue* tempNode = startQueue;
		nodeQueue* nextTempNode;
		int tempMin = maxWeightTotal;

		while((*tempNode).next != NULL) {
			nextTempNode = (*tempNode).next;

			int remainWeight = tempWeightStorage[(*tempNode).apex][(*nextTempNode).apex];
			
			//printf("%d", remainWeight);
			tempMin = tempMin<remainWeight?tempMin:remainWeight;
			tempNode = (*tempNode).next;
		}
		//if(tempMin == 0) return;
		tempNode = startQueue;
		while((*tempNode).next != NULL) {
			nextTempNode = (*tempNode).next;

			tempWeightStorage[(*nextTempNode).apex][(*tempNode).apex] += tempMin;
			tempWeightStorage[(*tempNode).apex][(*nextTempNode).apex] -= tempMin;

			tempNode = (*tempNode).next;
		}
		printQueue(startQueue);
		printf("added: %d\n",tempMin);
		printWeightStorage(tempWeightStorage);
		printf("==========================================\n");
		// printf("(found)\n");
		return;
	}
	//NOTE nếu đổi thứ tự duyệt thì được một kết quả khác
	for(int i=0;i<apexNumber;++i) {
	//for(int i=apexNumber-1;i>=0;--i) {
		if(tempWeightStorage[currentApex][i] != 0) {
			if(!checkExistInQueue(startQueue,i)) {
				maxFlowNetwork(i,step+1,startQueue,tempWeightStorage);
				popQueueSpecific(startQueue,step+1);
				//printf("pop at step: %d - apex:%d\n\n",step+1,i);
			}
		}
	}
}
//convert ngược đồ thị tăng luồng về dạng bình thường
void convertResidualToNormal(int residualGraph[][apexNumber], int neighborStorage[][apexNumber]) {
	for(int i=0;i<apexNumber;++i) for(int j=0;j<apexNumber;++j) {
		if(neighborStorage[i][j] != 0) neighborStorage[i][j] = residualGraph[j][i];
	}
}


int main(int argc, char const *argv[])
{
	//--------------------------- INPUT ------------------------------
	FILE* inputGraph = fopen("inputGraph.txt","r");

	apexNumber = getOneIntFromFile(inputGraph);
	sideNumber = getOneIntFromFile(inputGraph);
	startApex = 0;
	endApex = 5;

	int sideStorage[sideNumber][2];					//lưu danh sách cạnh trong file

	int neighborStorage[apexNumber][apexNumber];   	//lưu cạnh ra vào dúng của đồ thị
	int tempWeightStorage[apexNumber][apexNumber];	//lưu trạng thái tạm thời của đồ thị

	initTwoDimArray(apexNumber,apexNumber,neighborStorage);
	initTwoDimArray(apexNumber,apexNumber,tempWeightStorage);

	readGraphFromFile(inputGraph,sideStorage,tempWeightStorage,neighborStorage);

	//printSideStorage(sideStorage);
	//printWeightStorage(maxWeightStorage);
	//printWeightStorage(neighborStorage);
	//printWeightStorage(tempWeightStorage);
	//printf("maxWeightTotal: %d\n",maxWeightTotal);

	printf("=================================================================================\n\n");

	nodeQueue* startQueue = newNodeQueue(startApex);
	maxFlowNetwork(startApex,0,startQueue,tempWeightStorage);

	printf("=================================================================================\n\n");
	printf("\n");
	printWeightStorage(tempWeightStorage);
	convertResidualToNormal(tempWeightStorage,neighborStorage);
	printWeightStorage(neighborStorage);

	printf("SUCESS END");
	return 0;
}



























//NOTE lưu trữ dùng danh sách kề cái này chỉ để chuẩn bị cho các bước sau này
// struct neighborApex {
// 	int apex;
// 	struct neighborApex * next; 
// };
// typedef struct neighborApex neighborApex;

// void insertNeighborApex(neighborApex* rootApex, int whichApex) {
// 	neighborApex* temp = rootApex;
// 	while((*temp).next != NULL) temp = (*temp).next;
// 	neighborApex* new = (neighborApex*)malloc(sizeof(neighborApex));
// 	(*temp).next = new;
// 	(*new).apex = whichApex;
// 	(*new).next = NULL;
// }
// void readSideStorage (int sideStorage[][2], neighborApex *apexStorage[]) {
// 	for(int i=0;i<sideNumber;++i) {
// 		//printf("%d -- %d\n",sideStorage[i][0],sideStorage[i][1]);
// 		insertNeighborApex(apexStorage[sideStorage[i][0]],sideStorage[i][1]);
// 		insertNeighborApex(apexStorage[sideStorage[i][1]],sideStorage[i][0]);
// 	}
// }
// void printApexNeighbor(neighborApex* rootApex) {
// 	neighborApex *temp = (*rootApex).next;
// 	printf("  %d -- {",(*rootApex).apex);
// 	while(temp != NULL) {
// 		printf((*temp).next==NULL?"%d}\n": "%d,",(*temp).apex);
// 		temp = (*temp).next;
// 	}	
// }
// void printApexStorage(neighborApex* apexStorage[]) {
// 	neighborApex* temp;

// 	printf("\napexStorage {\n");
// 	for(int i=0;i<apexNumber;++i) {
// 		printApexNeighbor(apexStorage[i]);
// 	}
// 	printf("}\n");
// }


