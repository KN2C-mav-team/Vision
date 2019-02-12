#ifndef LINEFOLLOW
#define LINEFOLLOW

#include <QObject>
#include <iostream>
#include <std_msgs/String.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <QString>
#include <QDebug>
#include <include/quad_board.h>
#include <LineFollow/lineMsg.h>
#include "string"
#include "QTime"

///
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
#define PI 3.14159265359

#define up 0
#define down 1
#define Right 2
#define left 3
#define right_up 4
#define right_down 5
#define left_up 6
#define left_down 7
#define R 70

using namespace cv;

class Linefollow : public QObject
{
    Q_OBJECT
public:
    explicit Linefollow(QObject *parent = 0);


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
    Mat hsv, hue, mask, hist, histimg , backproj ,backproj_blured,dst,dst_norm,dst_norm_scaled ;
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int thresh = 200;
    bool allowed,begin=false;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<Vec4i> hierarchy;
    int minArea=219;
    int morph_elem = 0;
    int morph_size = 0;
    int morph_operator = 0;
    int const max_operator = 4;
    int const max_elem = 2;
    int const max_kernel_size = 21;
    const char* window_name = "back";
    Scalar color;
    RNG rng;
    LineFollow::lineMsg msg;
    int white_count;
    float prev_distance;
    Rect poly ;
    Point p1,p2,p3,prev_mc;
    RotatedRect rotate_poly;
    // Quad_Board *transmit;
    ros::NodeHandle lineSet_nh;
    ros::Publisher linePub;
    //pole_handler::poleMsg msg;
    void erosion_dilation();
    void contours_handler(Mat BP_image);
    void Morphology_Operations();
    float dist(Point A,Point B);
    void bubbleSort_point_y(Point arr[], int n);
    void bubbleSort_point_x(Point arr[], int n);
    void bubbleSort(float arr[], int n);
    // void swap(Point *xp, Point *yp);
    Point verticies[4];
    QTime d;
    String color_string;
    int angle=0;
    double d1,d2;
    bool forward_right=false,forward_left=false;
    void zbarCB(const std_msgs::String &msg );
    bool firstBegin;

public slots:
    void CalcHist(Mat raw_image);


};

#endif // LineFollow
