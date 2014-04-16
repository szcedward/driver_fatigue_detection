#include "MatServer.h"
#include "TCPServer.h"

MatServer::MatServer(void) {
  tcpNet = new TCPServer();
}


MatServer::~MatServer(void) {
}

Status MatServer::setupNetwork() {
  tcpNet->setPort(port);
  return tcpNet->setupNetwork();
}

Status MatServer::doBlockedAccept() {
  return ((TCPServer*)(tcpNet))->doBlockedAccept();
}

Status MatServer::closeNetwork() {
  return tcpNet->closeNetwork();
}