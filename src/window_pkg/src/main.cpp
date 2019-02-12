#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QThread>
#include <QApplication>
#include "detectwindow.h"
#include <capture.h>





int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "window_detection");

    ros::NodeHandle nh_;
    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber rgb_image_sub_;

    QThread tr1;
    QThread tr2;

    qDebug() << "main: " << QThread::currentThreadId();

    DetectWindow win;
    Capture cap_;
    win.moveToThread(&tr1);
    cap_.moveToThread(&tr2);
    tr1.start();
    tr2.start();


    QObject::connect(&cap_, SIGNAL(imageReadySig(Mat)),&win, SLOT(getImage(Mat)),Qt::ConnectionType::DirectConnection);
    rgb_image_sub_ = it.subscribe("/usb_cam/image_raw", 1,
                                  &Capture::imageCb_bgr, &cap_);
    ros::spin();

    return 0;
}

