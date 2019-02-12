#ifndef DETECTION_H
#define DETECTION_H

#include <QObject>
#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <QDebug>
#include <marker_node/MarkerDetMsg.h>
#include <marker_node/MarkerSetMsg.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int8.h>
#include <image_transport/image_transport.h>
#include "std_msgs/String.h"
#include <sstream>
#include <nodelet/nodelet.h>


#define approxpoly_      5
#define sizemin_         3
#define sizemax_         7

#define sizemin_ch       4
#define sizemax_ch       8

//#define accuracy_        8

#define center_    30
//#define show_
#define draw_
#define time_
#define checking_
#define biggest_
using namespace std;
using namespace cv;
class Detection : public QObject
{
    Q_OBJECT
public:
    explicit Detection(QObject *parent = 0);
    ~Detection();

    void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    void img_set(const marker_node::MarkerSetMsgConstPtr& msg1);

    marker_node::MarkerDetMsg output;
    ros::Publisher output_pub;
    ros::Publisher image_pub;
    ros::Publisher image_binary_pub;
    ros::NodeHandle marker_nh;
    sensor_msgs::ImagePtr imageMsg;

    int imgenable;
    static Mat frame1;
    static int px;
    static int py;
    Mat gray;
    Mat binary;
    Mat binary1;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point>  shape1;
    vector<Point>  shape2;
    vector<Point>  shape3;
    Rect rect;
    RotatedRect recttt;
    int tlx;
    int tly;
    int x;
    int yy;
    double t;
    Point ox;
    Point oy;
    Point result;
    int counter;
    Mat kernal;
    Point2f rect_points[4];
    Point2f rectpoint[4];
    Point2f rectpoint1[4];
    float d;
    float a;
    float h;
    Point p2;
    Mat M, rotated, cropped;
    RotatedRect r2;
    RotatedRect r3;
    int x3, y3, x4, y4 ,x5 ,y5;
RotatedRect rrect1;
RotatedRect rrect2;
float px2;
float py2;

    Mat kernall;
    int counter1;
    int flag;
    int flag1;
    Point oz,oz1;
    int area;
    int a1,a2;
    int coun;
    int check;
    int r1;
    int r0;
    int aa;
    Point swapp;
    int dilationkernel_;
    int erosinkernel_;
    int deltax1;
    int deltax2;
    int deltay1;
    int deltay2;
int accuracy_;

   // QUdpSocket *run_socket;
   // int run_port  ;
   // QHostAddress *host_ad;
   // QByteArray datagram;
private:
    bool invers;
signals:
    void img_ready();
public slots:
    void main_function();
};

#endif // DETECTION_H
