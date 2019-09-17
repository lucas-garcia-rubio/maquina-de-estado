# include <stdio.h>
# include <stdlib.h>
# include "sm.h"

sm_t sm1;

void handle(unsigned char *string);
void programa(sm_t *sm, unsigned char *data, unsigned char size);

int main(){
    unsigned char data[] = {STX, 0x0A, 0x00, 3, 'L', 'G', 'R', 0xF2, ETX};

    InitSM(&sm1, 10, handle);
    programa(&sm1, data, 9);
}

void programa(sm_t *sm, unsigned char *data, unsigned char size){
    for(int i = 0; i < size; i++)
        ExecSM(sm, data[i]);
}

void handle(unsigned char* string){
    printf("Pacote recebido: %s\n", string);
}
