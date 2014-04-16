#ifndef _MATSERVER_H
#define _MATSERVER_H

#pragma once

#include "matnet.h"


class MatServer : public MatNet
{
private:
  
public:
  MatServer(void);
  ~MatServer(void);

  Status setupNetwork();
  Status doBlockedAccept();
  Status closeNetwork();
};

#endif