# ifndef _sm_h
# define _sm_h
# define STX '$'
# define ETX '#'
# define buffer_size 128

typedef enum{
    ST_STX, ST_ADDR, ST_QTD, ST_DATA, ST_CHK, ST_END
}state_t;

typedef void(*action_t)(void *, char data);
typedef void(*handle_t)(unsigned char *);

typedef struct _sm_t{

    state_t state;
    action_t action[ST_END+1];
    handle_t handle;

    short my_addr;
    short my_addr_receive;
    unsigned char msg[buffer_size];
    unsigned char qtd_data;
    unsigned char qtd_data_receive;
    unsigned char chk;
    unsigned char chk_receive;
    unsigned char etx;

}sm_t;

void InitSM(sm_t *sm, short addr, handle_t handle);
void ExecSM(sm_t *sm, char data);

# endif // _sm_h
