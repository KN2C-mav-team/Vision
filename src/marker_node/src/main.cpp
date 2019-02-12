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
#include "include/detection.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    ros::init(argc, argv, "marker_node");
    ros::NodeHandle n;
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub;
    Detection *detect;
    detect=new Detection();
    sub= it.subscribe("/below_cam/image_raw", 1, &Detection::imageCallback,detect);
    
    //******************************
    ros::Subscriber img_state;
    img_state= n.subscribe("markerset_output", 1,&Detection::img_set,detect);

    ros::spin();
    return 0;


}

