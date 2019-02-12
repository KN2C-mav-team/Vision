#include <iostream>
#include <ros/ros.h>
#include <include/capture.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <QApplication>
#include <QThread>
#include "zbar_detect.h"


int main( int argc, char* argv[] )
{
    QApplication a(argc,argv);

    ros::init( argc, argv, "zbar_detector" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(n);
    image_transport::Subscriber bgr_image_sub_;
    capture cap;
    zbar_detect zbar_detector;
    QThread thr1,thr2;
    cap.moveToThread(&thr1);
    zbar_detector.moveToThread(&thr2);
    thr1.start();
    thr2.start();

    QObject::connect(&cap,SIGNAL(imageReady(Mat)),&zbar_detector,SLOT(detector(Mat)),Qt::ConnectionType::DirectConnection);
    bgr_image_sub_ = it.subscribe
            ("/usb_cam_below/image_raw_below",1,&capture::imageCallback,&cap);

    ros::spin();
    return 0;
}