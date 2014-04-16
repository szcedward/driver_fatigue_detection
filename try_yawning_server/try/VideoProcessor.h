#pragma once
#include <cv.h>
#include <highgui.h>
#include "string"
#include "LS.h"

#define VERBOSE_MODE

class VideoProcessor {
private:
  LS ls;

  std::string inputDir;
  std::vector<std::string> result;
  std::vector<std::string>::iterator it;

  cv::VideoCapture cap;
  cv::VideoWriter writer;

  void trimEndingSlash();

public:
  VideoProcessor(void);
  VideoProcessor(const std::string& inputDir);
  ~VideoProcessor(void);

  void setInputDir(const std::string& inputDir);

  bool setInputVideo(const std::string& filename);

  bool setOutputVideo(const std::string& filename, int codec,
                      double frameRate, cv::Size frameSize, bool isColor);

  static std::vector<std::string> split(const std::string& s, char delim);

  void cvtDirToVideo();

  bool appendToVideo(const cv::Mat& frame);

  bool getNextFrameFromVideo(cv::Mat& frame);

  bool getNextFrameFromDir(cv::Mat& frame);

  void resetDirIter();

  void display();

};
