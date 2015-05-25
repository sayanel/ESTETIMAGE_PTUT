#include "include/analyse.hpp"

//using namespace kn;
using namespace cv;
using namespace std;


int getPixelColorType(int H, int S, int V){
        String color;

        int icolor = 0;

        if (V < 75)
            { color = "cBLACK"; icolor = 1; }
        else if (V > 190 && S < 27)
            { color = "cWHITE"; icolor = 2; }
        else if (S < 53 && V < 185)
            { color = "cGREY"; icolor = 3; }
        else {  // Is a color
            if (H < 14)
                { color = "cRED"; icolor = 4; }
            else if (H < 25)
                { color = "cORANGE"; icolor = 5; }
            else if (H < 34)
                { color = "cYELLOW"; icolor = 6; }
            else if (H < 73)
                { color = "cGREEN"; icolor = 7; }
            else if (H < 102)
                { color = "cAQUA"; icolor = 8; }
            else if (H < 127)
                { color = "cBLUE"; icolor = 9; }
            else if (H < 149)
                { color = "cPURPLE"; icolor = 10; }
            else if (H < 175)
                { color = "cPINK"; icolor = 11; }
            else    // full circle 
                { color = "cRED";  icolor = 4; }// back to Red
        }

        // cout << "global color: " << color << "icolor: " << icolor << endl;
        return icolor;
}


int hsvrgb(Mat src, int & icolor, int & h, int & s, int & v, int & r, int & g, int & b){
  Mat dst;

  /// Load image

  if( !src.data )
    { return -1; }

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  //namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  //imshow("calcHist Demo", histImage );


  Mat hsv;
  // hsv = cvCloneImage(src);
  // cvCvtColor(src, hsv, CV_BGR2HSV);
  cvtColor(src, hsv, CV_BGR2HSV);

  //imshow("HSV", hsv );

  //H = hue = teinte
  //S = saturation
  //V = value/lightness
  h=0, s=0, v=0; 

  int nb_pixels = hsv.cols * hsv.rows;
  //String colors[] = {"cBLACK", "cWHITE", "cGREY", "cRED", "cORANGE", "cYELLOW", "cGREEN", "cAQUA", "cBLUE", "cPURPLE", "cPINK", "cRED"};
  for (int i = 0; i < hsv.rows; i++)
  {
      for (int j = 0; j < hsv.cols; j++)
      {
         //HSV
         h += (int)hsv.at<Vec3b>(i,j)[0];
         s += (int)hsv.at<Vec3b>(i,j)[1];
         v += (int)hsv.at<Vec3b>(i,j)[2];

         //BGR
         r += (int)src.at<Vec3b>(i,j)[2]; 
         g += (int)src.at<Vec3b>(i,j)[1];
         b += (int)src.at<Vec3b>(i,j)[0];

          
      }
  }

  h = h/nb_pixels;
  s = s/nb_pixels;
  v = v/nb_pixels;

  r = r/nb_pixels;
  g = g/nb_pixels;
  b = b/nb_pixels;

  // cout<<"h = " << h << " s = " << s << " v = " << v << " nb_pixels = " << nb_pixels << endl;
  // cout<<"r = " << r << " g = " << g << " b = " << b << " nb_pixels = " << nb_pixels << endl;
  icolor = getPixelColorType(h,s,v);


  return 1;

}

void detectPortrait(Mat & src, int & isPortrait, int & nbPers){
    nbPers = faceDetector(src, isPortrait);
}

void detectContour(Mat & src, int &nbContours){
    nbContours = contourDetector(src);
}


void calculatevarianceRGBHSV(Mat & src, int mean_r, int mean_g, int mean_b, int & var_r, int & var_g, int & var_b, int mean_h, int mean_s, int mean_v, int & var_h, int & var_s, int & var_v){

  int height = src.rows;
  int width = src.cols;

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      
      var_b += ((int)src.at<Vec3b>(i,j)[0] - mean_b) * ((int)src.at<Vec3b>(i,j)[0] - mean_b); 
      var_g += ((int)src.at<Vec3b>(i,j)[1] - mean_g) * ((int)src.at<Vec3b>(i,j)[1] - mean_g); 
      var_r += ((int)src.at<Vec3b>(i,j)[2] - mean_r) * ((int)src.at<Vec3b>(i,j)[2] - mean_r); 
      var_h += ((int)src.at<Vec3b>(i,j)[0] - mean_h) * ((int)src.at<Vec3b>(i,j)[0] - mean_h); 
      var_s += ((int)src.at<Vec3b>(i,j)[1] - mean_s) * ((int)src.at<Vec3b>(i,j)[1] - mean_s); 
      var_v += ((int)src.at<Vec3b>(i,j)[2] - mean_v) * ((int)src.at<Vec3b>(i,j)[2] - mean_v); 

    }
  }

  var_b = sqrt(fabs(var_b / (height * width)));
  var_g = sqrt(fabs(var_g / (height * width)));
  var_r = sqrt(fabs(var_r / (height * width)));
  var_h = sqrt(fabs(var_h / (height * width)));
  var_s = sqrt(fabs(var_s / (height * width)));
  var_v = sqrt(fabs(var_v / (height * width)));

}


int analyseImageToBuildVector(Mat src, VectorXd & xx){

  
        //************ H S V & R G B ********************************//
      int icolor = 0, h = 0, s = 0, v = 0, r = 0, g = 0, b = 0, var_r = 0, var_g = 0, var_b = 0, var_h = 0, var_s = 0, var_v = 0;
      hsvrgb(src, icolor, h, s, v, r, g, b);

      //************ VARIANCE **************************************//
      calculatevarianceRGBHSV(src, r, g, b, var_r, var_g, var_b, h, s, v, var_h, var_s, var_v);

      //************ CONTOURS *************************************//
      int isPortrait = 0; int nbPers = 0;
      detectPortrait(src, isPortrait, nbPers);

      //************ FACE DETECTION *******************************//
      int nbContours = 0;
      detectContour(src, nbContours);


      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "global color: " << icolor << endl;
      cout << "global hue: " << h << " var: " << var_h << endl;
      cout << "global saturation: " << s <<" var: " << var_s <<  endl;
      cout << "global lightness/value: " << v <<" var: " << var_v <<  endl;
      cout << "avg red: " << r << " var: " << var_r << endl;
      cout << "avg green: " << g << " var: " << var_g << endl;
      cout << "avg blue: " << b << " var: " << var_b << endl;
      cout << "Portrait (0/1): " << isPortrait << endl;
      cout << "Nombre faces: " << nbPers << endl;
      cout << "Nombre contours: " << nbContours << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;


      //nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, 
      //var_h, var_s, var_v, mean_red, mean_green, mean_blue, var_red, var_green, var_blue
      xx << nbContours, isPortrait, nbPers,  icolor, h, s, v, var_h, var_s, var_v, r, g, b, var_r, var_g, var_b;
      //WARNING: add isPortrait & nbfaces later with value 1 minimum

  return 1;
}