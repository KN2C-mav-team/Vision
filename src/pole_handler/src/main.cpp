#include <iostream>
#include <ros/ros.h>
#include <include/capture.h>
#include <include/HistAndBackproj.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <QApplication>
#include <QThread>


int main( int argc, char* argv[] )
{
    QApplication a(argc,argv);

    ros::init( argc, argv, "pole_handler" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(n);
    image_transport::Subscriber bgr_image_sub_;
    capture cap;
    HistAndBackproj Hist_handler;
    QThread thr1,thr2,thr3;
    cap.moveToThread(&thr1);
    Hist_handler.moveToThread(&thr2);
    thr1.start();
    thr2.start();

    QObject::connect(&cap,SIGNAL(imageReady(Mat)),&Hist_handler,SLOT(CalcHist(Mat)),Qt::ConnectionType::DirectConnection);
    bgr_image_sub_ = it.subscribe
            ("/usb_cam/image_raw",1,&capture::imageCallback,&cap);

    ros::spin();
    return 0;
}
