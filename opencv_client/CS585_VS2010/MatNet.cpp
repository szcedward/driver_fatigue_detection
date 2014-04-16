#include "MatNet.h"
#include <iostream>

MatNet::MatNet(void) {
  buf = new char[BUFLEN];

  if (!buf) {
    std::cerr << "Buffer allocation failed." << std::endl;
  }
}


MatNet::~MatNet(void) {
  if (tcpNet) {
    delete[] tcpNet;
  }

  if (buf) {
    delete[] buf;
  }
}

void MatNet::setPort(unsigned short port) {
  this->port = port;
}

unsigned short MatNet::getPort(void) {
  return port;
}

Status MatNet::matRecv(cv::Mat& frame) {
  int recvLen;
  char* ptr = buf;
  int offset = 0;
  recvLen = tcpNet->doRecv(ptr);

  if (recvLen < 0) {
    return FAIL;
  }

  std::cout << "Received " << recvLen << " bytes." << std::endl;
  int rows;
  int cols;
  unsigned int dataLen;
  void* data;
  int type;
  unsigned int step;
  memcpy_s(&rows, sizeof(int), ptr + offset, sizeof(int));
  offset += sizeof(int);
  rows = ntohl(rows);
  memcpy_s(&cols, sizeof(int), ptr + offset, sizeof(int));
  offset += sizeof(int);
  cols = ntohl(cols);
  memcpy_s(&dataLen, sizeof(unsigned int), ptr + offset, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  dataLen = ntohl(dataLen);
  data = new char[dataLen];
  memcpy_s(data, dataLen, ptr + offset, dataLen);
  offset += dataLen;
  memcpy_s(&type, sizeof(int), ptr + offset, sizeof(int));
  offset += sizeof(int);
  type = ntohl(type);
  memcpy_s(&step, sizeof(unsigned int), ptr + offset, sizeof(unsigned int));
  step = ntohl(step);
  std::cout << "rows=" << rows << " cols=" << cols << " type=" << type << " step=" << step << std::endl;
  cv::Mat frameRecv(rows, cols, type, data, step);
  frameRecv.copyTo(frame);
  delete[] data;
  return SUCCESS;
}

Status MatNet::matSend(const cv::Mat& frame) {
  int sendLen;
  char* ptr = buf;
  int offset = 0;
  int rows = frame.rows;
  int cols = frame.cols;
  unsigned int dataLenActual = frame.dataend - frame.datastart;
  void* data = frame.data;
  int type = frame.type();
  unsigned int step = frame.step;
  std::cout << "rows=" << rows << " cols=" << cols << " type=" << type << " step=" << step << std::endl;
  unsigned long len = 3 * sizeof(int) + 2 * sizeof(unsigned int) + dataLenActual;
  unsigned int dataLen;
  rows = htonl(rows);
  cols = htonl(cols);
  dataLen = htonl(dataLenActual);
  type = htonl(type);
  step = htonl(step);
  memcpy_s(ptr + offset, sizeof(int), &rows, sizeof(int));
  offset += sizeof(int);
  memcpy_s(ptr + offset, sizeof(int), &cols, sizeof(int));
  offset += sizeof(int);
  memcpy_s(ptr + offset, sizeof(unsigned int), &dataLen, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  memcpy_s(ptr + offset, dataLenActual, data, dataLenActual);
  offset += dataLenActual;
  memcpy_s(ptr + offset, sizeof(int), &type, sizeof(int));
  offset += sizeof(int);
  memcpy_s(ptr + offset, sizeof(unsigned int), &step, sizeof(unsigned int));
  std::cout << "Sending ..." << std::endl;
  sendLen = tcpNet->doSend(buf, len);

  if (sendLen < 0) {
    return FAIL;
  }

  std::cout << "Sent " << sendLen << " bytes." << std::endl;
  return SUCCESS;
}