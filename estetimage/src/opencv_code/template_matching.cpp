#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <string>

#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include "gphoto2/gphoto2-camera.h"

Camera *camera;  
GPContext *context; 

#include "include/params.h"
#include "include/parameters.hpp"
#include "include/capture.hpp"
#include "include/notifications.hpp"
#include "include/config.hpp"
#include "include/widget.hpp"

//Compilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main
//ou, cd build_estetimacge ; cmake ../estetimacge ; make -j ; ./estetimacge
using namespace cv;
using namespace std;
/// Global Variables
Mat img; Mat templ; Mat result;


int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and matchTemplate
  img = imread( argv[1], 1 );
  templ = imread( argv[2], 1 );

  /// Create windows
  namedWindow( "Source Image", CV_WINDOW_AUTOSIZE );
  namedWindow( "Result window", CV_WINDOW_AUTOSIZE );

  /// Create Trackbar
  String trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, "Source Image", &match_method, max_Trackbar, MatchingMethod );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( "Source Image", img_display );
  imshow( "Result window", result );

  return;
}