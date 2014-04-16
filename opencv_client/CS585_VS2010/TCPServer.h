#pragma once
#include "tcpnet.h"
class TCPServer : public TCPNet {
private:
  SOCKET srvSock;
  struct sockaddr_in fromAddr;
  int fromLen;
public:
  TCPServer(void);
  ~TCPServer(void);

  Status setupNetwork();
  Status doBlockedAccept();
  Status closeNetwork();
};

