# Introduction #

Currently it converts image format ('jpg', 'bmp', 'png') to the [SVM^light](http://svmlight.joachims.org) format, and vice-versa for Support Vector Machine (SVM) machine learning application in images.

# Dependencies #

[OpenCV](http://opencv.willowgarage.com/wiki)

# Installation #

$ make

# Usage #

**Images to SVM^light format**

```
./svmlight_image [path_to_the_images] [svmlight_filename_to_be_created]
```

 in the 'path\_to\_the\_images', there should be 'neg\_label' and 'pos\_label' directories for 'images\_with\_negative\_label' and 'images\_with\_positive\_label' each.

**SVM^light format to images**

```
./svmlight_image [svm_light_filename_including_whole_path] 
```

 then it creates in the 'images' directory with 'neg\_label' and 'pos\_label' directories for 'images\_with\_negative\_label' and 'images\_with\_positive\_label' each in the current path.