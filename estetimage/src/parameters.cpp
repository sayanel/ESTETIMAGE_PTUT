#include "include/parameters.hpp"
#include "include/config.hpp"
#include <string.h>

using namespace std;


int update_parameters(Camera *camera, GPContext *context, float aperture, std::string shutterspeed, std::string iso){

	char buffer[32];
	//char buffer3[32];
	//char * chaine;
	
	//get_config_value_string(camera,"aperture",&chaine,context);
	std::cout << "****************************" << std::endl;
	//if(*chaine != aperture){
		if((float)aperture - (int)aperture != 0) snprintf(buffer,32,"%.1f",aperture);
		else snprintf(buffer,32,"%.f",aperture);
  		set_config_value_string(camera,"aperture",buffer,context);
  		std::cout << "Modification de aperture : "<< buffer<<std::endl;
  	//}

	int size_shutterspeed = shutterspeed.size();
	char str_shutterspeed[size_shutterspeed];
	for(int i = 0; i< size_shutterspeed; ++i){
		str_shutterspeed[i] = shutterspeed[i];
	}
	str_shutterspeed[size_shutterspeed] = '\0';

  	set_config_value_string(camera,"shutterspeed",str_shutterspeed,context);
  	std::cout << "Modification de shutterspeed : "<< str_shutterspeed <<std::endl;
  	//}


  	//get_config_value_string(camera,"iso",&chaine,context);
  	//if(*chaine != iso){
	int size_iso = iso.size();
	char str_iso[size_iso];
	for(int i = 0; i< size_iso; ++i){
		str_iso[i] = iso[i];
	}
	str_iso[size_iso] = '\0';

  	set_config_value_string(camera,"iso",str_iso,context);
  	std::cout << "Modification de iso : "<< str_iso <<std::endl;
  	//}


	std::cout << "****************************" << std::endl;
	std::cout << "****************************" << std::endl << std::endl;

	return 1;
}

void print_parameters(Camera *camera, GPContext *context){
	char * chaine;
	std::cout << "****************************" << std::endl;
	get_config_value_string(camera,"aperture",&chaine,context);
	std::cout<<"Aperture = " << chaine <<std::endl;

	get_config_value_string(camera,"shutterspeed",&chaine,context);
	std::cout<<"shutterspeed : " << chaine <<std::endl;

	get_config_value_string(camera,"iso",&chaine,context);
	std::cout<<"iso : " << chaine <<std::endl;

	/*
	get_config_value_string(camera,"whitebalance",&chaine,context);
	std::cout<<"whitebalance : " << chaine <<std::endl;

	get_config_value_string(camera,"meteringmode",&chaine,context);
	std::cout<<"meteringmode : " << chaine <<std::endl;

	get_config_value_string(camera,"manualfocusdrive",&chaine,context);
	std::cout<<"manualfocusdrive : " << chaine <<std::endl;

	get_config_value_string(camera,"eoszoomposition",&chaine,context);
	std::cout<<"eoszoomposition : " << chaine <<std::endl;
	*/

	std::cout << "****************************" << std::endl;
	std::cout << "****************************" << std::endl << std::endl;
}

