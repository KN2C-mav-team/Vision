#include "Headers/WhiteDetector.h"


WhiteDetector::WhiteDetector(){
    namedWindow(WINDOW,WINDOW_AUTOSIZE);
}

WhiteDetector::~WhiteDetector(){
    destroyAllWindows();
}

void WhiteDetector::detect(Mat& raw_image){
    imshow(WINDOW,raw_image);
    waitKey(3);
}
