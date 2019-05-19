#include <stdio.h>
#include <string.h>
#include <stdlib.h>









typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc {
  char *name;
  void (*fun)(state* s); /* setting the state as a pointer allows the functions to change it.*/

};

state *progState = NULL;

void quit(state* s){
	exit(0);
}

void TDM(state* s){
	printf("%c\n",s->debug_mode);
	if(s->debug_mode == '1'){
		s->debug_mode = '0';
		printf("debug mode is now off\n");
 }
 else{
		s->debug_mode = '1';
		printf("debug mode is now on\n");
 }
 
}
void SFM(state* s){
	fgets(progState->file_name,100,stdin);
}
void SUS(state* s){
	int unitNum;
	char num[1];
	printf("Enter a number:\n");
	fgets(num,100,stdin);
	unitNum = atoi(num);
	
	if((unitNum!=1)&&(unitNum!=2)&&(unitNum!=4)){
		progState->unit_size = unitNum;
	}

	else{
		printf("ERROR, an unexpected number\n");
	}	
}
void printDebug(char c){

}

struct fun_desc menu[] = {{"Toggle Debug Mode",TDM},{"Set File Name",SFM},{"Set Unit Size",SUS},{"quit",quit},{NULL,NULL}};



int main(int argc, char **argv){
	 progState = malloc(sizeof(state));
	 progState->debug_mode = '0';
	 char name[1];
	 int chosenFun;
	 int menuSize = sizeof(menu)/(sizeof (struct fun_desc));
	 while(1){
     printf("Please choose a function:\n");
     printDebug(progState->debug_mode);
     for(int i=0;i<menuSize-1;i++){
    	printf("%d) ",i);
    	printf("%s\n",menu[i].name);
	   }
	    printf("type:");
    	fgets(name,100,stdin);
   		chosenFun = atoi(name);
   		if((chosenFun>=0)&& (chosenFun<=3))
   			((menu[chosenFun]).fun)(progState);
   		else{
   			perror("ERROR: ARguemnts are not Valid \n");
   		}	
	  }
}



