#include <algorithm> 
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include "std_msgs/String.h"
#include <QObject>
#include "include/get_rgb.h"
#include "include/get_depth.h"
#include <QThread>
#include <QApplication>



int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "image_converter");

    ros::NodeHandle nh_;
    image_transport::ImageTransport it_(nh_);
    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber depth_image_sub_;
    image_transport::Subscriber bgr_image_sub_;
    ros::Subscriber bgr_camInfo_sub_;
    ros::Subscriber depth_camInfo_sub_;
    ros::Subscriber kinect_resize_rgb;
    ros::Subscriber kinect_resize_depth;

    get_depth depth;
    get_rgb rgb;

    QThread th1,th2;
    depth.moveToThread(&th1);
    rgb.moveToThread(&th2);

    // Subscribe to input video feed for grab depth and publish output video feed
    depth_image_sub_ = it_.subscribe("/camera/depth/image_raw", 1,
                                     &get_depth::imageCb_depth, &depth);

    // Subscribe to input video feed for grab bgr and publish output video feed
    bgr_image_sub_ = it.subscribe("/camera/rgb/image_rect_color", 1,
                                   &get_rgb::imageCb_bgr, &rgb);

    bgr_camInfo_sub_ = nh_.subscribe("/camera/rgb/camera_info", 1,
                                   &get_rgb::camInfoCb_bgr, &rgb);

    depth_camInfo_sub_= nh_.subscribe("/camera/depth/camera_info", 1,
                                      &get_rgb::camInfoCb_depth, &rgb);

    kinect_resize_rgb = nh_.subscribe("/kinect_resize", 1,
                                      &get_rgb::resize_rgb_cb, &rgb);
	
    kinect_resize_depth = nh_.subscribe("/kinect_resize", 1,
                                      &get_depth::resize_depth_cb, &depth);

    th1.start();
    th2.start();

    ros::spin();
    return 0;
}

