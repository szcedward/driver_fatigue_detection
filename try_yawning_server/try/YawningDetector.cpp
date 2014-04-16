#include "YawningDetector.h"
#include "FaceEyeDetector.h"
#include "mode.h"

YawningDetector::YawningDetector(void) {
#ifdef VIDEO_MODE
  yawningThresh = 70;
#else
  yawningThresh = 30;
#endif
}

YawningDetector::YawningDetector(const cv::Mat& originalFrame, const cv::Mat& mouthROI) {
#ifdef VIDEO_MODE
  yawningThresh = 70;
#else
  yawningThresh = 35;
#endif
  originalFrame.copyTo(this->originalFrame);
  mouthROI.copyTo(this->mouthROI);
}

YawningDetector::~YawningDetector(void) {
}

void YawningDetector::setOriginalFrame(const cv::Mat& frame) {
  frame.copyTo(originalFrame);
}

cv::Mat YawningDetector::getOriginalFrame() {
  return originalFrame;
}

void YawningDetector::setMouthROI(const cv::Mat& frame) {
  frame.copyTo(mouthROI);
}

void YawningDetector::setMouthROIUpperLeft(const cv::Point& mouthUpperLeft)
{
	this->mouthUpperLeft = mouthUpperLeft;
}

void YawningDetector::setMouthSampleColor(const Color& color) {
  this->mouthSampleColor = color;
}

void YawningDetector::doDetect(const Color& color) {
	if (!mouthROI.data)
		return;
  imshow("MouthROI", mouthROI);
  cv::Mat mouthROIGray(mouthROI);
  /*for (int j = 0; j < mouthROI.rows; j++)
  {
    for (int i = 0; i < mouthROI.cols; i++) {
  	  if(
  		  (abs(mouthROI.data[j * mouthROI.step + mouthROI.channels() * i + 0] - mouthSampleColor.b) < 10) &&
  		  (abs(mouthROI.data[j * mouthROI.step + mouthROI.channels() * i + 1] - mouthSampleColor.g) < 10) &&
  		  (mouthROI.data[j * mouthROI.step + mouthROI.channels() * i + 2] - mouthSampleColor.r < 20) &&
  		  (mouthROI.data[j * mouthROI.step + mouthROI.channels() * i + 2] - mouthSampleColor.r > -5))
  	  {
  	  cvUtil::setMatColor(mouthROIGray, cv::Point(i, j), cv::Scalar(255, 255, 255));
  	  }
  	  else
  	  {
  		  cvUtil::setMatColor(mouthROIGray, cv::Point(i, j), cv::Scalar(0,0,0));
  	  }
    }
  }*/

  for (int j = 0; j < mouthROI.rows; j++) {
    for (int i = 0; i < mouthROI.cols; i++) {
      if ((cvUtil::getB(mouthROI, cv::Point(i, j)) < color.b) &&
          (cvUtil::getG(mouthROI, cv::Point(i, j)) < color.g) &&
          (cvUtil::getR(mouthROI, cv::Point(i, j)) < color.r)) {
        cvUtil::setMatColor(mouthROIGray, cv::Point(i, j), cv::Scalar(255, 255, 255));
      } else {
        cvUtil::setMatColor(mouthROIGray, cv::Point(i, j), cv::Scalar(0, 0, 0));
      }
    }
  }

  cv::cvtColor(mouthROIGray, mouthROIGray, CV_RGB2GRAY);
  blur(mouthROIGray, mouthROIGray, cv::Size(3,3) );
  //cv::erode(mouthROIGray, mouthROIGray, cv::Mat());
  //cv::dilate(mouthROIGray, mouthROIGray, cv::Mat());
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mouthROIGray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

  

  if (contours.size() == 0) {
	  std::cout << "No connected component detected." << std::endl;
  } else {
	  int maxLength = 0;
	  int maxIndex = 0;
	  for(int k = 0; k < (int)contours.size(); k++)
	  {
		  if((int)contours[k].size() > maxLength)
		  {
			  maxLength = contours[k].size();
			  maxIndex = k;
		  }
	  }
	  if(maxLength > 30)
	  {
		  for (int i = 0; i < maxLength; i++) {
			  int j = (i + 1) % maxLength;
			  cv::line(mouthROIGray, (contours[maxIndex])[i], (contours[maxIndex])[j], CV_RGB(0, 0, 255), 2, 8, 0);
			  cv::line(originalFrame, cv::Point((contours[maxIndex])[i] + mouthUpperLeft), cv::Point((contours[maxIndex])[j] + mouthUpperLeft), CV_RGB(0, 255, 0), 2, 8, 0);
		  }
		  int mouthDiff = findMouthDiff(contours[maxIndex]);
		  std::cout<<"mouthDiff = "<<mouthDiff<<std::endl;
		  if (mouthDiff > yawningThresh) {
			  std::string text = "Yawning";
			  cvUtil::printText(originalFrame, text, cv::Scalar(0, 0, 255), cv::Point(10, 130));
		  }
	  }
  }
  

  /*
    // Convert to YCrCb
    cv::Mat mouthROI_YCrCb;
    cv::cvtColor(mouthROI, mouthROI_YCrCb, CV_BGR2YCrCb);

    double sumCrSq = 0;
    double sumCrOverCb = 0;
    for(int i = 0; i < mouthROI.rows; i++) {
      for(int j = 0; j < mouthROI.cols; j++) {
        // Get YCrCb component
        cv::Vec3b pix_ycrcb = mouthROI_YCrCb.ptr<cv::Vec3b>(i)[j];
        int Y = pix_ycrcb.val[0];
        int Cr = pix_ycrcb.val[1];
        int Cb = pix_ycrcb.val[2];

        sumCrSq += Cr * Cr;
        sumCrOverCb += (Cr / Cb);
      }
    }
    // Compute eta
    double eta = 0.95 * (sumCrSq / sumCrOverCb);

    if (eta > 255) {
      std::cout << "eta is greater than 255 !!!!!!!!!!!" << std::endl;
    }

    for(int i = 0; i < mouthROI.rows; i++) {
      for(int j = 0; j < mouthROI.cols; j++) {
        // Get YCrCb component
        cv::Vec3b pix_ycrcb = mouthROI_YCrCb.ptr<cv::Vec3b>(i)[j];
        int Y = pix_ycrcb.val[0];
        int Cr = pix_ycrcb.val[1];
        int Cb = pix_ycrcb.val[2];

        double mouthMap = (Cr * Cr) * pow((Cr * Cr) - (eta * Cr / Cb), 2);
  //      std::cout << "Mouth Map: " << mouthMap << std::endl;
        mouthROI.data[i * mouthROI.step + mouthROI.channels() * j + 0] = mouthMap;
        mouthROI.data[i * mouthROI.step + mouthROI.channels() * j + 1] = mouthMap;
        mouthROI.data[i * mouthROI.step + mouthROI.channels() * j + 2] = mouthMap;
      }
    }
    */
  imshow("MouthROI processed", mouthROIGray);
}

int YawningDetector::findMouthDiff(const std::vector<cv::Point>& contour)
{
	int minY = FRAME_HEIGH;
	int maxY = 0;
	for(int i = 0; i < (int)contour.size(); i++)
	{
		if(contour[i].y < minY)
		{
			minY = contour[i].y;
		}
		if(contour[i].y > maxY)
		{
			maxY = contour[i].y;
		}
	}

	return maxY - minY;
}