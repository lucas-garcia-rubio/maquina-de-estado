# include "sm.h"
# include <stdio.h>
# include <stdlib.h>

static void fnSTX(sm_t *sm, char data){
    if(data == STX){
        sm->state = ST_ADDR;
    }
}

static void fnADDR(sm_t *sm, char addr){
    if(sm->my_addr_receive == 0){
        sm->my_addr_receive += addr;
        sm->chk_receive += addr;
    }
    else{
        sm->my_addr_receive |= (((short)addr)<<8);
        sm->chk_receive += addr;
        if(sm->my_addr == sm->my_addr_receive)
            sm->state = ST_QTD;
        else
            printf("Erro no endereço\n");
    }
}

static void fnQTD(sm_t *sm, char qtd){
    sm->qtd_data = qtd;
    sm->chk_receive += qtd;
    sm->state = ST_DATA;
}

static void fnDATA(sm_t *sm, char data){
    if(sm->qtd_data_receive < sm->qtd_data){
        sm->msg[sm->qtd_data_receive++] = data;
        sm->chk_receive += data;
        if(sm->qtd_data == sm->qtd_data_receive)
            sm->state = ST_CHK;
    }
}

static void fnCHK(sm_t *sm, unsigned char chk){
    sm->chk = chk;
    if(sm->chk == sm->chk_receive)
        printf("Sucesso! Todo pacote foi recebido\n");
    else{
        printf("Erro. Falha na soma de checksum\n");
        printf("Deveria receber: %x  Recebeu: %x\n", sm->chk, sm->chk_receive);
    }
    sm->state = ST_END;
}

static void fnEND(sm_t *sm, char end){
    if(end == ETX)
        sm->handle(sm->msg);
}

void InitSM(sm_t *sm, short addr, handle_t handle){
    sm->state = ST_STX;
    sm->action[ST_STX] = (action_t)fnSTX;
    sm->action[ST_ADDR] = (action_t)fnADDR;
    sm->action[ST_QTD] = (action_t)fnQTD;
    sm->action[ST_DATA] = (action_t)fnDATA;
    sm->action[ST_CHK] = (action_t)fnCHK;
    sm->action[ST_END] = (action_t)fnEND;
    sm->my_addr = addr;
    sm->my_addr_receive = 0;
    sm->chk = 0;
    sm->chk_receive = 0;
    sm->qtd_data = 0;
    sm->qtd_data_receive = 0;
    sm->handle = handle;
}

void ExecSM(sm_t *sm, char data){
    sm->action[sm->state](sm, data);
}
