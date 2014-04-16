#include "TCPClient.h"
#include <iostream>

TCPClient::TCPClient(void) {
}


TCPClient::~TCPClient(void) {
}

void TCPClient::setHost(char host[STRLEN]) {
  memcpy_s(this->host, STRLEN, host, STRLEN);
}

Status TCPClient::setupNetwork() {
  // Initialize
  WSAStartup(MAKEWORD(2, 0), &wsaData);
  addr = inet_addr(host);

  if (addr == INADDR_NONE) {
    hostEnt = gethostbyname(host);

    if (!hostEnt) {
      std::cerr << "Invalid host" << std::endl;
      return FAIL_RESOLVE_HOST;
    }

    memcpy(&addr, hostEnt->h_addr_list[0], hostEnt->h_length);
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);
  servAddr.sin_addr.s_addr = addr;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == INVALID_SOCKET) {
    std::cerr << " Create socket Fails, error = " << WSAGetLastError() << std::endl;
    return FAIL_SOCKET;
  }

  return SUCCESS;
}

Status TCPClient::doBlockedConnect() {
  int res = connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr));

  if (res == SOCKET_ERROR) {
    closesocket(sock);
    std::cerr << "Fail to connect, error = " << WSAGetLastError() << std::endl;
    return FAIL_CONNECT_HOST;
  }

  return SUCCESS;
}

Status TCPClient::closeNetwork() {
  if (closesocket(sock) == SOCKET_ERROR) {
    std::cerr << "Fail to close, error = " << WSAGetLastError() << std::endl;
    return FAIL_CLOSE;
  }

  WSACleanup();
  std::cerr << "The client is stopped." << std::endl;
  return SUCCESS;
}