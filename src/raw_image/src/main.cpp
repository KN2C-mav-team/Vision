#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QApplication>
#include "include/sendimage.h"
#include "include/spin.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "raw_image");

    ros::NodeHandle nh_;
    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber image_sub;
ros::Subscriber prog_end;
    SendImage send;


    // Subscribe to input video feed for grab rgb and publish output video feed
    image_sub = it.subscribe("/front_cam/image_raw", 1,
                             &SendImage::raw_image, &send);
prog_end=nh_.subscribe("image_end",1,&SendImage::prog_end,&send);
    Spin spinner;
    spinner.start();

    qDebug() << "main";
    // ros::spin();
    return a.exec();
}

