#ifndef _TCPNET_H
#define _TCPNET_H

#pragma once

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

enum Status {
  FAIL = -100,
  FAIL_SOCKET,
  FAIL_BIND,
  FAIL_LISTEN,
  FAIL_ACCEPT,
  FAIL_CLOSE,
  FAIL_RESOLVE_HOST,
  FAIL_CONNECT_HOST,
  FAIL_SEND_LENGTH,
  FAIL_SEND_DATA,
  FAIL_RECV_LENGTH,
  FAIL_RECV_DATA,
  SUCCESS = 1,
};

#define STRLEN 1024

class TCPNet {
protected:
  unsigned short port;

  WSADATA wsaData;
  SOCKET sock;

  struct sockaddr_in servAddr;

  int read(char* vptr, unsigned long len);
  int write(const char* vptr, unsigned long len);

public:
  TCPNet(void);

  void setPort(unsigned short port);
  unsigned short getPort(void);

  long doRecv(char* vptr);
  long doSend(const char* s, unsigned long len);

  virtual Status setupNetwork() = 0;
  virtual Status closeNetwork() = 0;

  virtual ~TCPNet(void);
};

#endif