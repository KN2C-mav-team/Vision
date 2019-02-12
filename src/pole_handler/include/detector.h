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

#define EROSION_ELEM    0
#define EROSRION_SIZE   1
#define DILATION_ELEM   0
#define DILATION_SIZE   3
#define _BORDER             10
#define _MIN_CONTOUR_SIZE   500
#define _WHTIEPIX_THR       0.5
#define _APPROXCURE_EPSILON 20

using namespace cv;

class HistAndBackproj : public QObject
{
    Q_OBJECT
public:
    explicit HistAndBackproj(QObject *parent = 0);
    Mat frame;
    int k=0;

signals:
    void imageReady(Mat);

public slots:
    void imageCallback(const sensor_msgs::ImageConstPtr& msg);


};

#endif // HistAndBackproj
