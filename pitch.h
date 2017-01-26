#ifndef PITCH_H
#define PITCH_H

#include <pthread.h>
#include "dlist/dlist.h"

#define PITCH_LISTEN_PORT 15001
#define PITCH_REPLY_PORT 15002
#define BACKLOG 100

#define CLIENT_LIST_INIT_LINKS 10
#define CLIENT_LIST_MAX_LINKS 10000
#define NICK_LEN 31


typedef struct Dlist_List cli_list;


typedef struct {
  int lstn_sock;
  int lstn_port;
  int reply_port;
  pthread_t cli_mgr_ID1;
  pthread_t cli_mgr_ID2;
  pthread_t cli_mgr_ID3;
} top_ctrl;


typedef struct {
  int cli_mgr_ID;
  dlist_list *cli_mgr_list;
  dlist_list *cli_list;
} cli_mgr_ctrl;


typedef struct {
  int cli_mgr_ID;
  dlist_list *cli_mgr_list;
  } cli_mgr_arg;


typedef struct {
  int id;
  char nick[NICK_LEN];
  dlist_link *cli_link;
  const int cli_sock;
} cli_dat;


#endif
