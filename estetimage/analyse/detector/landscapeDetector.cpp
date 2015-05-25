#include "include/landscapeDetector.hpp"



/*

int detectAndDisplayLandscape (Mat & frame){

	//Mat frame_gray;
	Mat back;
	Mat fore;

	BackgroundSubtractorMOG2 bg;
	bg.nmixtures = 3; // set number of gaussian mixtures
	bg.bShadowDetection = false; // turn the shadow detection off

	std::vector<std::vector<cv::Point> > contours;

	bg.operator ()(frame,fore);
	bg.getBackgroundImage(back);

	erode(fore,fore,cv::Mat());
	dilate(fore,fore,cv::Mat());


	findContours( fore, // binary input image
                               contours, // vector of vectors of points
                               CV_RETR_EXTERNAL, // retrieve only external contours
                               CV_CHAIN_APPROX_NONE); // detect all pixels of each contour
 
	drawContours( frame, // draw contours here
                                  contours, // draw these contours
                                  -1, // draw all contours
                                  cv::Scalar(0,0,255), // set color
                                  2); // set thickness


}

*/