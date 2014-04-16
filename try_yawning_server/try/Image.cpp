#include "Image.h"

Image::Image(void) {
}

Image::Image(int w, int h, int type): img(h, w, type) {
}

Image::Image(int w, int h, int type, const cv::Scalar& s): img(h, w, type, s) {
}

Image::Image(const cv::Mat& frame) {
  frame.copyTo(img);
}

void Image::operator()(const cv::Mat& frame) {
  frame.copyTo(img);
}


Image::~Image(void) {
}

int Image::getR(int x, int y) const {
  if (img.rows == 0) {
    throw "Image is NULL";
  }

  return img.data[y * img.step + img.channels() * x + 2];
}

int Image::getG(int x, int y) const {
  if (img.rows == 0) {
    throw "Image is NULL";
  }

  return img.data[y * img.step + img.channels() * x + 1];
}

int Image::getB(int x, int y) const {
  if (img.rows == 0) {
    throw "Image is NULL";
  }

  return img.data[y * img.step + img.channels() * x + 0];
}

Color Image::getColor(int x, int y) const {
  Color c;
  unsigned char* pixel = &img.data[img.step * y + img.channels() * x];
  c.r = img.data[y * img.step + img.channels() * x + 2];
  c.g = img.data[y * img.step + img.channels() * x + 1];
  c.b = img.data[y * img.step + img.channels() * x + 0];
  return c;
}

void Image::setColor(int x, int y, Color& c) {
  setColor(x, y, c.r, c.g, c.b);
}

void Image::setColor(int x, int y, int r, int g, int b) {
  if (img.rows == 0) {
    throw "Image is NULL";
  }

  img.data[y * img.step + img.channels() * x + 2] = r;
  img.data[y * img.step + img.channels() * x + 1] = g;
  img.data[y * img.step + img.channels() * x + 0] = b;
}

int Image::getGrayscale(int x, int y){
  return img.data[y * img.step + img.channels() * x];
}

void Image::setGrayscale(int x, int y, int v) {
  if (img.rows == 0) {
    throw "Image is NULL";
  }

  img.data[y * img.step + img.channels() * x] = v;
}

void Image::rescale(double scaleFactor) {
  if (scaleFactor == 0) {
    throw "invalid scaleFactor:";
  }

  cv::Mat resized;
  resize(img, resized, cv::Size(), scaleFactor, scaleFactor);
  img = resized.clone();
}
