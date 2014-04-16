#pragma once
#include <iostream>
#include <cv.h>
class cvUtil {
public:
  static inline void printText(cv::Mat& frame, const std::string& text, const cv::Scalar& color, const cv::Point& position) {
    double fontScale = 0.75;
    int thickness = 1;
    cv::putText(frame, text, position, cv::FONT_HERSHEY_TRIPLEX, fontScale, color, thickness, CV_AA, false);
  }

  static inline void printDot(cv::Mat& frame, const cv::Point& point, const cv::Scalar& color, int length) {
//    std::cout << "color channel: " << color.channels << std::endl;
    int radius = (int)floor((double)(length / 2));
    if (frame.channels() == 3) {
      for (int m = -1 * radius; m <= radius; m++) {
        for (int n = -1 * radius; n <= radius; n++) {
          frame.data[(point.y + n) * frame.step + frame.channels() * (point.x + m) + 0] = (uchar)color.val[0];
          frame.data[(point.y + n) * frame.step + frame.channels() * (point.x + m) + 1] = (uchar)color.val[1];
          frame.data[(point.y + n) * frame.step + frame.channels() * (point.x + m) + 2] = (uchar)color.val[2];
        }
      }
    } else {
      for (int m = -1 * radius; m <= radius; m++) {
        for (int n = -1 * radius; n <= radius; n++) {
          frame.data[(point.y + n) * frame.step + frame.channels() * (point.x + m)] = (uchar)color.val[0];
        }
      }
    }
  }

  static inline int getB(const cv::Mat& frame, const cv::Point& point)
  {
	  return frame.data[point.y * frame.step + frame.channels() * point.x + 0];
  }

  static inline int getG(const cv::Mat& frame, const cv::Point& point)
  {
	  return frame.data[point.y * frame.step + frame.channels() * point.x + 1];
  }

  static inline int getR(const cv::Mat& frame, const cv::Point& point)
  {
	  return frame.data[point.y * frame.step + frame.channels() * point.x + 2];
  }

  static inline void setMatColor(cv::Mat& frame, const cv::Point& point, const cv::Scalar& color)
  {
	  frame.data[point.y * frame.step + frame.channels() * point.x + 0] = (uchar)color.val[0];
	  frame.data[point.y * frame.step + frame.channels() * point.x + 1] = (uchar)color.val[1];
	  frame.data[point.y * frame.step + frame.channels() * point.x + 2] = (uchar)color.val[2];
  }

};