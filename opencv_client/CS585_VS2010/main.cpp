#include "MatClient.h"
#include <iostream>
#include <cv.h>
#include <highgui.h>

int main(int argc, char** argv) {
  int sentLen;
  cv::VideoCapture cap = cv::VideoCapture(0);
  cv::Mat frame;

  
  MatClient matClient;
  matClient.setHost("169.254.143.110");
  matClient.setPort(50000);
  matClient.setupNetwork();
  matClient.doBlockedConnect();
  while(1)
  {
	  cap >> frame;
	  sentLen = matClient.matSend(frame);
	  if (sentLen >= 0) {
		  std::cout << "Mat sent " << std::endl;
	  }
	  else
		  break;
  }
  matClient.closeNetwork();

  return 0;
}

