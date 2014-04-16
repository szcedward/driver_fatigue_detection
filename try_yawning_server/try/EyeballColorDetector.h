#pragma once
#include <cv.h>
#include <highgui.h>
#include "Rect.h"
#include "Image.h"
#include "FaceEyeDetector.h"

#define WIDTH    EYE_ROI_RADIUS_W * 2
#define HEIGHT   EYE_ROI_RADIUS_H * 2

class EyeballColorDetector {
private:
  int xMin;
  int xMax;
  int yMin;
  int yMax;
  int xProject[WIDTH];
  int yProject[HEIGHT];
  Image processed;
  cv::Mat originalFrame;
  cv::Point eyeROIUpperLeft;
  Color eyeSampleColor;
  int epsilon;
  cv::Rect boundingBox;
public:
  EyeballColorDetector(): epsilon(10) {
    int i, j = 0;

    //for (i = 0; i < WIDTH; i++) {
    //  xProject[i] = 0;
    //}

    for (i = 0; i < HEIGHT; i++) {
      yProject[i] = 0;
    }

    xMin = 0;
    xMax = 0;
    yMin = 0;
    yMax = 0;
  }

  void initialize(const cv::Mat& orignialFrame,
                  const cv::Point& eyeROIUpperLeft,
                  const cv::Mat& frame,
                  const Color& eyeSampleColor);
  void clearStatus();
  void doDetect();
  void getProjection();
  void getBoundingBox();
  int findCliffUp(const int* project, int size);
  int findCliffDown(const int* project, int size);
  void findCliffs(const int* project, int size);
  void markBoundingBox();
  void showProcessed();

  cv::Mat getOriginalFrame(void) {
    return originalFrame;
  }

  int getCliffDiff(void) {
    return yMax - yMin;
  }

};