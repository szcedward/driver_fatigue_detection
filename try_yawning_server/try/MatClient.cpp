#include "MatClient.h"
#include "TCPClient.h"

MatClient::MatClient(void) {
  tcpNet = new TCPClient();
}


MatClient::~MatClient(void) {
}

void MatClient::setHost(char host[STRLEN]) {
  memcpy_s(this->host, STRLEN, host, STRLEN);
  ((TCPClient*)(tcpNet))->setHost(host);
}

Status MatClient::setupNetwork() {
  tcpNet->setPort(port);
  ((TCPClient*)(tcpNet))->setHost(host);
  return tcpNet->setupNetwork();
}

Status MatClient::doBlockedConnect() {
  return ((TCPClient*)(tcpNet))->doBlockedConnect();
}

Status MatClient::closeNetwork() {
  return tcpNet->closeNetwork();
}