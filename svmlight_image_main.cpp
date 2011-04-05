//
//  main.cpp
//  svmlight_image
//
//  Created by Hoo-Chang Shin on 31/03/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "svmlight_image.h"
using namespace std;

int main (int argc, char const *argv[]){

    // Usage (1)
    // ./svmlight_image [path_to_the_images] [svmlight_filename_to_be_created]
    // in the [path_to_the_images], there should be [neg_label] and [pos_label] directories
    // for [images_with_negative_label] and [images_with_positive_label] each.
    if (argc == 3){
        image_to_svmlight(argv[1], argv[2]);
    }
    // Usage (2)
    // ./svmlight_image [svm_light_filename_including_whole_path]
    // then it creates in the [images] directory with [neg_label] and [pos_label] directories
    // for [images_with_negative_label] and [images_with_positive_label] each
    // in the current path.
    else if (argc == 2){
        svmlight_to_image(argv[1]);
    }else{
        cout << "svmlight_image" << endl;
        cout << "Ken HC Shin" << endl;
        cout << "ken.hc.shin@gmail.com" << endl << endl;
        
        cout << "Usage (1)" << endl;
        cout << "./svmlight_image [path_to_the_images] [svmlight_filename_to_be_created]" << endl;
        cout << "in the [path_to_the_images], there should be [neg_label] and [pos_label] directories" << endl;
        cout << "for [images_with_negative_label] and [images_with_positive_label] each." << endl;
        
        cout << "Usage (2)" << endl;
        cout << "./svmlight_image [svm_light_filename_including_whole_path]" << endl;
        cout << "then it creates in the [images] directory with [neg_label] and [pos_label] directories" << endl;
        cout << "for [images_with_negative_label] and [images_with_positive_label] each" << endl;
        cout << "in the current path." << endl;
    }
}
