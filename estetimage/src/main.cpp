#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

//#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "gphoto2/gphoto2-camera.h"

#include "include/params.h"
#include "include/parameters.hpp"
#include "include/capture.hpp"
#include "include/notifications.hpp"
#include "include/config.hpp"
#include "include/widget.hpp"
#include "include/pca.hpp"
#include "include/analyse.hpp"
#include "include/parametersToIndex.hpp"
#include "include/filter.hpp"

#include <sqlite3.h> 

//Compilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main
//ou, cd build_estetimacge ; cmake ../estetimacge ; make -j ; ./estetimacge

using namespace cv;
using namespace std;

Camera *camera;  
GPContext *context; 



void displayImages(int i, char * filename, Mat inputMat, int & decalagex, int & decalagey){

    int width = 400; 
    int height = 200;

    //int width = 160;
    //int height = 120;

    cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
    resize(inputMat, inputMat, cvSize(width, height));
    cvMoveWindow(filename, decalagex, decalagey);
    decalagex += width + 50;
    //if(i%2 == 0 && i != 0 && i!= 4){ decalagey += height + 75; decalagex = 30; }
    if(i == 1 || i == 3){ decalagey += height + 25; decalagex = 10; }
    imshow(filename, inputMat);

}

 /** @function main */

 int main( int argc, const char** argv ){
 
    cout << "*** ESTETIMAGE ***" << endl;

    //lecture de l'image
    //src = imread( argv[1], 1 );

    //init
	    //nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, 
	    //var_h, var_s, var_v, mean_red, mean_green, mean_blue, var_red, var_green, var_blue
    VectorXd xx(16);
    Mat src;
    vector<Parameters> best_params;
    vector<int> aperture_best_index;
    vector<int> shutterspeed_best_index;
    vector<int> iso_best_index;
    char filename[256];
    gp_camera_new (&camera);
    context = gp_context_new();

    int ret = gp_camera_init(camera, context);
    if (ret < GP_OK) {
      printf("L'appareil photo n'est pas détecté, essayez de démonter le lanceur dans votre barre de tâches ou de retirer la carte mémoire de l'appareil photo.\n");
      gp_camera_free(camera);
      return 1;
     }

    //capture de la première image
    set_config_value_string(camera,"whitebalance","Auto",context);
    //Si la ligne du dessous est commentée la 1ère photo sera prise avec les paramètres actuels de l'appareil sinon vous pouvez choisir des paramètres généraux ici:
    //update_parameters(camera, context, aperture_tab[4], shutterspeed_tab[26], iso_tab[1]);
    capture(camera, context, filename); 
    src = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
    resize(src, src, cvSize(640, 480));
    snprintf(filename, 256, "firstshotauto");
    //imshow(filename, src);

    Mat cpy_src = src;

    //construction du vecteur comportant toute l'analyse de notre image
    analyseImageToBuildVector(src, xx);
    
    //comparaison de ce vecteur avec la matrice pour récupérer les correspondances
    getMatchesWitchPCA(xx, best_params);

    //convert les valeurs de best_params en index des tableaux aperture_tab, shutterspeed_tab et iso_tab
    convertBestParamsToIndex(best_params, aperture_best_index, shutterspeed_best_index, iso_best_index);

    //photos

    int const nShots = 4;
    int i;
    Mat img[10];
    int decalagex = 30;
    int decalagey = 30;


    for (i = 0; i < nShots; i++) {
        snprintf(filename, 256, "shot-%04d.jpg", i);
        printf("Capturing to file %s\n", filename);
        //print_parameters(camera, context);

        int ape_i = aperture_best_index[i];
        int shu_i = shutterspeed_best_index[i];
        int iso_i = iso_best_index[i];
        //iso_i = 0; //auto
      
        if(i==2) set_config_value_string(camera,"whitebalance","Tungsten",context);
        if(i==3) set_config_value_string(camera,"whitebalance","Daylight",context);
        update_parameters(camera, context, aperture_tab[ape_i], shutterspeed_tab[shu_i], iso_tab[iso_i]);

        capture(camera, context, filename);

        img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
      
        displayImages(i, filename, img[i], decalagex, decalagey);

        
     }

     //white balance 
     update_parameters(camera, context, aperture_tab[7], shutterspeed_tab[28], iso_tab[1]);
     set_config_value_string(camera,"whitebalance","Shadow",context);
     snprintf(filename, 256, "whitebalance");
     capture(camera, context, filename);
     img[4] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
     displayImages(4, filename, img[4], decalagex, decalagey);

     //filtres
     Mat imgWithFilter;
      //img[6] = cartoonMatConversion(img[0]);
      img[5] = softFocusConversion(img[0]); //cool
     // imgWithFilter =  grayMatConversion(img[0]);
     // imgWithFilter =  yuvMatConversion(img[0]);
     // imgWithFilter =  inverseMatConversion(img[0]);
     // imgWithFilter =  sepiaConversion(img[0]);
     // imgWithFilter =  sketchConversion(img[0]);
     // imgWithFilter =  pencilSketchConversion(img[0]);
     // imgWithFilter =  retroEffectConversion(img[0]);
     // imgWithFilter =  filmGrainConversion(img[0]);
     // imgWithFilter =  pinholeCameraConversion(img[0]);

      //displayImages(6, filename, img[6], decalagex, decalagey);

      snprintf(filename, 256, "soft focus filter");
      displayImages(5, filename, img[5], decalagex, decalagey);




     // close camera et release les matrices images
     gp_camera_unref(camera);
     gp_context_unref(context);

      cvWaitKey(0);
      cvDestroyAllWindows();
      for(i = 0; i < nShots; i++){
        img[i].release();
      }


  return 0;
}

