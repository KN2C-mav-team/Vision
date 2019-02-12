#ifndef DETECTWINDOW_H
#define DETECTWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <opencv/cv.h>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QDate>
#include <QTime>
#include <iostream>
#include <ctype.h>
#include "capture.h"
#include "ros/ros.h"
#include <std_msgs/Int8.h>
#include "opencv2/opencv.hpp"
#include <window_pkg/WinMsg.h>
#include <sstream>

using namespace cv;
using namespace std;

#define _CAM_NUM            0
#define _FRAME_WIDTH        640
#define _FRAME_HEIGTH       480
#define _VIDEO_FPS          30
#define _THREAD_SLEEPTIME   2 ////////////////////////////ino 2 bezar
#define _BORDER             10
#define _MIN_CONTOUR_SIZE   500
#define _WHTIEPIX_THR       0.5
#define _APPROXCURE_EPSILON 20
#define _PIXEL_ANGLE_THR    25
#define _FIRST_LEVEL_SCALE  55
#define _FIRST_LEVEL_DILATE 3

#define EROSION_ELEM    0
#define EROSRION_SIZE   0
#define DILATION_ELEM   0
#define DILATION_SIZE   0

//#define _SHOW_HIST
#define _ERODE_DILATE_ON
//#define _SPLIT_ON
//#define _SMOOTH_ON
#define _INIT
#define _SERIAL_ENABLED
#define _WIFI_ENABLED
#define _ELIMINATE_EXTERNAL
//#define _PRINT_LOG

#define _LAST_POINTS_NUM 2
//#define  _DRAW_TWO_WINDOWS



class DetectWindow : public QObject
{
    Q_OBJECT

public:
    DetectWindow(QObject *parent = 0);
    ~DetectWindow();

    void bubbleSort(int arr[], int n);
    void bubbleSort_point(Point arr[], int n);
    void Erosion();
    void Dilation();

    ros::NodeHandle status_nh;
    std_msgs::Int8 status_msg;

    Capture *cap;

    Mat image,edges,image_bordered;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //vector<Point> approxCurve[100];
    int contour_cols_size[100];
    int window_num[100];
    int three_window_num_border[100];
    Point window_center;
    Point three_windows_center[3];
    Point three_windows_center_sorted[3];
    int left_border, right_border, middle;
    int three_windows_x[3];
    window_pkg::WinMsg msg;
    ros::Publisher win_pub;
    ros::NodeHandle win_nh;
    bool backprojMode;
    bool selectObject;
    int trackObject;
    bool showHist;
    Point origin;
    Rect selection;
    int vmin, vmax, smin;
    QDate CurrDate;
    QTime CurrTime;
    VideoWriter CamOut;
    QString ImgPath,ImgPath2,CurrDateText,CurrTimeText,path,path_img;

    int hsize;
    float hranges[2];
    const float* phranges;

    Mat hsv, hue, mask, hist, histimg , backproj ,backproj_blured ;
    bool paused;

    bool show;
    int sub_mode;
    Rect roi;
    static Mat sub;
    
    Mat erosion_dst, dilation_dst, ERODE_elem, DILATE_elem;
    Mat detectFrame;

    int erosion_elem;
    int erosion_size;
    int dilation_elem;
    int dilation_size;

    int side1_y,side2_y,side1_x,side2_x,scale,scale1,scale2;
    int hit_border;
    int inside_polygons;
    //int const max_elem;
    //int const max_kernel_size;
//recent
 

   Point two_windows_center[3];
    Point two_windows_center_sorted[3];
     int mean_window_center;
    int window_center_y;
    int last_points_detected[3];
    int last_point_counter;
    int last_scale;
bool one_edge_mode;
bool edge_out_mode;
int window_index;

    int suggested_point_x;


public slots:
    void getImage(Mat raw_image);
    void dontshow();
    void doshow();

signals:
    void dataReady();

};

#endif // DETECTWINDOW_H
