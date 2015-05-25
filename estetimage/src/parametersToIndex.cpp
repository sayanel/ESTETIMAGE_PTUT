#include "include/parametersToIndex.hpp"


int getApertureIndex(float aperture){
    if(aperture == 0) return -1;
    for(int i = 0; i < 17; i++){
        if(aperture <= aperture_tab[i]) return i;
    }
    return -1;
}

int getShutterspeedIndex(float shutterspeed){
    //if(shutterspeed == 0) return -1;
    for(int i = 0; i < 44; i++){
        if(shutterspeed >= shutterspeed_tab_convert[i]) return i;
    }
    return -1;
}

int getISOIndex(int iso){
    if(iso == 0) return 0; //auto
    for(int i = 0; i < 8; i++){
        if(iso <= iso_tab_convert[i]) return i;
    }
    return 0; //auto
}

void convertBestParamsToIndex(vector<Parameters> & best_params, vector<int> & aperture_best_index, vector<int> & shutterspeed_best_index, vector<int> & iso_best_index){

    for(int i = 0; i < best_params.size(); ++i){

        int aperture_index = getApertureIndex(best_params[i].aperture);
        aperture_best_index.push_back(aperture_index);
        int shutterspeed_index = getShutterspeedIndex(best_params[i].shutterspeed);
        shutterspeed_best_index.push_back(shutterspeed_index);
        int iso_index = getISOIndex(best_params[i].iso);
        iso_best_index.push_back(iso_index);

    }

}