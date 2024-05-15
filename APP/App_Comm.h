#ifndef APP_COMM_H
#define APP_COMM_H
#include "Includes.h" 
#include "lwip/opt.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
void Communication_Param_Init(void);
int IPStrtoArr(const char* IPAddrStr,ip_addr_t* IPAddrArr);
void communication_send(void);
int communication_back_poll(void);
void string_handle(void);
#endif