//int retval;
    gp_camera_new (&camera);
    context = gp_context_new();

 // set callbacks for camera messages
 //gp_context_set_error_func(context, error_func, NULL);
 //gp_context_set_message_func(context, message_func, NULL);

 //This call will autodetect cameras, take the first one from the list and use it
 printf("Camera init. Can take more than 10 seconds depending on the "
  "memory card's contents (remove card from camera to speed up).\n");
 int ret = gp_camera_init(camera, context);
 if (ret < GP_OK) {
  printf("No camera auto detected.\n");
  gp_camera_free(camera);
  return 1;
 }

 // take 10 shots
 char filename[256];
 int const nShots = 1;
 int i;


//Image de base
//IplImage* basic = cvCreateImage(cvSize(320, 240), 8, 3);
//IplImage* img[10] = {NULL};
//const char* window_title = "Hello, OpenCV!";
//cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);

cv::Mat img[10];// = {NULL}; //= cv::imread("something.jpg");
//cv::Mat src = cv::imread("src.jpg");

// Resize src so that is has the same size as img
//cv::resize(src, src, img.size());
 
int decalagex = 30;
int decalagey = 30;
 // do some capturing

//const float val = 0.0;
 
 //sprintf(buffer,"%f",val);
 /*retval =set_config_value_string(camera,"aperture","25",context);
		if (retval != GP_OK) {
			printf("  failed setting shutterspeed to %f: %d\n", val, retval);


			exit (1);
		}*/


//gphoto2 --list-config

 for (i = 1; i <= nShots; i++) {
  snprintf(filename, 256, "shot-%04d.nef", i);
  printf("Capturing to file %s\n", filename);
  print_parameters(camera, context);
  update_parameters(camera, context, aperture_tab[i+3],shutterspeed_tab[24-i],iso_tab[0]);

  // set_config_value_string(camera,"whitebalance","Auto",context);
  // set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  // set_config_value_string(camera,"focusmode","AI Focus",context);
  // set_config_value_string(camera,"eoszoom","1",context);
  // set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  // set_config_value_string(camera,"eoszoomposition","5,5",context);
  // set_config_value_string(camera,"whitebalanceadjusta","15",context);
  // set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  



/* system, ne fonctionne pas car le programme c++ a la main sur l'appareil
	int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  std::cout << "Executing command DIR..." << std::endl;
  i=system ("gphoto2 --set-config whitebalance=2");
  printf ("The value returned was: %d.\n",i);
*/

  capture(camera, context, filename);
  print_parameters(camera, context);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 

  /*if (img[i] == NULL)
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }*/
  //cvResize(img[i],img[i], cvSize(320, 240));
  
    /* comments car pas de capture */
  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
  cv::imshow(filename,img[i]);
  

  //cvShowImage (window_title, img[i]);
 }

 // close camera
 gp_camera_unref(camera);
 gp_context_unref(context);

//END OF GPHOTO PART
 //STARTING OPENCV

  
  /*if (argc < 2)
  {
    fprintf (stderr, "usage: %s IMAGE\n", argv[0]);
    return EXIT_FAILURE;
  }*/
  
  
  
  //cvShowImage (window_title, img[1]);
  cvWaitKey(0);
  cvDestroyAllWindows();
  for(i = 0; i < nShots; i++){
    img[i].release();
    //cvReleaseImage(&img[i]);
  }