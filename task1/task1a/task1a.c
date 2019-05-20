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
		perror("Cannot open file for reading");
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
	fseek(F,locationNum,SEEK_SET);
	fgets(beforeCopyingToMem,(length*(progState->unit_size)),F);
	strcpy((progState->mem_buf), beforeCopyingToMem);
	fclose(F);
	printf("Loaded %d units into the memory \n",length);

}


char* unit_to_format(int unit) {
	static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
	return formats[unit-1];
	}
void read_units_to_memory(FILE* input, char* buffer, int count) {
	fread(buffer, progState->unit_size , count, input);
}
void print_units(FILE* output, char* buffer, int count) {
	char* end = buffer + (progState->unit_size)*count;
	while (buffer < end) {
		//print ints
		int decimalValue =0;
		int var = *((int*)(buffer));
		char toChange[1000];
//		sscanf(buffer, "%d", &decimalValue);
		sprintf(toChange, unit_to_format(progState->unit_size), var);
		sscanf(toChange+2, "%0x\n",&decimalValue);
		printf("%d",decimalValue);
		printf("           ");

		fprintf(output, unit_to_format(progState->unit_size), var);
		printf("\n");
		buffer += (progState->unit_size);
	}
//	fflush(stdin);
}

/* Writes buffer to file without converting it to text with write */
void write_units(FILE* output, char* buffer, int count, size_t unitSize) {
	fwrite(buffer, unitSize, count, output);
}

void MD(state* s){
	char inputString[120];
	int numOfUnits;
	int addr;
	FILE *F = fopen(s->file_name,"r");
	fgets(inputString, 120,stdin);
	fflush(stdin);
	sscanf(inputString, "%0x %d",&addr, &numOfUnits);
	if((F==NULL)&&(addr!=0)){
		perror("Cannot open file for reading\n");
		return;
	}
	printf("Decimal   Hexadecimal\n");
	printf("======================\n");
	if(addr!=0) {
		fseek(F, addr, SEEK_SET);

	}
	else{
		print_units(stdout,progState->mem_buf,numOfUnits);
	}

	fclose(F);

}

void FM(state* s){
	FILE *F = fopen(s->file_name, "r+");
	if(F==NULL){
		perror("Cannot open file for reading");
		return;
	}
	printf("Please enter <location> <val>\n");
	char inputString[120];
	int location;
	char value[120];
	fgets(inputString, 120,stdin);
	fflush(stdin);
	sscanf(inputString, "%d %s",&location,value);
	fseek(F,location,SEEK_SET);
	fwrite(value,1,strlen(value),F);
	fclose(F);
}



void SIF(state* s){
	printf("Please enter <source-address> <target-location> <length>\n");
	char inputString[120];
	int sourceAddress;
	int targetLocation;
	int length;
	int fileSize;
	fgets(inputString, 120,stdin);
	fflush(stdin);
	sscanf(inputString, "%0x %0x %d",&sourceAddress, &targetLocation, &length);
	FILE* F;
	F = fopen(s->file_name, "r+");
	if(F==NULL){
		perror("Cannot open file for reading\n");
		return;
	}

	fseek(F, 0L, SEEK_END);
	fileSize = ftell(F);
	fseek(F, 0L, SEEK_SET);
	fseek(F,targetLocation,SEEK_SET);
	if(targetLocation > fileSize){
		perror("ERROR:::: SIF -> targetLocation is bigger than fileSize\n");
	}
	if(sourceAddress == 0){
		write_units(F, progState->mem_buf,length,(size_t) s->unit_size);
	}
	else{
		write_units(F, sourceAddress, length, (size_t)s->unit_size);
	}

	fclose(F);

}

struct fun_desc menu[] = {{"Toggle Debug Mode",TDM},{"Set File Name",SFN},{"Set Unit Size",SUS},{"Load Into Memory",LIM},{"Memory Display",MD}, {"Save Into File", SIF},{"File Modify",FM},{"quit",quit},{NULL,NULL}};
int menuSize = sizeof(menu)/(sizeof (struct fun_desc));



int main(int argc, char **argv){
	 progState = malloc(sizeof(state));
	 progState->debug_mode = '0';
	 progState->unit_size=1;
	 progState->file_name[0] = NULL;
	 char name[1];
	 int chosenFun;
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
   		if((chosenFun>=0)&& (chosenFun<=6))
   			((menu[chosenFun]).fun)(progState);
   		else{
   			printf("ERROR: ARguemnts are not Valid \n");
   		}	
	  }
}



