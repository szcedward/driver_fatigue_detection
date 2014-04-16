#pragma once
#include <cv.h>
#include <highgui.h>
#include <string>
#include "Image.h"
#include "RingBuffer.h"
#include "cvUtil.h"
class YawningDetector {
private:
  cv::Mat originalFrame;
  cv::Mat mouthROI;
  cv::Point mouthUpperLeft;
  Color mouthSampleColor;
  int yawningThresh;
public:
  YawningDetector(void);
  YawningDetector(const cv::Mat& originalFrame, const cv::Mat& mouthROI);
  ~YawningDetector(void);

  void setOriginalFrame(const cv::Mat& frame);
  cv::Mat getOriginalFrame(void);

  void setMouthROI(const cv::Mat& frame);
  void setMouthROIUpperLeft(const cv::Point& mouthUpperLeft);
  void setMouthSampleColor(const Color& color);

  int findMouthDiff(const std::vector<cv::Point>& contour);

  void doDetect(const Color& color);
};

