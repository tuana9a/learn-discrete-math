//			cd Dropbox/vmware/cProgramming/discreteMath/searching


#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>



#define BUCKET_NUMBER_DEFINE 3
const int BUCKET_NUMBER = 3;
const int MAX_BUCKET[BUCKET_NUMBER_DEFINE] = {10,7,4};

int count = 0;
int indexExist=1;// đánh số tồn tại mỗi lần thêm một status
struct status     // cấu trúc status sẽ gồm prev để biết bước pháia trước làm gì
{
	int prev;
	int bucket[BUCKET_NUMBER_DEFINE];
	struct status *next;
};
typedef struct status status;



status* newStatus(int bucket_0,...)
{
	status *temp = (status*)malloc(sizeof(status));
	
	va_list ap;//argument pointer
	va_start(ap,bucket_0);
	(*temp).bucket[0]= bucket_0;
	for(int i=1;i<BUCKET_NUMBER;++i){
		(*temp).bucket[i]= va_arg(ap,int);
	}
	va_end(ap);
	(*temp).prev = -1;
	(*temp).next = NULL;
	return temp;
}

status* newCopiedStatus(status* origin)
{
	status *copied = (status*)malloc(sizeof(status));
	(*copied).prev = -1;
	for(int i=0;i<BUCKET_NUMBER;++i)
	{
		(*copied).bucket[i] = (*origin).bucket[i];
	}
	(*copied).next=NULL;
	return copied;
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
		for(int j=0;j<BUCKET_NUMBER;++j){
			(*temp).bucket[j] = (*newStatus).bucket[j];	
		}
		(*temp).next = NULL;
	}
	//printf("inser OK");
}

void printStatus(status *temp)
{
	printf("|prev  : %2d",(*temp).prev);
	for(int i=0;i < BUCKET_NUMBER;++i)
	{
		printf("|buck %2d: %2d",i+1,(*temp).bucket[i]);
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
	if(checkMatchCase(temp/*,satisfiedCase*/)) {
		insertNewStatus(startListTemp,temp,indexTemp);
		//printListStatus(startListTemp);
		//printf("\n\n");
		count++;
		//printf("match case\n");
		//return;
	} else 
	if(checkExistStatus(startListExist,temp) && indexTemp > 1) {// cái này chuyển startListExist thành startListTemp là tìm tất cả
		//count++;
		//printListStatus(startListTemp);
		//printf("\n\n");
		//count++;
		//printf("duplicate\n");
		//return;
	} else {
		status *copied_temp[BUCKET_NUMBER*(BUCKET_NUMBER-1)];
		

		for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i){
			//copied_temp[i] = newStatus((*temp).bucket[0],(*temp).bucket[1],(*temp).bucket[2]);
			copied_temp[i] = newCopiedStatus(temp);
			//printStatus(copied_temp[i]);
		}
		//status *copied_temp_exist =  newStatus((*temp).bucket[0],(*temp).bucket[1],(*temp).bucket[2]);
		status *copied_temp_exist =  newCopiedStatus(temp);				// cái này bật là cho DFS, tắt là cho tìm tất cả


		insertNewStatus(startListExist,copied_temp_exist,indexExist++);	// cái này bật là cho DFS, tắt là cho tìm tất cả
		//printf("%d\n",indexExist);
		insertNewStatus(startListTemp,temp,indexTemp);
		
		for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i)
		{
			(*copied_temp[i]).prev=i;
			changeStatus(copied_temp[i],i);
			recursiveWaterBucket(startListTemp,startListExist,copied_temp[i],/*satisfiedCase,*/indexTemp + 1);
			insertNewStatus(startListTemp,temp,indexTemp);
		}
		
		//remember to free memory;
		// for(int i=0;i<BUCKET_NUMBER*(BUCKET_NUMBER-1);++i){
		// 	free(copied_temp[i]);
		// }
	}
}



int main(int argc, char const *argv[])
{
	status *startListExist = newStatus(0,7,4);
	status *startListTemp = newCopiedStatus(startListExist);
	recursiveWaterBucket(startListTemp,startListExist,startListTemp,/*satisfiedCase,*/1);
	printf("%d",count);



	// status *test[5];
	// for(int i=0;i<5;++i){
	// 	test[i] = newStatus(1,2,3);
	// 	insertNewStatus(startListExist,test[i],i+2);
	// }
	// printListStatus(startListExist);
	// printf("ok1\n");
	// insertNewStatus(startListExist,startListTemp,3);
	// printf("ok2\n");
	// printListStatus(startListExist);
	// printf("END.\n");


	return 0;
}
