#include "FaceEyeDetector.h"
#include "cvUtil.h"

void FaceEyeDetector::doDetect(void) {
  cv::Mat frame_gray;
  cvtColor(frame, frame_gray, CV_BGR2GRAY);
  equalizeHist(frame_gray, frame_gray);
  //-- Detect faces
  face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(250, 250));

  for (int i = 0; i < (int)faces.size(); i++) {
    // Face center
    cv::Point center((int)(faces[i].x + faces[i].width * 0.5), (int)(faces[i].y + faces[i].height * 0.5));
    // Select ROI
    // Face ROI
    frame(faces[i]).copyTo(faceROI);
    // Eye related
    // Potential eye ROI
    cv::Rect potentialEyeRect(faces[i].x + (int)(faces[i].width * 0.1), faces[i].y + (int)(faces[i].height * 0.25), (int)(faces[i].width * 0.4), (int)(faces[i].height * 0.35));
    cv::Mat potentialEyeROI = frame_gray(potentialEyeRect);
    // Mark the potential eye ROI
    rectangle(frame, potentialEyeRect, cv::Scalar(255, 0, 0), 2);
    // In each face, detect eyes
    eyes_cascade.detectMultiScale(potentialEyeROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    for (int j = 0; j < (int)eyes.size(); j++) {
      eyeCenter.x = (int)(potentialEyeRect.x + eyes[j].x + eyes[j].width * 0.5);
      eyeCenter.y = (int)(potentialEyeRect.y + eyes[j].y + eyes[j].height * 0.5);
      eyeSampleColor.b = cvUtil::getB(frame, cv::Point(eyeCenter.x, eyeCenter.y));
      eyeSampleColor.g = cvUtil::getG(frame, cv::Point(eyeCenter.x, eyeCenter.y));
      eyeSampleColor.r = cvUtil::getR(frame, cv::Point(eyeCenter.x, eyeCenter.y));
      // Mark eye center
      cvUtil::printDot(frame, eyeCenter, cv::Scalar(255, 255, 255), 3);
      //cv::Rect rect(cv::Point(eyeCenter.x - EYE_ROI_RADIUS_W, eyeCenter.y - EYE_ROI_RADIUS_H), cv::Point(eyeCenter.x + EYE_ROI_RADIUS_W, eyeCenter.y + EYE_ROI_RADIUS_H));
      //eyeROI = frame(rect);
    }

    if ((eyeCenter.x - EYE_ROI_RADIUS_W >= 0) &&
        (eyeCenter.x - EYE_ROI_RADIUS_W < FRAME_WIDTH) &&
        (eyeCenter.x + EYE_ROI_RADIUS_W >= 0) &&
        (eyeCenter.x + EYE_ROI_RADIUS_W < FRAME_WIDTH) &&
        (eyeCenter.y - EYE_ROI_RADIUS_H >= 0) &&
        (eyeCenter.y - EYE_ROI_RADIUS_H < FRAME_HEIGH) &&
        (eyeCenter.y + EYE_ROI_RADIUS_H >= 0) &&
        (eyeCenter.y + EYE_ROI_RADIUS_H < FRAME_HEIGH)) {
      cv::Rect eyeBallRect(cv::Point(eyeCenter.x - EYE_ROI_RADIUS_W, eyeCenter.y - EYE_ROI_RADIUS_H), cv::Point(eyeCenter.x + EYE_ROI_RADIUS_W, eyeCenter.y + EYE_ROI_RADIUS_H));
      frame(eyeBallRect).copyTo(eyeBallROI);
    }

    // Mouth related
    // Mouth ROI
    double mouthROIub = 0.7;
    double mouthROIlb = 0.3;

    if ((faces[i].y + faces[i].height * (mouthROIub + mouthROIlb)) >= frame.rows) {
      mouthROIlb = 1 - mouthROIub;
    }

    cv::Rect mouthROIRect(faces[i].x + (int)(faces[i].width * 0.3), faces[i].y + (int)(faces[i].height * mouthROIub), (int)(faces[i].width * 0.4), (int)(faces[i].height * mouthROIlb));
    mouthUpperLeft.x = faces[i].x + (int)(faces[i].width * 0.3);
    mouthUpperLeft.y = faces[i].y + (int)(faces[i].height * mouthROIub);
    frame(mouthROIRect).copyTo(mouthROI);
    // Mark the potential mouth ROI
    rectangle(frame, mouthROIRect, cv::Scalar(255, 0, 0), 2);
    // In each face, detect mouth
    //mouth_cascade.detectMultiScale(mouthROI, mouth, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));
    //for (int j = 0; j < (int)mouth.size(); j++) {
    //  cv::Point center((int)(mouthROIRect.x + mouth[j].x + mouth[j].width * 0.5), (int)(mouthROIRect.y + mouth[j].y + mouth[j].height * 0.5));
    //  int radius = cvRound((mouth[j].width + mouth[j].height) * 0.25);
    //  circle(frame, center, radius, cv::Scalar(0, 255, 0), 4, 8, 0);
    //  if (!(frame.data[center.y * frame.step + frame.channels() * center.x + 0] > 170 && frame.data[center.y * frame.step + frame.channels() * center.x + 1] > 170 && frame.data[center.y * frame.step + frame.channels() * center.x + 2] > 170) &&
    //      !(frame.data[center.y * frame.step + frame.channels() * center.x + 0] < 40 && frame.data[center.y * frame.step + frame.channels() * center.x + 1] < 40 && frame.data[center.y * frame.step + frame.channels() * center.x + 2] < 40)) {
    //    mouthSampleColor.b = frame.data[center.y * frame.step + frame.channels() * center.x + 0];
    //    mouthSampleColor.g = frame.data[center.y * frame.step + frame.channels() * center.x + 1];
    //    mouthSampleColor.r = frame.data[center.y * frame.step + frame.channels() * center.x + 2];
    //  }
    //  // Mark mouth center
    //  cvUtil::printDot(frame, center, cv::Scalar(255, 255, 255), 6);
    //}
    // Draw ellipse
    ellipse(frame, center, cv::Size((int)(faces[i].width * 0.5), (int)(faces[i].height * 0.5)), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
  }
}

