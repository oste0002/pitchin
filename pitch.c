#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

#include <netdb.h>
#include <sys/socket.h>

#include "pitch.h"
#include "dlist/dlist.h"


int setup_con(int *fd_sock, int *port);
void *cli_mgr_thread(void *arg);




// main
int main(int argc, char* argv[]) {
  int err;
  static top_ctrl ctrl = {0};
  cli_mgr_arg cli_mgr_arg1, cli_mgr_arg2, cli_mgr_arg3;
  ctrl.l_port = PITCH_LISTEN_PORT;
  ctrl.r_port = PITCH_REPLY_PORT;

  // Syntax check
  if (argc != 1) {
    fprintf(stderr, "Syntax: %s\n", argv[0]);
    exit(EXIT_FAILURE); }

  // Setup listen connection
  if ((err = setup_con(&ctrl.l_sock, &ctrl.l_port)) != 0) {
    fprintf(stderr,"Could not open listen connection\n");
    if ((err < -2) && (close(ctrl.l_sock) != 0))
      perror("close listen socket"); }


  // Initialize 3 client manager threads
  cli_mgr_arg1.cli_mgr_ID = 1;
  cli_mgr_arg2.cli_mgr_ID = 2;
  cli_mgr_arg3.cli_mgr_ID = 3;


  if ((errno = pthread_create(&ctrl.cli_mgr_ID1, NULL, &cli_mgr_thread,
          &cli_mgr_arg1)) != 0)
    perror("Create client manager thread #1");
  if ((errno = pthread_create(&ctrl.cli_mgr_ID2, NULL, &cli_mgr_thread,
          &cli_mgr_arg2)) != 0)
    perror("Create client manager thread #2");
  if ((errno = pthread_create(&ctrl.cli_mgr_ID3, NULL, &cli_mgr_thread,
          &cli_mgr_arg3)) != 0)
    perror("Create client manager thread #3");


  if (close(ctrl.l_sock) != 0)
    perror("close listen socket");

  exit(EXIT_SUCCESS);
}



int setup_con(int *sock, int *port) {
  struct addrinfo hints;
  struct addrinfo *servinfo, *p;
  char port_char[5];
  int gai_err;
  int yes = 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  sprintf(port_char, "%d", *port);
  if ((gai_err = getaddrinfo(NULL, port_char, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_err));
    return -1; }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    // Create socket
    if ((*sock = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
      perror("socket:");
      continue; }

    // Reuse occupied sockets
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &yes,
          sizeof(int)) == -1) {
      close(*sock);
      perror("setsockopt:");
      return -2; }

    // Bind socket to port
    if (bind(*sock, p->ai_addr, p->ai_addrlen) == -1) {
      close(*sock);
      perror("bind:");
      continue; }

    break; }
  freeaddrinfo(servinfo);

  // Exit on bind fail
  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    return -3; }

  // Listen
  if (listen(*sock, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE); }

  return 0;
}






void *cli_mgr_thread(void *arg) {
  cli_mgr_arg *thr_arg = (cli_mgr_arg *) arg;
  static cli_mgr_ctrl ctrl = {0};



  return NULL;
}
