#include "TCPServer.h"
#include "TCPNet.h"
#include <iostream>

TCPServer::TCPServer(void) {
  fromLen = sizeof(fromAddr);
}


TCPServer::~TCPServer(void) {
}

Status TCPServer::setupNetwork() {

  int res;

  // Initialize
  WSAStartup(MAKEWORD(2, 0), &wsaData);
  srvSock = socket(AF_INET, SOCK_STREAM, 0);

  if (srvSock == INVALID_SOCKET) {
    std::cerr << "server socket() Fails, error = " << WSAGetLastError() << std::endl;
    return FAIL_SOCKET;
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind
  res = bind(srvSock, (struct sockaddr*) &servAddr, sizeof(servAddr));

  if (res == SOCKET_ERROR) {
    closesocket(srvSock);
    std::cerr << "Fail to bind, error = " << WSAGetLastError() << std::endl;
    return FAIL_BIND;
  }

  // Listen
  res = listen(srvSock, SOMAXCONN);
  if (res == SOCKET_ERROR) {
    closesocket(srvSock);
    std::cerr << "Fail to listen, error = " << WSAGetLastError() << std::endl;
    return FAIL_LISTEN;
  }

  std::cerr << "The server is running ..." << std::endl;
  return SUCCESS;
}

Status TCPServer::doBlockedAccept() {
  sock = accept(srvSock, (struct sockaddr*) &fromAddr, &fromLen);

  if (sock == INVALID_SOCKET) {
    std::cerr << "Fail to accept, error = " << WSAGetLastError() << std::endl;
    return FAIL_ACCEPT;
  }

  std::cerr << "Accepted address: " << inet_ntoa(fromAddr.sin_addr) << ":" << ntohs(fromAddr.sin_port) << std::endl;
  closesocket(srvSock);
  return SUCCESS;
}

Status TCPServer::closeNetwork() {
  if (closesocket(sock) == SOCKET_ERROR) {
    std::cerr << "Fail to close, error = " << WSAGetLastError() << std::endl;
    return FAIL_CLOSE;
  }

  WSACleanup();
  std::cerr << "The server is stopped." << std::endl;
  return SUCCESS;
}