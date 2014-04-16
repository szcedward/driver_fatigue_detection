#include "TCPNet.h"
#include <iostream>

TCPNet::TCPNet(void) {
}


TCPNet::~TCPNet(void) {
}

void TCPNet::setPort(unsigned short port) {
  this->port = port;
}

unsigned short TCPNet::getPort(void) {
  return port;
}

int TCPNet::read(char* vptr, unsigned long len) {
  unsigned long nleft;
  long nread;
  char* ptr;
  ptr = vptr;
  nleft = len;

  while (nleft > 0) {
    nread = recv(sock, ptr, nleft, 0);

    if (nread == SOCKET_ERROR) {
      std::cerr << "Fail to recv, error = " << WSAGetLastError() << std::endl;
      return FAIL;
    } else if (nread == 0) {
      break;
    }

    nleft -= nread;
    ptr += nread;
  }

  return len - nleft;
}

int TCPNet::write(const char* vptr, unsigned long len) {
  unsigned long nleft;
  unsigned long quota;
  long nwritten;
  const char* ptr;
  ptr = vptr;
  nleft = len;

  while (nleft > 0) {
    if (nleft > 1000) {
      quota = 1000;
    } else {
      quota = nleft;
    }

    nwritten = send(sock, ptr, quota, 0);

    if (nwritten == SOCKET_ERROR) {
      std::cerr << "Fail to send, error = " << WSAGetLastError() << std::endl;
      return FAIL;
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return len;
}

long TCPNet::doSend(const char* vptr, unsigned long len) {
  unsigned long lenn = htonl(len);

  if (write((char*)&lenn, sizeof(unsigned long)) != sizeof(unsigned long)) {
    return FAIL_SEND_LENGTH;
  }

  if (write(vptr, len) != len) {
    return FAIL_SEND_DATA;
  }

  return len;
}

long TCPNet::doRecv(char* vptr) {
  unsigned long len;

  if (read((char*)&len, sizeof(unsigned long)) != sizeof(unsigned long)) {
    return FAIL_RECV_LENGTH;
  }

  len = ntohl(len);

  if (read(vptr, len) != len) {
    return FAIL_RECV_DATA;
  }

  return len;
}