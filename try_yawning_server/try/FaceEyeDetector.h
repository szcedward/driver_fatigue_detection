#pragma once
#include <cv.h>
#include <highgui.h>
#include <string>
#include "Image.h"
#include "RingBuffer.h"
#include "mode.h"

#define SUCCESS 1
#define FAIL -1

#ifdef VIDEO_MODE
#define EYE_ROI_RADIUS_W 22
#define EYE_ROI_RADIUS_H 18
#define FRAME_WIDTH 1920
#define FRAME_HEIGH 1080
#else
#define EYE_ROI_RADIUS_W 13
#define EYE_ROI_RADIUS_H 15
#define FRAME_WIDTH 640
#define FRAME_HEIGH 480
#endif

class FaceEyeDetector {
private:
  cv::Mat frame;
  cv::Mat faceROI;
  cv::Mat eyeBallROI;
  cv::Point eyeCenter;
  Color eyeSampleColor;
  Color mouthSampleColor;
  cv::Mat mouthROI;
  cv::Point mouthUpperLeft;
  std::vector<cv::Rect> faces;
  std::vector<cv::Rect> eyes;
  std::vector<cv::Rect> mouth;
  cv::CascadeClassifier face_cascade;
  cv::CascadeClassifier eyes_cascade;
  cv::CascadeClassifier mouth_cascade;

public:
  bool setFaceCascade(std::string face_cascade_name) {
    if (!face_cascade.load(face_cascade_name)) {
      printf("--(!)Error loading face\n");
      return (bool)FAIL;
    } else {
      return (bool)SUCCESS;
    }
  }

  bool setEyeCascade(std::string eyes_cascade_name) {
    if (!eyes_cascade.load(eyes_cascade_name)) {
      printf("--(!)Error loading eye\n");
      return (bool)FAIL;
    } else {
      return (bool)SUCCESS;
    }
  }

  bool setMouthCascade(std::string mouth_cascade_name) {
    if (!mouth_cascade.load(mouth_cascade_name)) {
      printf("--(!)Error loading mouth\n");
      return (bool)FAIL;
    } else {
      return (bool)SUCCESS;
    }
  }

  void setFrame(cv::Mat inputFrame) {
    frame = inputFrame;
  }

  cv::Mat getEyeBallROI(void) {
    return eyeBallROI;
  }

  Color getEyeSampleColor(void) {
    return eyeSampleColor;
  }

  Color getMouthSampleColor(void) {
	  return mouthSampleColor;
  }

  cv::Mat getOriginalFrame(void) {
    return frame;
  }

  cv::Point getEyeROIUpperLeft(void) {
    return cv::Point(eyeCenter.x - EYE_ROI_RADIUS_W, eyeCenter.y - EYE_ROI_RADIUS_H);
  }
  
  cv::Mat getMouthROI(void) {
    return mouthROI;
  }

  cv::Point getMouthROIUpperLeft(void)
  {
	  return mouthUpperLeft;
  }

  void doDetect(void);
};

