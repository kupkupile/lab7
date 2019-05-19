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
void printDebug(char * string){
    if(progState->debug_mode=='1'){
        if(strncmp(string, "builtin", 7) ==0){
            printf("Unit size: %d\tFileName: %s\tMemCount: %d\n",progState->unit_size, progState->file_name, (int)progState->mem_count);

        }
        else{
            printf("%s",string);
        }
    }
}

void quit(state* s){
    printDebug("quitting\n");
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
void SFN(state* s){
	char beforeTok[128];
	fgets(beforeTok,100,stdin);
	strtok(beforeTok,"\n");
	strcpy(s->file_name, beforeTok);
	fflush(stdin);
	char* messageToSend;
	printDebug("DEBUG: file name set to ");
	printDebug(progState->file_name);
	printDebug("\n");
}
void SUS(state* s){
	int unitNum;
	char num[124];
	printf("Enter a number:\n");
	fgets(num,100,stdin);
	fflush(stdin);
	unitNum = atoi(num);
	
	if((unitNum==1)||(unitNum!=2)||(unitNum!=4)){
		s->unit_size = unitNum;
        printDebug("Debug: set size to ");
        printDebug(num);
        printDebug("\n");
	}

	else{
		printf("ERROR, an unexpected number\n");
	}	
}
void LIM(state* s){
	if(s->file_name[0]==NULL){
		perror("ERROR, File name is null\n");
		return;
	}
	FILE *F = fopen(s->file_name,"r");
	if(F==NULL){
		perror("Cannot open file for reading\n");
		return;
	}
	printf("Please enter <location> <length>\n");
	char inputString[120];
	int locationNum;
	int length;
	fgets(inputString, 120,stdin);
	fflush(stdin);
	sscanf(inputString, "%0x %d",&locationNum, &length);
	char beforeCopyingToMem[(length*(progState->unit_size))+1];
	fgets(beforeCopyingToMem,(length*(progState->unit_size)),F);
	strcpy((progState->mem_buf)+locationNum, beforeCopyingToMem);
	fclose(F);

}
struct fun_desc menu[] = {{"Toggle Debug Mode",TDM},{"Set File Name",SFN},{"Set Unit Size",SUS},{"Load Into Memory",LIM},{"quit",quit},{NULL,NULL}};



int main(int argc, char **argv){
	 progState = malloc(sizeof(state));
	 progState->debug_mode = '0';
	 progState->unit_size=1;
	 progState->file_name[0] = NULL;
	 char name[1];
	 int chosenFun;
	 int menuSize = sizeof(menu)/(sizeof (struct fun_desc));
	 while(1){
     printf("Please choose a function:\n");
     printDebug("builtin");
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



