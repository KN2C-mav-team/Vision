#ifndef HISTANDBACKPROJ
#define HISTANDBACKPROJ

#include <QObject>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <QString>
#include <QDebug>
#include <include/quad_board.h>
#include "pole_handler/poleMsg.h"


//#define Min_Area 4000
#define EROSION_ELEM    0
#define EROSRION_SIZE   1
#define DILATION_ELEM   0
#define DILATION_SIZE   3
#define show_hist
#define show_backproj
#define Stop 1
#define blue 9
#define red 0

using namespace cv;

class HistAndBackproj : public QObject
{
    Q_OBJECT
public:
    explicit HistAndBackproj(QObject *parent = 0);

    Mat sub;
    int hsize;
    float hranges[2];
    const float* phranges;
    Mat ERODE_elem, DILATE_elem;
    int erosion_elem = EROSION_ELEM;
    int erosion_size = EROSRION_SIZE;
    int dilation_elem = DILATION_ELEM;
    int dilation_size = DILATION_SIZE;
    int vmin = 10, vmax = 256, smin = 50;
    Mat hsv, hue, mask, hist, histimg , backproj ,backproj_blured ;
    bool allowed;
   std::vector<std::vector<cv::Point> > contours;
    std::vector<Vec4i> hierarchy;
    int minArea=20000;
    int morph_elem = 0;
    int morph_size = 0;
    int morph_operator = 0;
    int const max_operator = 4;
    int const max_elem = 2;
    int const max_kernel_size = 21;
    const char* window_name = "Morphology Transformations Demo";
    Scalar color;
    RNG rng;
    int white_count;
    Rect poly ;
   // Quad_Board *transmit;
    ros::NodeHandle poleSet_nh;
    ros::Publisher polePub;
    pole_handler::poleMsg msg;
    void erosion_dilation();
    void contours_handler(Mat BP_image);
    void Morphology_Operations();
public slots:
    void CalcHist(Mat raw_image);


};

#endif // HistAndBackproj