#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {


}



int digit_cnt(char *input) {
    int counter = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] >= '0' && input[i] <= '9')
            counter++;
    }
    return counter;
}