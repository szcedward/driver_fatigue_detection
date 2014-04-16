#pragma once
#include <cv.h>
#include <highgui.h>
#include <string>
#include "Image.h"
#include "RingBuffer.h"
#define EYE_OPEN 0
#define EYE_CLOSED 1

class EyeAnalyzer {
private:
  int cliffDiff;
  int lastCliffDiff;
  RingBuffer last10EyePixel;
  RingBuffer last50EyePixel;
  cv::Mat originalFrame;
  int blinkRate;
  bool isEyeBlinked;
  bool isEyeClosed;
  int eyeCloseThresh;

  int currState;
  int lastState;

  int showBlink;

  void getEyeStatus();
public:
  EyeAnalyzer(void);
  ~EyeAnalyzer(void);

  void doAnalyze(void);
  void setCliffDiff(int diff);
  void setOriginalFrame(const cv::Mat& frame);
  cv::Mat getOriginalFrame(void);
  void clearStatus(void);
};