#include "VideoProcessor.h"
#include <iostream>


VideoProcessor::VideoProcessor(void) {
}

VideoProcessor::VideoProcessor(const std::string& inputDir) {
  this->inputDir = inputDir;
  trimEndingSlash();
}


VideoProcessor::~VideoProcessor(void) {
}

void VideoProcessor::setInputDir(const std::string& inputDir) {
  this->inputDir = inputDir;
  trimEndingSlash();
  ls.setDir(inputDir);
  result = ls.getResult();
  it = result.begin();
}

bool VideoProcessor::setInputVideo(const std::string& filename) {
  return cap.open(filename);
}

bool VideoProcessor::setOutputVideo(const std::string& filename,
                                    int codec, double frameRate,
                                    cv::Size frameSize, bool isColor) {
  return writer.open(filename, codec, frameRate, frameSize, isColor);
}

std::vector<std::string> VideoProcessor::split(const std::string& s, char delim) {
  std::vector < std::string > elems;
  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }

  return elems;
}

void VideoProcessor::trimEndingSlash() {
  int len = inputDir.length();

  if (inputDir.at(len - 1) == '\\') {
    inputDir.erase(len - 1, 1);
  }
}

void VideoProcessor::cvtDirToVideo() {
  cv::Mat frame;
  std::vector<std::string>::iterator it1;

  for (it1 = result.begin(); it1 < result.end(); it1++) {
#ifdef VERBOSE_MODE
    std::cout << "Converting frame " << *it1 << std::endl;
#endif
    getNextFrameFromDir(frame);
    appendToVideo(frame);
  }
}

bool VideoProcessor::getNextFrameFromDir(cv::Mat& frame) {
  if (it == result.end()) {
    return false;
  }

  std::string filename = inputDir + "\\" + *it;
#ifdef VERBOSE_MODE
  std::cout << "Reading " << filename << std::endl;
#endif
  frame = cv::imread(filename);
  it++;
  return true;
}

bool VideoProcessor::appendToVideo(const cv::Mat& frame) {
  if (!writer.isOpened()) {
    return false;
  }
  if (frame.data) {
    writer << frame;
#ifdef VERBOSE_MODE
    std::cout << "Wrote a frame into video" << std::endl;
#endif
    return true;
  } else {
    return false;
  }
}

bool VideoProcessor::getNextFrameFromVideo(cv::Mat& frame) {
  if (!cap.isOpened()) {
    return false;
  }

  cap >> frame;

  if (frame.data) {
#ifdef VERBOSE_MODE
    std::cout << "Retrieved a frame from video" << std::endl;
#endif
    return true;
  } else {
    return false;
  }
}

void VideoProcessor::display() {
  cv::namedWindow("video", 1);
  cv::Mat frame;

  while (getNextFrameFromVideo(frame)) {
    cv::imshow("video", frame);
    char c = cv::waitKey(30);
  }
}

void VideoProcessor::resetDirIter() {
  it = result.begin();
}