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
#include "include/sendimage.h"
#include "include/receive.h"
#include "include/spin.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "wifi_handler");

    ros::NodeHandle nh_;
    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber image_sub;

    image_transport::ImageTransport it1(nh_);
    image_transport::Subscriber marker_sub;

    image_transport::ImageTransport it2(nh_);
    image_transport::Subscriber front_img;

    image_transport::ImageTransport it3(nh_);
    image_transport::Subscriber below_img;

    SendImage send;
    //receive recc;
    ros::Subscriber win_state;
    ros::Subscriber marker_state;
ros::Subscriber opt_state;
ros::Subscriber cam_state;

    // Subscribe to input video feed for grab rgb and publish output video feed
    image_sub = it.subscribe("window_image", 1,
                             &SendImage::imageCB, &send);
    marker_sub = it1.subscribe("Marker_image", 1,
                               &SendImage::marker_image, &send);
    front_img = it2.subscribe("/front_cam/image_raw", 1,
                               &SendImage::front_image, &send);
    below_img = it3.subscribe("/below_cam/image_raw", 1,
                               &SendImage::below_image, &send);

    win_state= nh_.subscribe("window_output", 1, &SendImage::window_s, &send);
    marker_state= nh_.subscribe("Marker_output", 1, &SendImage::marker_s, &send);
    opt_state=nh_.subscribe("opt_status_output",1,&SendImage::opt_s,&send);
cam_state=nh_.subscribe("camnum",1,&SendImage::cam_s,&send);

    Spin spinner;
    spinner.start();

    qDebug() << "main";
    // ros::spin();
    return a.exec();
}

