#include "MatServer.h"

#include <cv.h>
#include <highgui.h>

#include <iostream>
#include <stdio.h>

#include "Image.h"
#include "FaceEyeDetector.h"
#include "EyeAnalyzer.h"
#include "EyeballColorDetector.h"
#include "VideoProcessor.h"
#include "YawningDetector.h"
#include "cvUtil.h"
#include "mode.h"



/** Global variables */

cv::RNG rng(12345);

/** @function main */
int main(int argc, char* argv[]) {

  //
  cv::Mat frame;
  cv::namedWindow("Fatigue detection", CV_WINDOW_AUTOSIZE);
  // Mouth threshold adjust
  //int b = 50;
  //int g = 50;
  //int r = 90;
  int b = 25;
  int g = 25;
  int r = 70;
  cv::namedWindow("Mouth threshold", CV_WINDOW_AUTOSIZE);
  cv::createTrackbar("B", "Mouth threshold", &b, 255, NULL);
  cv::createTrackbar("G", "Mouth threshold", &g, 255, NULL);
  cv::createTrackbar("R", "Mouth threshold", &r, 255, NULL);

  FaceEyeDetector faceEyeDetector;
  EyeAnalyzer eyeAnalyzer;
  EyeballColorDetector eyeballColorDetector;
  YawningDetector yawningDetector;
  faceEyeDetector.setFaceCascade("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_alt_tree.xml");
  faceEyeDetector.setEyeCascade("C:\\opencv\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
//  faceEyeDetector.setMouthCascade("C:\\opencv\\data\\haarcascades\\haarcascade_mcs_mouth.xml");
  // Time
  double start = static_cast<double>(cv::getTickCount());
  double tickFreq = cv::getTickFrequency();
  double elapsed;
  // Statistic
  unsigned long frameCount = 0;
  double frameRate = 0;
  char text_c_str[100];
  std::string text;
#ifdef CAM_MODE
  cv::VideoCapture cap = cv::VideoCapture(0);
#endif

#ifdef VIDEO_MODE
  VideoProcessor vp;
  vp.setInputVideo("input.mov");
  vp.setOutputVideo("output.avi", CV_FOURCC('D', 'I', 'V', 'X'), 20, cvSize(1920, 1080), true);
#endif

#ifdef REMOTE_MODE
  MatServer matServer;
  matServer.setPort(50000);
  matServer.setupNetwork();
  matServer.doBlockedAccept();
#endif

#ifdef VIDEO_MODE
  while (vp.getNextFrameFromVideo(frame)) {
#else
  while(1) {
#endif

#ifdef REMOTE_MODE
    if (matServer.matRecv(frame) < 0) {
      break;
    }
#endif

#ifdef CAM_MODE
	cap>>frame;
#endif
    frameCount++;
    double loopStart = static_cast<double>(cv::getTickCount());
    faceEyeDetector.setFrame(frame);
    faceEyeDetector.doDetect();
    eyeballColorDetector.initialize(faceEyeDetector.getOriginalFrame(), faceEyeDetector.getEyeROIUpperLeft(), faceEyeDetector.getEyeBallROI(), faceEyeDetector.getEyeSampleColor());
    eyeballColorDetector.doDetect();
    eyeAnalyzer.setOriginalFrame(eyeballColorDetector.getOriginalFrame());
    eyeAnalyzer.setCliffDiff(eyeballColorDetector.getCliffDiff());
    eyeAnalyzer.doAnalyze();
    yawningDetector.setOriginalFrame(eyeAnalyzer.getOriginalFrame());
    yawningDetector.setMouthROI(faceEyeDetector.getMouthROI());
    yawningDetector.setMouthROIUpperLeft(faceEyeDetector.getMouthROIUpperLeft());
    yawningDetector.setMouthSampleColor(faceEyeDetector.getMouthSampleColor());
    yawningDetector.doDetect(Color(r, g, b));
    // Statistic
    elapsed = static_cast<double>(cv::getTickCount()) - start;
    elapsed /= tickFreq;
    // Print to terminal
    double loopElapsed = ((static_cast<double>(cv::getTickCount()) - loopStart) / tickFreq);
    std::cout << "Time to process this frame: " << (loopElapsed * 1000) << " ms" << std::endl;
    std::cout << "Time elapsed: " << (int)floor(elapsed) / 60 << " m " << (int)floor(elapsed) % 60 << " s" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    // Print to frame
    yawningDetector.getOriginalFrame().copyTo(frame);
    sprintf_s(text_c_str, "Frame rate = %f fps", 1 / loopElapsed);
    text.assign(text_c_str);
#ifndef VIDEO_MODE
    cvUtil::printText(frame, text, cv::Scalar(255, 255, 0), cv::Point(10, 170));
#endif

    // Show
#ifdef VIDEO_MODE
	vp.appendToVideo(yawningDetector.getOriginalFrame());
#endif
    if (frame.data) {
      imshow("Fatigue detection", frame);
    }
	if(cv::waitKey(24) == 'q')
		break;
  }

  std::cout << "Average frame rate: " << frameCount / elapsed << " fps" << std::endl;

//
//#else
//  VideoProcessor vp;
//  vp.setInputVideo("input.mov");
//  //vp.setOutputVideo("output.avi", CV_FOURCC('D','I','V','X'), 20, cvSize(568, 320), true);
//  vp.setOutputVideo("output.avi", CV_FOURCC('D', 'I', 'V', 'X'), 20, cvSize(1920, 1080), true);
//
//  while (vp.getNextFrameFromVideo(frame)) {
//    frameCount++;
//    double loopStart = static_cast<double>(cv::getTickCount());
//    faceEyeDetector.setFrame(frame);
//    faceEyeDetector.doDetect();
//    eyeballColorDetector.initialize(faceEyeDetector.getOriginalFrame(), faceEyeDetector.getEyeROIUpperLeft(), faceEyeDetector.getEyeBallROI(), faceEyeDetector.getEyeSampleColor());
//    eyeballColorDetector.doDetect();
//    eyeAnalyzer.setOriginalFrame(eyeballColorDetector.getOriginalFrame());
//    eyeAnalyzer.setCliffDiff(eyeballColorDetector.getCliffDiff());
//    eyeAnalyzer.doAnalyze();
//    yawningDetector.setOriginalFrame(eyeAnalyzer.getOriginalFrame());
//    yawningDetector.setMouthROI(faceEyeDetector.getMouthROI());
//    yawningDetector.doDetect();
//    // Statistic
//    elapsed = static_cast<double>(cv::getTickCount()) - start;
//    elapsed /= tickFreq;
//    std::cout << "Time to process this frame: " << ((static_cast<double>(cv::getTickCount()) - loopStart) / tickFreq * 1000) << " ms" << std::endl;
//    std::cout << "Time elapsed: " << (int)floor(elapsed) / 60 << " m " << (int)floor(elapsed) % 60 << " s" << std::endl;
//    std::cout << "--------------------------------------------------------------------------------" << std::endl;
//    frame = yawningDetector.getOriginalFrame();
//    sprintf_s(text_c_str, "Frame rate = %f fps", frameCount / elapsed);
//    text.assign(text_c_str);
//    cvUtil::printText(frame, text, cv::Scalar(255, 255, 0), cv::Point(10, 170));
//    // Append frame
//    vp.appendToVideo(yawningDetector.getOriginalFrame());
//    // Show
//    imshow("Fatigue detection", frame);
//    cv::waitKey(20);
//    std::cout << "Average frame rate: " << frameCount / elapsed << " fps" << std::endl;
//    system("pause");
//  }
//
//#endif
  return 0;
}


