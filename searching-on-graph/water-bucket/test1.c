//			cd Dropbox/vmware/cProgramming/discreteMath/searching


#include<string.h>
#include<stdlib.h>
#include<stdio.h>



#define BUCKET_NUMBER_DEFINE 3
const int BUCKET_NUMBER = 3;
const int MAX_BUCKET[BUCKET_NUMBER_DEFINE] = {10,7,4/*,5*/};

int indexExist=1;// đánh số tồn tại mỗi lần thêm một status
struct status     // cấu trúc status sẽ gồm prev để biết bước pháia trước làm gì
{
	int prev;
	int bucket[BUCKET_NUMBER_DEFINE];
	struct status *next;

};
typedef struct status status;

status* newStatus(int bucket_0, int bucket_1, int bucket_2/*, int bucket_3*/)
{
	status *temp = (status*)malloc(sizeof(status));
	(*temp).prev = -1;
	(*temp).bucket[0]=bucket_0;
	(*temp).bucket[1]=bucket_1;
	(*temp).bucket[2]=bucket_2;
	//(*temp).bucket[3]=bucket_3;
	(*temp).next = NULL;
	return temp;
}

void freeStatus(status *unuseStatus)
{
	free(unuseStatus);
}

void insertNewStatus(status *startList, status *newStatus, int index)
{
	int i=1;
	status *temp = startList; 
	while((*temp).next != NULL && i<index)
	{
		temp = (*temp).next;
		++i;
	}
	if((*temp).next==NULL && i != index ) (*temp).next = newStatus;
	else if(i==index){
		(*temp).prev = (*newStatus).prev;
		for(int i=0;i<BUCKET_NUMBER;++i){
			(*temp).bucket[i] = (*newStatus).bucket[i];	
		}
		(*temp).next = NULL;
	}
}

void printStatus(status *temp)
{
	printf("|prev  : %2d",(*temp).prev);
	for(int i=0;i < BUCKET_NUMBER;++i)
	{
		printf("|buck_%2d: %2d",i+1,(*temp).bucket[i]);
	}
	printf("|\n");
}

int checkMatchCase(status *in/*, status *satisfiedCase*/)
{
	if(
		// (*in).bucket[0] == (*satisfiedCase).bucket[0] &&
		// (*in).bucket[1] == (*satisfiedCase).bucket[1] &&
		// (*in).bucket[2] == (*satisfiedCase).bucket[2]
		(*in).bucket[1] == 2 ||
		(*in).bucket[2] == 2
	){
		return 1;//1 is same
	}
	return 0;//0 is different
}
int checkExistStatus(status *startList,status *tempStatus)
{
	status *temp = startList;
	do
	{
		int check = 1;
		for(int i=0;i<BUCKET_NUMBER;++i)
		{
			if((*tempStatus).bucket[i] != (*temp).bucket[i]) check = 0; 
		}
		if(check){
			//printf("checkComplete\n");
			return 1;//1 is same
		}
		temp= (*temp).next; 
	}
	while(temp != NULL);
	//printf("checkComplete\n");
	return 0;//0 is different
}






int toWhichBucket(int fromBucket, int whichChange)
{
	
	if(whichChange < fromBucket) return whichChange;
	int toBucket=fromBucket;
	while(toBucket <= whichChange){
		toBucket++;
	}
	return toBucket;

}
void changeStatus(status *temp,int check)
{
	//10  7  4
	int fromBucket = check/(BUCKET_NUMBER-1);
	int whichChange = check%(BUCKET_NUMBER-1);
	int toBucket = toWhichBucket(fromBucket,whichChange);
	if((*temp).bucket[fromBucket] > MAX_BUCKET[toBucket] - (*temp).bucket[toBucket])
	{
		(*temp).bucket[fromBucket] -= MAX_BUCKET[toBucket] - (*temp).bucket[toBucket]; 
		(*temp).bucket[toBucket] = MAX_BUCKET[toBucket];
	} else
	{
		(*temp).bucket[toBucket] += (*temp).bucket[fromBucket];
		(*temp).bucket[fromBucket] = 0;
	}
}

void printListStatus(status *startList)
{
	status *temp = startList;
	int fromBucket;
	int whichChange;
	int toBucket;
	do
	{
		if((*temp).prev != -1)
		{
			fromBucket = ((*temp).prev)/(BUCKET_NUMBER-1);
			whichChange = ((*temp).prev)%(BUCKET_NUMBER-1);
			toBucket = toWhichBucket(fromBucket,whichChange);
			printf("%2d -> %2d",MAX_BUCKET[fromBucket],MAX_BUCKET[toBucket]);
		}
		printf("\n");
		for(int i=0;i < BUCKET_NUMBER;++i)
		{
			printf("|buck %2d: %2d(L)",MAX_BUCKET[i],(*temp).bucket[i]);
		}
		printf("|\n");
		temp= (*temp).next; 
	}
	while(temp != NULL);
}

void recursiveWaterBucket(status *startListTemp,status *startListExist,status *temp/*,status *satisfiedCase*/, int indexTemp)
{
	// if(checkMatchCase(temp/*,satisfiedCase*/)) {
	// 	insertNewStatus(startListTemp,temp,indexTemp);
	// 	printListStatus(startListTemp);
	// 	printf("\n\n");
	// 	//printf("match case\n");
	// 	//return;
	// } else 
	if(checkExistStatus(startListExist,temp) && indexTemp > 1) {
		
		printListStatus(startListTemp);
		//printf("duplicate\n");
		//return;
	} else {
		status *copied_temp[6];
		for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i){
			copied_temp[i] = newStatus((*temp).bucket[0],(*temp).bucket[1],(*temp).bucket[2]);
		}
		status *copied_temp_exist =  newStatus((*temp).bucket[0],(*temp).bucket[1],(*temp).bucket[2]);

		insertNewStatus(startListExist,copied_temp_exist,indexExist++);
		//printf("%d\n",indexExist);
		insertNewStatus(startListTemp,temp,indexTemp);
		
		for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i)
		{
			changeStatus(copied_temp[i],i); (*copied_temp[i]).prev=i; recursiveWaterBucket(startListTemp,startListExist,copied_temp[i],/*satisfiedCase,*/indexTemp + 1);
		}
		
		//remember to free memory;
		for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i){
			free(copied_temp[i]);
		}
	}
}

int main(void)
{
	status *startListExist = newStatus(0,7,4);
	status *startListTemp = newStatus(0,7,4);
	//changeStatus(startListTemp,3);
	//printStatus(startListTemp);
	//printf("%d\n",toWhichBucket(2,0));
	recursiveWaterBucket(startListTemp,startListExist,startListTemp,/*satisfiedCase,*/1);

	//  for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i)
	//  {
	//  	int fromBucket = i/(BUCKET_NUMBER-1);
	//  	int whichChange = i%(BUCKET_NUMBER-1);
	//  	int toBucket = toWhichBucket(fromBucket,whichChange);
	//  	printf("i:%2d|kind:%2d|%2d -> %2d\n",i,whichChange,fromBucket,toBucket);
	//  }
	// return 0;
}
