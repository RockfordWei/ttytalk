//
//  ttycall.c
//
//  Created by Rocky Wei on 2018-09-02.
//  Copyright © 2018 Rocky Wei. All rights reserved.
//
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

typedef struct _session {
  int tty;
  unsigned char * buffer;
  int size;
  int received;
} tty_session_t;

void * tty_recv(void * args)
{
  if (!args) return NULL;
  tty_session_t * s = (tty_session_t *)args;
  fd_set rds, eds;
  int wait = 0;
  #define szSTR 256
  unsigned char buf[szSTR];
  size_t loaded = 0;
  struct timeval tv;
  memset(&tv, 0, sizeof(tv));
  tv.tv_sec = 1;
  size_t size = 0;
  memset(s->buffer, 0, s->size);
  s->received = 0;
  int ret = 0;
  while(wait >= 0) {
    FD_ZERO(&rds);
    FD_ZERO(&eds);
    FD_SET(s->tty, &rds);
    FD_SET(s->tty, &eds);
    wait = select(s->tty + 1, &rds, NULL, &eds, &tv);
    if(wait < 0) {
      ret = -6;
    } else if (wait == 0) {
      if (size) {
        ret = size;
        break;
      } else {
        continue;
      }
    } else {
      if (FD_ISSET(s->tty, &eds)) {
        ret = -7;
        break;
      } else if (FD_ISSET(s->tty, &rds)) {
        memset(buf, 0, szSTR);
        loaded = read(s->tty, buf, szSTR);
        if (loaded > 0) {
          if (size + loaded > s->size) {
            memset(s->buffer, 0, s->size);
            size = 0;
          }
          memcpy(s->buffer + size, buf, loaded);
          size += loaded;
        }
      }
    }
  }
  s->received = ret;
  return NULL;
}

/// tty session function - send a command line to tty and wait for data return.
/// once done, this function will trigger the callback
/// - parameter device: device to open
/// - parameter command: command to send
/// - parameter size: size of command
/// - parameter buffer: data buffer for receiving
/// - parameter szbuf: max size of buffer
/// - returns: size of data that actual received
int tty_session(const char * device,
                const unsigned char * command, const int szcmd,
                unsigned char * buffer, const int szbuf)
{
  if (!device || !command || !buffer || szcmd < 1 || szbuf < 1) {
    return -1;
  }

  tty_session_t * session = (tty_session_t *)malloc(sizeof(tty_session_t));
  memset(session, 0, sizeof(tty_session_t));
  session->buffer = buffer;
  session->size = szbuf;

  session->tty = open(device, O_NONBLOCK | O_RDONLY | O_NOCTTY | O_NDELAY);
  if (session->tty < 1) {
    free(session);
    return -2;
  }

  int tty = open(device, O_NONBLOCK | O_WRONLY | O_NOCTTY | O_NDELAY);
  if (tty < 1) {
    free(session);
    return -3;
  }

  pthread_t receiver;
  int test = pthread_create(&receiver, NULL, tty_recv, session);
  if (test) {
    close(tty);
    close(session->tty);
    free(session);
    return -4;
  }

  test = write(tty, command, szcmd) == szcmd;
  close(tty);
  if (test == 0) {
    pthread_cancel(receiver);
    test = -5;
  } else {
    pthread_join(receiver, NULL);
    test = session->received;
  }
  close(session->tty);
  free(session);
  return test;
}
