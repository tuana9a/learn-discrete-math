// cd  Dropbox\vmware\cProgramming\discreteMath\searchingOnGraph\findShortestPath


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <string.h>
#include <math.h>



int apexNumber, sideNumber;
int startApex, endApex;
int maxLength=0; // cái này để gán độ dài cho các cái ban đầu sao cho nó max nhất



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
void readGraphFromFile(FILE* inputGraph,int sideStorage[][2], int weightStorage[][apexNumber]){
	
	for(int i=0;i<apexNumber;++i) for(int j=0;j<apexNumber;++j) weightStorage[i][j]=0;

	for(int i=0;i<sideNumber;++i) {
		int temp = getOneIntFromFile(inputGraph);
		sideStorage[i][0] = temp;

		temp = getOneIntFromFile(inputGraph);
		sideStorage[i][1] = temp;

		temp = getOneIntFromFile(inputGraph);
		weightStorage[sideStorage[i][0]][sideStorage[i][1]] = temp;
		weightStorage[sideStorage[i][1]][sideStorage[i][0]] = temp;

		maxLength += temp;
	}
	maxLength++;
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
	for(int i=0;i<apexNumber;++i) {
		for(int j=0;j<apexNumber;++j) printf(j==0?"  %d ":"%d ",weightStorage[i][j]);
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



//NOTE lưu trữ dùng danh sách kề cái này chỉ để chuẩn bị cho các bước sau này
struct neighborApex {
	int apex;
	struct neighborApex * next; 
};
typedef struct neighborApex neighborApex;

void insertNeighborApex(neighborApex* rootApex, int whichApex) {
	neighborApex* temp = rootApex;
	while((*temp).next != NULL) temp = (*temp).next;
	neighborApex* new = (neighborApex*)malloc(sizeof(neighborApex));
	(*temp).next = new;
	(*new).apex = whichApex;
	(*new).next = NULL;
}
void readSideStorage (int sideStorage[][2], neighborApex *apexStorage[]) {
	for(int i=0;i<apexNumber;++i) {
		apexStorage[i] = (neighborApex*)malloc(sizeof(neighborApex));
		(*apexStorage[i]).apex = i;
		(*apexStorage[i]).next = NULL;
	}
	for(int i=0;i<sideNumber;++i) {
		//printf("%d -- %d\n",sideStorage[i][0],sideStorage[i][1]);
		insertNeighborApex(apexStorage[sideStorage[i][0]],sideStorage[i][1]);
		insertNeighborApex(apexStorage[sideStorage[i][1]],sideStorage[i][0]);
	}
}
void printApexNeighbor(neighborApex* rootApex) {
	neighborApex *temp = (*rootApex).next;
	printf("  %d -- {",(*rootApex).apex);
	while(temp != NULL) {
		printf((*temp).next==NULL?"%d}\n": "%d,",(*temp).apex);
		temp = (*temp).next;
	}	
}
void printApexStorage(neighborApex* apexStorage[]) {
	neighborApex* temp;

	printf("\napexStorage {\n");
	for(int i=0;i<apexNumber;++i) {
		printApexNeighbor(apexStorage[i]);
	}
	printf("}\n");
}



//NOTE dùng BFS(hàng đợi) để tìm đường ngắn nhất
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
void popQueue(nodeQueue** startQueue) {
	//phải dùng con trỏ cấp 2 vì mình đang thay đổi giá trị của nó, trỏ sang vùng nhớ khác :V
	nodeQueue* temp = (*startQueue);
	*startQueue = (**startQueue).next;
	free(temp);
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
void popQueueSpecific(nodeQueue * startQueue, int pos) {
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



//NOTE dùng cây để lưu kết quả, lưu cách này thi chỉ lưu được một đường
struct treeNode {
	int apex;
	struct treeNode* child;
	struct treeNode* sibling;
};
typedef struct treeNode treeNode;

treeNode* newResultNode(int whichApex) {
	treeNode* new = (treeNode*)malloc(sizeof(treeNode));
	(*new).apex = whichApex;
	(*new).child = NULL;
	(*new).sibling = NULL;
	return new;
}
void deleteChild(treeNode* resultTree[], treeNode* currentNode, int childToDelete, int* stop) {
	if((*stop) == 0) return;
	treeNode* tempChild = (*currentNode).child;
	treeNode* prevChild;
	int childCount=0;

	while(tempChild != NULL) {
		if((*tempChild).apex == childToDelete) {
			*stop = 0;

			if(childCount == 0) (*currentNode).child = (*tempChild).sibling;
			else (*prevChild).sibling = (*tempChild).sibling;
			
			(*tempChild).sibling = NULL;
			free(tempChild);
			return;  
		}
		childCount++;
		prevChild = tempChild;
		tempChild = (*tempChild).sibling;
	}

	if((*currentNode).child != NULL) deleteChild(resultTree,(*currentNode).child,childToDelete,stop);
	if((*currentNode).sibling != NULL) deleteChild(resultTree,(*currentNode).sibling,childToDelete,stop);
}
void appendChild(treeNode* resultTree[], int whichChild, int dad) {
	treeNode* tempChild = (*resultTree[dad]).child;
	if(tempChild == NULL) {
		(*resultTree[dad]).child = resultTree[whichChild];
		return;
	}
	while((*tempChild).sibling != NULL) {
		tempChild = (*tempChild).sibling;
	}
	(*tempChild).sibling = resultTree[whichChild];
}
void changeDad(treeNode* resultTree[],int child, int dad) {
	int stop=1;
	deleteChild(resultTree,resultTree[startApex],child,&stop);
	appendChild(resultTree,child,dad);
}



void treeTraversing(treeNode* resultTree[],treeNode* currentNode) {
	if(currentNode != NULL) {
		printf("A: %d\n",(*currentNode).apex);
		treeTraversing(resultTree,(*currentNode).child);
		treeTraversing(resultTree,(*currentNode).sibling);
	}
}
void printPath(treeNode* resultTree[], treeNode* currentNode, nodeQueue* startQueue,int step , int* stop) {
	if(*stop == 0) return;
	if(currentNode != NULL) {
		if(step!=0) insertToQueue(startQueue,(*currentNode).apex);//printf("step: %d, insert: %d\n",step,(*currentNode).apex);//printQueue(startQueue);
		if((*currentNode).apex == endApex) {
			printf("\n(FOUND)\n");
			printQueue(startQueue);
			popQueueSpecific(startQueue,step);//printf("pop %d\n",step);//printQueue(startQueue);
			*stop=0;
			return;
		}

		printPath(resultTree,(*currentNode).child,   startQueue,step+1,stop);

		popQueueSpecific(startQueue,step);//printf("pop %d\n",step);//printQueue(startQueue);

		printPath(resultTree,(*currentNode).sibling, startQueue,step,stop);
		//popQueueSpecific(startQueue,step);//printf("pop %d\n",step);
	}
}





//------------------------------------------TODO---TODO---TODO---TODO---TODO---TODO---TODO---TODO---TODO---TODO---------------------------------------------
void findShortestPath(neighborApex* apexStorage[], nodeQueue* nodeQueuePointer, int distance[], int weightStorage[][apexNumber],treeNode* resultTree[]) {
	while(nodeQueuePointer != NULL) {  //still has node in queue
		neighborApex* temp = apexStorage[(*nodeQueuePointer).apex];
		int currentApex = (*temp).apex;//this var is to store current root apex because when use it, it lost its pointer
		while((*temp).next != NULL) {  //still have neighbor

			int nearbyApex = (*(*temp).next).apex;
			int newDistance = distance[currentApex] + weightStorage[currentApex][nearbyApex];

			if(newDistance < distance[nearbyApex]) {
				printf("[%d] spread [%d] old:%2d - new:%2d",currentApex,nearbyApex,distance[nearbyApex],newDistance);
				printf(" --> Insert [%d]\n",nearbyApex);
				
				distance[nearbyApex] = newDistance;
				insertToQueue(nodeQueuePointer,nearbyApex);
				changeDad(resultTree,nearbyApex,currentApex);
				
				printf("CHANGE DAD of [%d] to [%d]\n",nearbyApex,currentApex);
				// printf("distance: ");printArray(distance,apexNumber);
				printf("\n");
				
				//treeTraversing(resultTree,resultTree[0]);
			} else if(newDistance == distance[nearbyApex]) {
				//printf(" Same Distance\n"); 

				//FIXIT nếu sau này cải tiến sẽ đi được nhiều đường chứ không chỉ một nữa

				//printf("\nAPPEND CHILD  [%d] to [%d]\n",nearbyApex,currentApex);
				//appendChild(resultTree,nearbyApex,currentApex);
			} else {
				//printf(" Fail   %2d\n",nearbyApex);
			}
			temp = (*temp).next;
		}
		printQueue(nodeQueuePointer);
		//printf("distance: ");//printArray(distance,apexNumber);printf("\n");
		popQueue(&nodeQueuePointer);
	}
}


int main(int argc, char const *argv[]) {
//--------------------------------------- INPUT ------------------------------------
	FILE* inputGraph = fopen("inputGraph.txt","r");

	apexNumber = getOneIntFromFile(inputGraph);	//apexNumber
	sideNumber = getOneIntFromFile(inputGraph);	//sideNumber
	startApex=0;endApex=5;



	int sideStorage[sideNumber][2];				//sideStorage     0:apex1 |  1:apex2  |  3:weight of this side
	int weightStorage[apexNumber][apexNumber];	//weightStorage
	

	readGraphFromFile(inputGraph,sideStorage,weightStorage);	//store sideStorage and weightStorage
	
//--------------------- PARSING FILE and INIT SOME STRUCTURE ------------------------

	neighborApex *apexStorage[apexNumber];
	readSideStorage(sideStorage,apexStorage);	//store apexStorage
	
	int distance[apexNumber];					//innnit distance
	for(int i=0;i<apexNumber;++i) {
		distance[i] = maxLength; // thuc ra la maxLength + 1 san roi
		if(i == startApex) distance[i] = 0;
	}


	treeNode* resultTree[apexNumber];
	for(int i=0;i<apexNumber;++i){
		resultTree[i] = newResultNode(i);
	}
	
//------------------------------------ PRINT INPUT ---------------------------------

	printf("=====================================================================INPUT==============================================================\n\n\n");
	printf("ApexNumber: %d\nSideNumber: %d\n",apexNumber,sideNumber);
	printSideStorage(sideStorage);
	printWeightStorage(weightStorage);
	printGraph(sideStorage,weightStorage);
	printApexStorage(apexStorage);				
	printf("\nmaxLength+1: %d\n\n", maxLength);
	printf("distance: ");
	printArray(distance,apexNumber);

//------------------------------------ PROCESSING ------------------------------------

	printf("\n\n\n=============================================================PROCESSING===========================================================\n\n\n");

	nodeQueue* nodeQueuePointer = newNodeQueue(startApex);

	printQueue(nodeQueuePointer);
	printf("distance: ");
	printArray(distance,apexNumber);
	printf("\n");
	findShortestPath(apexStorage,nodeQueuePointer,distance,weightStorage,resultTree);

	//void printPath(treeNode* resultTree[], treeNode* currentNode, nodeQueue* startQueue,int step ,int * stop) {

	printf("\n\n=================================================================PrintPath===========================================================\n");

	nodeQueue* startResult = newNodeQueue(startApex);


	int stop = 1;
	printPath(resultTree,resultTree[startApex],startResult,0,&stop);
	


	//treeTraversing(resultTree, resultTree[startApex]);


	printf("\nSUCCESS!\n");
	return 0;
}