#include "include/contourDetector.hpp"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 40;
int max_thresh = 255;
//RNG rng(12345);


/** @function main */
int contourDetector(Mat & image){
  /// Load source image and convert it to gray
  //src = imread( argv[1], 1 );
  src = image;
  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback_bis );
  int cptContours = 0;
  cptContours = thresh_callback(image, 0, 0 );
  thresh_callback_bis( 0, 0 );

  return cptContours;
}

/** @function thresh_callback */
int thresh_callback(Mat & image, int, void* ){
  RNG rng(12345);
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  int cpt = 0;
  unsigned int size_contours = contours.size();
  for( unsigned int i = 0; i< size_contours; i++ )
     {
       cpt++;
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( image, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

     return cpt;
}

/** @function thresh_callback */
void thresh_callback_bis(int, void* ){
  RNG rng(12345);
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  unsigned int size_contours = contours.size();
  for( unsigned int i = 0; i< size_contours; i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }
}