#pragma once
#include <cv.h>

#include "Rect.h"
#include "Color.h"

/**************
 * Helper class which represents an image. The class contains
 * an pointer to a type of IplImage from the OpenCV imaging library.
 * This class allows simple access as well as some helper processing
 * operations.
 *
 * !WARNING!
 * When creating an instance of the image class, you'll need to pass it
 * a pointer to an instance of the IplImage* class. Note that you should
 * NEVER pass in the exact IplImage* from a captured video. Instead copy
 * the video using cvCopy.
 *
 * Sam Epstein
 **************/
class Image {
private:

  //Pointer to obect containing all the information
  cv::Mat img;

public:

  //Constructs an empty instance
  Image(void);

  Image(int w, int h);

  ~Image(void);

  void operator()(const cv::Mat& frame);

  //Raw data accessors
  cv::Mat& getImage() {
    return img;
  }

  //Raw data modifier
  int getWidth() {
    return img.cols;
  }

  int getHeight() {
    return img.rows;
  }

  Color getColor(int x, int y);
  int getR(int x, int y);
  int getG(int x, int y);
  int getB(int x, int y);

  void setColor(int x, int y, Color& c);
  void setColor(int x, int y, int r, int g, int b);

  int getGrayscale(int x, int y);
  void setGrayscale(int x, int y, int v);

  //Scales down the image by a factor represented by
  //scaleFactor
  void rescale(double scaleFactor);
};
