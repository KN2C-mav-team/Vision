#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <capture_webcam.h>
#include <optflow.h>
#include <QThread>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    ros::init( argc, argv, "opt_handler" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(n);
    image_transport::Subscriber bgr_image_sub_;
    capture_webcam cap;
    OptFlow w;
    QThread mtr1,mtr2;
    w.moveToThread(&mtr1);
    cap.moveToThread(&mtr2);
    mtr1.start(QThread::HighestPriority);
    mtr2.start();
    bgr_image_sub_ = it.subscribe
            ("/usb_cam_below/image_raw_below",1,&capture_webcam::imageEvent,&cap);
    QObject::connect(&cap,SIGNAL(imageReady(Mat)),&w,SLOT(timerEvent(Mat)),Qt::ConnectionType::DirectConnection);
    ROS_WARN("Opt main runs");
    ros::spin();
    return 0;
}
