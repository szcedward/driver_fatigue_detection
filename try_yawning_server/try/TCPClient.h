#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#pragma once

#include "tcpnet.h"

class TCPClient : public TCPNet {
private:
  char host[STRLEN];
  unsigned long addr;
  struct hostent* hostEnt;
public:
  TCPClient(void);
  ~TCPClient(void);

  void setHost(char host[STRLEN]);
  Status setupNetwork();
  Status doBlockedConnect();
  Status closeNetwork();
};

#endif