#include "include/globalTintDetector.hpp"



int globalTintDetector(Mat & image){
	int rows, cols;
	uint r=0; uint g=0; uint b=0;

	rows = image.rows;
	cols = image.cols;

	for(int i=0; i<rows; ++i){
		for(int j=0; j<cols; ++j){
			//BGR
			b += image.at<cv::Vec3b>(i,j)[0];
			g += image.at<cv::Vec3b>(i,j)[1];
			r += image.at<cv::Vec3b>(i,j)[2];
		}
	}
	int max = rows*cols*256 /100 ;

 	r = r/100  * 256 / max;
 	g = g/100  * 256 / max;
 	b = b/100  * 256 / max;
 
 	cout << "(" << r << "," << g << "," << b << ")" << endl;


 	// Mat new_image = Mat::zeros( image.rows, image.cols);
 	int t = 100;
 	Mat new_image = Mat::zeros( Size(t,t), image.type());
 	for( int y = 0; y < t; y++ ){
 		for( int x = 0; x < t; x++ ){
      			// new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
      			new_image.at<Vec3b>(y,x)[0] = b;
      			new_image.at<Vec3b>(y,x)[1] = g;
      			new_image.at<Vec3b>(y,x)[2] = r;
    	}
    }
 namedWindow("Global Tint", 1);
 imshow("Global Tint", new_image);
 imwrite( "../presentation/globalTintMontagne.jpg", new_image );

	//Mat color[3] = Mat(1,1, CV_64F, 0);
    // color.at<double>(0,0) = 0;

 // 	for(int i=0; i<300; ++i){
	// 	for(int j=0; j<300; ++j){
	// 		// color.at<cv::Vec3b>(i,j)[0] = b;
	// 		// color.at<cv::Vec3b>(i,j)[1] = g;
	// 		// color.at<cv::Vec3b>(i,j)[2] = r;
	// 	}
	// }

	//imshow("Couleur dominante", color );

	return 1;
}














