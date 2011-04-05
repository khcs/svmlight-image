//
//  svmlight_image.h
//  svmlight_image
//
//  Created by Hoo-Chang Shin on 31/03/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <dirent.h>

#include "highgui.h"

int image_to_svmlight(char const *img_dirname, char const *filename);
int svmlight_to_image(char const *filename);
int get_feat_num_or_pix_val(std::string str, std::string type);

char* digitize_filenum(int filenum, int digitnum);

char* validify_dirname_for_c(char const *dirname);
char* validify_dirname_for_unix(char const *dirname);
char* replace_space_in_filename(char const *filename);
char* check_and_add_last_char_in_dirname(char *dirname);
