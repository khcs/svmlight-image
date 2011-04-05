//
//  svmlight_image.cpp
//  svmlight_image
//
//  Created by Hoo-Chang Shin on 31/03/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include "svmlight_image.h"
using namespace std;


int image_to_svmlight(char const *img_dirname, char const *svm_filename){
    
    char *valid_dirname_for_c = validify_dirname_for_c(img_dirname);
    
    unsigned long dirname_length = strlen(valid_dirname_for_c) + 9; // 9 is for "neg_label"
    char *dirname_neg = new char[dirname_length];
    char *dirname_pos = new char[dirname_length];
    strcpy(dirname_neg, valid_dirname_for_c);
    strcat(dirname_neg, "neg_label/");
    strcpy(dirname_pos, valid_dirname_for_c);
    strcat(dirname_pos, "pos_label/");
    
    struct dirent *ent;
    DIR *neg_dir = opendir(dirname_neg);
    DIR *pos_dir = opendir(dirname_pos);
    ofstream svm_file;
    svm_file.open(svm_filename);
    if (neg_dir != NULL && pos_dir != NULL){
        while((ent = readdir(neg_dir)) != NULL){
            char *img_filename = new char[dirname_length + strlen(ent->d_name)];
            strcpy(img_filename, dirname_neg);
            strcat(img_filename, ent->d_name);
            
            if (strstr(ent->d_name, ".bmp")!=NULL || 
                strstr(ent->d_name, ".jpg")!=NULL ||
                strstr(ent->d_name, ".jpeg")!=NULL ||
                strstr(ent->d_name, "png")!=NULL){
                IplImage* img = cvLoadImage(img_filename);
                CvScalar pix_val;
                svm_file << "-1 ";
                int feat_num = 0;
                for(int row=0; row<img->height; row++){
                    for(int col=0; col<img->width; col++){
                        pix_val = cvGet2D(img, row, col);
                        if (pix_val.val[0] == 0) feat_num++;
                        else{
                            svm_file << feat_num++ << ":" << pix_val.val[0] << " ";
                        }
                    }
                }
                svm_file << endl;
                cvReleaseImage(&img);
            }                        
            delete[] img_filename;
        }
        while((ent = readdir(pos_dir)) != NULL){
            char *img_filename = new char[dirname_length + strlen(ent->d_name)];
            strcpy(img_filename, dirname_pos);
            strcat(img_filename, ent->d_name);
            
            if (strstr(ent->d_name, ".bmp")!=NULL || 
                strstr(ent->d_name, ".jpg")!=NULL ||
                strstr(ent->d_name, ".jpeg")!=NULL ||
                strstr(ent->d_name, "png")!=NULL){
                IplImage* img = cvLoadImage(img_filename);
                CvScalar pix_val;
                svm_file << "+1 ";
                int feat_num = 0;
                for(int row=0; row<img->height; row++){
                    for(int col=0; col<img->width; col++){
                        pix_val = cvGet2D(img, row, col);
                        if (pix_val.val[0] == 0) feat_num++;
                        else{
                            svm_file << feat_num++ << ":" << pix_val.val[0] << " ";
                        }
                    }
                }
                svm_file << endl;
                cvReleaseImage(&img);
            }                        
            delete[] img_filename;
        }
    }else{
        cout << "could not open the directory for storing images" << endl;
        svm_file.close();
        return -1;
    }
    
    closedir(neg_dir);
    svm_file.close();
    return 0;
}

int svmlight_to_image(char const *filename){
    
    string line;
    ifstream svmfile (filename);
    if (svmfile.is_open()){
        
        system("mkdir -p images");
        system("mkdir -p images/pos_label");
        system("mkdir -p images/neg_label");
        
        string sub;
    
        // count number of lines, determine size of the images
        int num_lines = 0, num_features, num_max_features = 0;
        string num_features_str;
        while (svmfile.good()){
            getline (svmfile, line);
            istringstream iss(line);
            
            do{
                iss >> sub;
                num_features = get_feat_num_or_pix_val(sub, "feat_num");
                if (num_features > num_max_features) num_max_features = num_features;
            }while(iss);
            
            num_lines++;
        }
        svmfile.clear();
        svmfile.seekg(0, ios::beg);
        
        
        // find image size
        int image_size = 1;
        while (image_size*image_size < num_max_features) image_size++;
        //cout << "num_lines: " << num_lines << endl << "max_num_features: " << num_max_features << endl << "image_size: " << image_size << endl;
        
        
        // write the image
        int pix_val, feat_num, col, row, current_file_num=0;
        char *outFileName = new char[28+(int)log10(num_lines)+1]; // 28 is for "images/neg_label/image-XXXX.jpg, log10+1 is for XXXX
        char *digitized_filenum;
        IplImage* im = cvCreateImage(cvSize(image_size, image_size), IPL_DEPTH_8U, 1);
        while (svmfile.good()){            
            getline (svmfile, line);
            istringstream iss(line);
            
            strcpy(outFileName, "images/");
            do{
                iss >> sub;
                
                if (sub.compare("+1")==0){
                    strcat(outFileName, "pos_label/image-");
                }else if (sub.compare("-1")==0){
                    strcat(outFileName, "neg_label/image-");
                }else{
                    pix_val = get_feat_num_or_pix_val(sub, "pix_val");
                    feat_num = get_feat_num_or_pix_val(sub, "feat_num");
                    row = feat_num/image_size;
                    col = feat_num%image_size;
                    //cout << pix_val << " " << col << " " << row << " " << endl;
                    cvSet2D(im, row, col, cvScalar(pix_val));
                }                
            }while (iss);
            
            digitized_filenum = digitize_filenum(current_file_num++, (int)log10(num_lines)+1);
            strcat(outFileName, digitized_filenum);
            strcat(outFileName, ".jpg");
            delete[] digitized_filenum;
            //cout << outFileName << endl;
            //cout << system("pwd") << endl;
            
            if (strstr(outFileName, "image-") != NULL && !cvSaveImage(outFileName, im)){
                cout << "could not save image in svmlight_to_image" << endl;
                return -1;
            }
            cvSet(im, cvScalar(0,0,0));
        }
        cvReleaseImage(&im);
        delete[] outFileName;
        svmfile.close();
    }
    else{
        cout << "Unable to open file" << endl;
        return -1;
    }
 
    return 0;   // on successful writing images
}

int get_feat_num_or_pix_val(string str, string type){
    size_t pos;
    int val;
    string str_val;
    pos = str.find(":");
    if (type.compare("feat_num") == 0) str_val = str.substr(0, pos);
    else if( type.compare("pix_val") == 0) str_val = str.substr(pos+1);
    else{
        cout << "not recongnisable type given in get_featureNum_or_image_Val" << endl;
        exit(1);
    }
    stringstream ss(str_val);
    ss >> val;
    return val;
}



char* digitize_filenum(int filenum, int digitnum){
    char *filenum_str = new char[digitnum];
    
    int num_real_digits;
    if (filenum == 0) num_real_digits = 1; else num_real_digits = floor(log10(filenum)) + 1;
    int num_zeros_in_the_front = digitnum - num_real_digits;
    char *real_digits = new char[num_real_digits];
    for (int i=0; i<num_zeros_in_the_front; i++){
        strcat(filenum_str, "0");
    }
    sprintf(real_digits, "%d", filenum);
    strcat(filenum_str, real_digits);
    
    delete[] real_digits;
    
    return filenum_str;
}

char* validify_dirname_for_unix(char const *dirname){
    char* dirname_space_char_replaced = replace_space_in_filename(dirname);
    char* last_dir_char_checked = check_and_add_last_char_in_dirname(dirname_space_char_replaced);
    
    return last_dir_char_checked;
}

char* validify_dirname_for_c(char const *dirname){
    char* dirname_to_pass = new char[strlen(dirname)];
    strcpy(dirname_to_pass, dirname);
    char* last_dir_char_checked = check_and_add_last_char_in_dirname(dirname_to_pass);
    
    return last_dir_char_checked;
}

char* replace_space_in_filename(char const *filename){
    unsigned long num_space = 0;
    for (int i=0; i<strlen(filename); i++){
        if (filename[i] == ' ') num_space++;
    }
    unsigned long new_filelength = strlen(filename) + num_space;
    char *new_filename = new char[new_filelength];
    int j=0;
    for (int i=0; i<strlen(filename); i++){
        if (filename[i] == ' '){
            new_filename[j++] = 92; // ASCII '\'
            new_filename[j++] = 32; // ASCII ' '
        }else{
            new_filename[j++] = filename[i];
        }
    }
    
    return new_filename;
}

char* check_and_add_last_char_in_dirname(char *dirname){
    char *new_dirname;
    if (dirname[strlen(dirname)] != '/'){
        new_dirname = new char[strlen(dirname)+1];
        strcpy(new_dirname, dirname);
        new_dirname[strlen(new_dirname)] = '/';
        delete[] dirname;
    }else{        
        new_dirname = dirname;
    }
    return new_dirname;
}
