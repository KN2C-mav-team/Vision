#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QThread>
#include <QApplication>
#include <include/quad_board.h>
#include <include/spin.h>




int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "serial_handler");

    ros::NodeHandle nh_;
    ros::Subscriber sub;
    ros::Subscriber sub1;
    ros::Subscriber sub2;
    ros::Subscriber sub3;
    ros::Subscriber sub4;

    Quad_Board serial;

    sub= nh_.subscribe("window_output", 1, &Quad_Board::windowCB, &serial);
    sub1= nh_.subscribe("Marker_output", 1, &Quad_Board::marker, &serial);
    sub2= nh_.subscribe("depth_output", 1, &Quad_Board::depthCB, &serial);
    sub3= nh_.subscribe("optical_output", 1, &Quad_Board::optical, &serial);
    sub4= nh_.subscribe("rc_output", 1, &Quad_Board::rc, &serial);

    Spin spinner;
    spinner.start();


//ros::spin();
    return a.exec();
    //return 0;

}

