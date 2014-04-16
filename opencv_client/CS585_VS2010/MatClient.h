#ifndef _MATCLIENT_H
#define _MATCLIENT_H

#pragma once

#include "matnet.h"

class MatClient : public MatNet
{
private:
  char host[STRLEN];
public:
  MatClient(void);
  ~MatClient(void);

  void setHost(char host[STRLEN]);
  Status setupNetwork();
  Status doBlockedConnect();
  Status closeNetwork();

};

#endif