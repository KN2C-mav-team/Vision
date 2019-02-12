#include <iostream>
#include <ros/ros.h>
#include <include/capture.h>
#include <include/Linefollow.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <QApplication>
#include <QThread>



int main( int argc, char* argv[] )
{
    QApplication a(argc,argv);

    ros::init( argc, argv, "LineFollow" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(n);
    image_transport::Subscriber bgr_image_sub_;
    capture cap;

    Linefollow Linefollower;

    QThread thr1,thr2,thr3;
    cap.moveToThread(&thr1);
    Linefollower.moveToThread(&thr2);
    thr1.start();
    thr2.start();
    ros::Subscriber zbarToline = n.subscribe("zbar_data", 1,&Linefollow::zbarCB,&Linefollower);
    QObject::connect(&cap,SIGNAL(imageReady(Mat)),&Linefollower,SLOT(CalcHist(Mat)),Qt::ConnectionType::DirectConnection);
    bgr_image_sub_ = it.subscribe
            ("/image_raw",1,&capture::imageCallback,&cap);




    ros::spin();
    return 0;
}
