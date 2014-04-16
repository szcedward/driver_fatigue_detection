#ifndef _MATNET_H
#define _MATNET_H

#pragma once

#include "TCPNet.h"
#include <cv.h>

#define BUFLEN 1024 * 1024 * 10 // 10MB

class MatNet {
protected:
  unsigned short port;
  char* buf;
  TCPNet* tcpNet;
public:
  MatNet(void);
  virtual ~MatNet(void);

  void setPort(unsigned short port);
  unsigned short getPort(void);

  Status matRecv(cv::Mat& frame);
  Status matSend(const cv::Mat& frame);

  virtual Status setupNetwork() = 0;
  virtual Status closeNetwork() = 0;
};

#endif