//			cd Dropbox/vmware/cProgramming/discreteMath/searching

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#define N 20

int a[N][N][N] ={0};  
int explore(int i,int j,int k)
{       
	int check;
	a[i][j][k]=1;
	if(j==2||k==2){
		printf("\nexits");
		return 1;
    } else {   
    	int x,y;
		if(i>0&&j<7) { 
  		    if(i+j>7) { 
  		    	y=7; x=i+j-7;
			} else {
				x=0; y=i+j;
			}   
			//if(y==2||k==2) return 1; 
			if(a[x][y][k]==0) {
				printf("convert 1 to 2\n");
				check=explore(x,y,k);
			}			  	
		}	         	     	
		if(i>0&&k<4 && check==0) {
  		    if(i+k>4) { 
  		    	y=4; x=i+k-4;
			}
			else {
				x=0; y=i+k;
			}	
			//if(j==2||y==2) return 1; 
			if(a[x][j][y]==0) {
     	        printf("convert 1 to 3\n");
				check = explore(x,j,y);
			}  			
     	}
		if(i<10&&j>0 && check==0) {
  		    if(i+j>10) { 
  		    	y=i+j-10; x=10;
			} else {
				x=i+j; y=0;
			}	
			//if(y==2||k==2) return 1; 
			if(a[x][y][k]==0) {
		        printf("convert 2 to 1\n");
			    check=explore(x,y,k);
			}  		
     	}
     	if(k<4&&j>0 && check ==0 ) {
  		    if(k+j>4) { 
  		    	x=k+j-4; y=4;
			} else {
				y=k+j; x=0;}	
			 	//if(x==2||y==2) return 1; 
			  	if(a[i][x][y]==0) {
     	        	printf("convert 2 to 3\n");
				    check = explore(i,x,y); 
				} 		  
     	}
     	if(k>0&&i<10 && check == 0 ) {
     		if(i+k>10) {
     			x=10; y=i+k-10;
			} else {
				x=k+i; y=0;
			}	
			//if(j==2||y==2) return 1; 
			if(a[x][j][y]==0) {
     	       	printf("convert 3 to 1\n");
				check = explore(x,j,y);
			}  	 
		}
		if(k>0&&j<7 && check ==0 ){
			if(k+j>7) {
				x=7;
				y=k+j-7;
			} else {
				x=k+j;y=0;
			}
			//if(x==2||y==2) return 1; 			
	    	if(a[i][x][y]==0) {
     	     	printf("convert 3 to 2\n");
			    explore(i,x,y); 
			} 
		}
	}
}
int main()
{	
    int i=0,j=7,k=4;  
	printf("Day chuyen doi la : \n") ;
 	explore(i,j,k);	
 	if(explore(i,j,k))
 		printf("\nket qua tim kiem: co");
 	else 
 		printf("\nket qua tim kiem: khong");
  	getch();
	return 0;
}