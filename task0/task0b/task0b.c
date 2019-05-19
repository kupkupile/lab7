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
	printf("Enter a number:\n");
	int num = fgetc(stdout);
	printf("%d",num);
	if((num!=1)&&(num!=2)&&(num!=4)){
		progState->unit_size = num;
	}

	else{
		perror("ERROR, an unexpected number\n");
	}	
}
void printDebug(char * string){
	if(progState->debug_mode=='1'){
	    if(strncmp(string, "builtin", 7) ==0){
            printf("Unit size: %d\tFileName: %s\tMemCount: %d\n",progState->unit_size, progState->file_name, (int)progState->mem_count);

	    }
	    else{
	        printf("%s\n",string);
	    }
	}
}

void quit(state* s){
    printDebug("quit");
    exit(0);
}

struct fun_desc menu[] = {{"Toggle Debug Mode",TDM},{"Set File Name",SFM},{"Set Unit Size",SUS},{"quit",quit},{NULL,NULL}};



int main(int argc, char **argv){
	 progState = malloc(sizeof(state));
	 progState->debug_mode = '0';
	 int menuSize = sizeof(menu)/(sizeof (struct fun_desc));
	 while(1){
     printf("Please choose a function:\n");
     printDebug("builtin");
     for(int i=0;i<menuSize-1;i++){
    	printf("%d) ",i);
    	printf("%s\n",menu[i].name);
	   }
	  }
}



