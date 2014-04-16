#include "EyeballColorDetector.h"

void EyeballColorDetector::initialize(const cv::Mat& originalFrame,
                                      const cv::Point& eyeROIUpperLeft,
                                      const cv::Mat& frame,
                                      const Color& eyeSampleColor) {
  originalFrame.copyTo(this->originalFrame);
  this->eyeROIUpperLeft = eyeROIUpperLeft;
  processed(frame);
  this->eyeSampleColor = eyeSampleColor;
}

void EyeballColorDetector::doDetect() {
  clearStatus();
  getProjection();
  getBoundingBox();
  markBoundingBox();
}

void EyeballColorDetector::getProjection() {
  int i = 0;
  int j = 0;
  int rValue = 0;
  int gValue = 0;
  int bValue = 0;

  for (j = 0; j < processed.getHeight(); j++) {
    for (i = 0; i < processed.getWidth(); i++) {
      rValue = processed.getR(i, j);
      gValue = processed.getG(i, j);
      bValue = processed.getB(i, j);

      if (abs(rValue - eyeSampleColor.r) <= epsilon &&
          abs(gValue - eyeSampleColor.g) <= epsilon &&
          abs(bValue - eyeSampleColor.b) <= epsilon) {
        //xProject[i]++;
        yProject[j]++;
      }
    }
  }

  /*for(j = 0; j < processed.getHeight(); j++)
  {
  	std::cout<<yProject[j]<<" ";
  }
  std::cout<<std::endl;*/
}

void EyeballColorDetector::getBoundingBox() {
  /*xMin = findCliffUp(xProject, processed.getWidth());
  xMax = findCliffDown(xProject, processed.getWidth());
  yMin = findCliffUp(yProject, processed.getHeight());
  yMax = findCliffDown(yProject, processed.getHeight());

  boundingBox.x = xMin;
  boundingBox.y = yMin;
  boundingBox.width = xMax - xMin;
  boundingBox.height = yMax - yMin;*/
  findCliffs(yProject, processed.getHeight());
  //std::cout<<"x, y = "<<boundingBox.x<<"\t"<<boundingBox.y<<std::endl;
  //std::cout<<"wiidth, height = "<<boundingBox.width<<"\t"<<boundingBox.height<<std::endl;
  //std::cout<<yMin<<" "<<yMax<<std::endl;
}

//int EyeballColorDetector::findCliffUp(const int* project, int size) {
//  int i = 0;
//
//  for (i = 0; i < size; i++) {
//    if (project[i] >= 4) {
//      return i;
//    }
//  }
//
//  return 0;
//}
//
//int EyeballColorDetector::findCliffDown(const int* project, int size) {
//  int i = 0;
//
//  for (i = size - 1; i >= 0; i--) {
//    if (project[i] >= 4) {
//      return i;
//    }
//  }
//
//  return 1;
//}

void EyeballColorDetector::findCliffs(const int* project, int size) {
  int i, j = 0;

  for (i = size - 1; i >= 0; i--) {
    if (project[i] >= 4) {
      yMax = i;

      for (j = i; j >= 0; j--) {
        if (project[j] == 0) {
          yMin = j + 1;
          return;
        }
      }
    }
  }

  yMin = 30;
  yMax = 31;
}

void EyeballColorDetector::markBoundingBox() {
  if (processed.getImage().data) {
    rectangle(originalFrame, cv::Rect(eyeROIUpperLeft.x, yMin + eyeROIUpperLeft.y, WIDTH, (yMax - yMin)), cv::Scalar(0, 255, 0), 2);
    rectangle(processed.getImage(), cv::Rect(0, yMin, WIDTH, yMax - yMin), cv::Scalar(0, 255, 0), 2);
    imshow("EyeROI", processed.getImage());
  }
}

void EyeballColorDetector::clearStatus() {
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

//useless
//void EyeballColorDetector::showProcessed() {
//  imshow("processed", processed.getImage());
//}