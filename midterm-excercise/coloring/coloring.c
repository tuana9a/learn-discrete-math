// cd Dropbox\vmware\cProgramming\discreteMath\mid-term-test\coloring


#include <stdio.h>
//#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char *colorful[] ={
"red",
"green",
"blue",
"pink",
"purple",
"orange",
"yellow",
"snow",
"bisque",
"honeydew",
"mistyrose",
"grey",
"gold",
"sienna",
"peru",
"burlywood",
"brown",
"salmon",
"violet",
"plum",
"maroon",
"DeepPink",
};


int apexNumber; //số đỉnh
int sideNumber; //số cạnh
int maxColor=0; //nếu đạt tới chỉ số này là fail
int checkStop = 0; // 0 là chưa dừng khác 0 là dừng



int readNumberInFile(FILE *inputFile, char firstNotation){
	int result = firstNotation -'0';
	char c;
	while(isdigit(c = fgetc(inputFile))){
		result = result*10 + c - '0';
	};
	return result;
}
void printSideStorage(int sideNumber,int sideStorage[][2]){
	for(int i = 0;i<sideNumber;++i){
		printf("%d -- %d\n",sideStorage[i][0],sideStorage[i][1]);
	}
}

void reveseString (char *string){
	int stringLength = strlen(string);
	for(int i=0;i<stringLength/2;++i){
		char temp = string[i];
		string[i]=string[stringLength-i-1];
		string[stringLength-i-1]=temp;
	}
	string[stringLength]='\0';
}

char* numToString(int num,char *outString){
	int i;
	for(i=0;num>0;++i,num/=10) {
		outString[i]=num%10 + '0';
		//printf("%d\n",outString[i]%10 + '0');
	}
	outString[i]='\0';
	reveseString(outString);
	return outString;
}
void outputToFile(int sideStorage[][2],int apexColor[], int wayToColor[]){
	FILE *output = fopen("output.txt","w+");
	char temp[]="";
	fprintf(output, "//thứ tự tô màu đỉnh: ");
	for(int i=0;i<apexNumber;++i) fprintf(output,"%d ",wayToColor[i]);
	//printf("ok1\n");
	fprintf(output,"\ngraph gemdino {\n");
	//printf("ok2\n");
	
	for(int i=0;i<apexNumber;++i){
		fprintf(output,"%d [fillcolor=%s, style=filled]\n",i,colorful[apexColor[i]]);
	}
	for(int i=0;i<sideNumber;++i){
		fprintf(output,"%d--%d\n",sideStorage[i][0],sideStorage[i][1]);
	}
	fprintf(output, "}");
	fclose(output);
}

void resetApexColor(int apexColor[], int from) {
	for(int i=from;i<apexNumber;++i) apexColor[i] = -1;
}
int colorIt(int sideStorage2[][apexNumber], int apexColor[], int whichApex, int colorUsed){
	int whichColor = 0 ;
	int i=0;
	while(i < apexNumber ) {
		if(sideStorage2[whichApex][i]==1 && apexColor[i]==whichColor){
			i=0;
			whichColor++;
		}
		++i;
	}
	//printf("color: %d\n",whichColor);
	apexColor[whichApex] = whichColor;
	if(whichColor==colorUsed) return 1;
	return 0;
}																							//  0         0              0	
void recursiveColoring(int sideStorage2[][apexNumber],int apexColor[], int wayToColor[],int tempApex, int tempPos, int colorUsed){
	//printf("tempPos: %d| apex: %d| ",tempPos,tempApex);
	
	if(checkStop!=0) {
		//printf("STOP:checkStop\n");
		return;
	}
	if(colorIt(sideStorage2,apexColor,tempApex,colorUsed)) colorUsed++;
	if(colorUsed > maxColor) {
		//resetApexColor(apexColor,tempPos);//?
		//printf("STOP:maxColor\n");
		return;
	}
	wayToColor[tempPos] = tempApex;
	
	if(tempPos == apexNumber-1){
		checkStop = 1;
	} else {
		for(int i=0;i<apexNumber;++i){
			int checkExist=0; //0 là chưa tồn tại, khác 0 là đã tồn tại
			for(int j=0;j<=tempPos;++j) if(wayToColor[j] == i) {checkExist=1;break;}
			if(checkExist==0) recursiveColoring(sideStorage2,apexColor,wayToColor,i,tempPos+1,colorUsed);
		}
	}
}



int main(int argc, char const *argv[])
{
	FILE *inputFile = fopen("input.txt","r");
	char c;


	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			apexNumber = readNumberInFile(inputFile,c);
			break;	
		}
	}
	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			sideNumber = readNumberInFile(inputFile,c);
			break;	
		}
	}


	int sideStorage[sideNumber][2];// mảng 2 chiều lưu cạnh
	int apexFrequency[apexNumber];// mảng 1 chiều lưu tần số của cạnh
	int step=0;
		

	for(int i = 0;i<apexNumber;++i) apexFrequency[i] = 0;
	while((c = fgetc(inputFile)) != EOF){
		if(isdigit(c)) {
			//printf("%c\apexNumber",c);
			//printf("%d\apexNumber",readNumberInFile(inputFile,c));
			int temp =   readNumberInFile(inputFile,c);
			sideStorage[step/2][step++%2] = temp;
			apexFrequency[temp]++;	
		}
	}	


	int apexColor[apexNumber]; // tô màu cho mỗi đỉnh mặc định là màu 0
	int wayToColor[apexNumber]; // cách tô màu
	int sideStorage2[apexNumber][apexNumber]; //mảng này thì các phần tử của nó sẽ là một nếu có cạnh nối giữa 2 thằng;


	for(int i=0;i<apexNumber;++i) for(int j=0;j<apexNumber;++j) sideStorage2[i][j]=0;
	int checkRegularGraph=0;
	for(int i=0;i<apexNumber;++i) {
		if(i!=0) if(apexFrequency[i] != apexFrequency[i-1]) checkRegularGraph=1;
		if(apexFrequency[i] > maxColor) maxColor = apexFrequency[i];
		apexColor[i]=-1;
		wayToColor[i]=-1;
	}
	if(checkRegularGraph!=1) maxColor++;
	for(int i=0;i<sideNumber;++i){
		sideStorage2[sideStorage[i][0]][sideStorage[i][1]] = 1;
		sideStorage2[sideStorage[i][1]][sideStorage[i][0]] = 1;
	}

	
	fclose(inputFile);
	/*
	printf("apexNumber: %d\nsideNumber: %d\n",apexNumber,sideNumber);
	printf("\napexLevel\n");
	for(int i = 0;i<apexNumber;++i) printf("apex %d: %d\n",i,apexFrequency[i]);
	printf("\nmaxColorCanUse: %d\n",maxColor-1);
	printf("\nsideStorage:\n");
	printSideStorage(sideNumber,sideStorage);
	printf("\nsideStorage2:\n");
	for(int i=0;i<apexNumber;++i){
		for(int j=0;j<apexNumber;++j){
			printf("%d ",sideStorage2[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/

	// printf("%d\n",colorIt(sideStorage2,apexColor,2,0));
	// printf("%d\n",colorIt(sideStorage2,apexColor,1,1));
	// printf("%d\n",colorIt(sideStorage2,apexColor,0,2));
	// for(int i=0;i<apexNumber;++i){
	// 	printf("%d ",apexColor[i]);
	// }
	

	for(int i=0;i<apexNumber;++i){
		recursiveColoring(sideStorage2,apexColor,wayToColor,i,0,0);
	}

	// wayToColor[0]=0;
	// wayToColor[1]=2;
	// wayToColor[2]=-1;

	// int tempPos = 2;
	// printf("TEST:\n");
	// for(int i=0;i<apexNumber;++i){
	// 	int checkExist = 0;// 0 là chưa tồn tại
	// 	for(int j=0;j<tempPos;++j) if(wayToColor[j] == i) {checkExist=1;break;}
	// 	if(checkExist==0)printf("%d\n",i);
	// }

	/*
	printf("\nColor of each apex\n");
	for(int i=0;i<apexNumber;++i) {
		printf("apex%d: color:%d\n",i,apexColor[i]);
	}
	*/
	printf("\nColoring order:\n");
	for(int i=0;i<apexNumber;++i) {
		printf(i==apexNumber-1?"%d":"%d->",wayToColor[i]);
	}

	printf("\n\n\n");
	outputToFile(sideStorage,apexColor,wayToColor);
	printf("\nYOUR COMPUTER HAS BEEN HACKED!!!\nTHANK FOR USING!!!\nDONE :))\n");
	return 0;
}
