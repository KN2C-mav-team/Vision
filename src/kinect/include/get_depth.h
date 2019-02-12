#ifndef GET_DEPTH_H
#define GET_DEPTH_H

#include <QObject>
#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include "std_msgs/String.h"
#include <sstream>
#include <qt4/Qt/qtimer.h>
#include <qt4/Qt/qimage.h>
#include <qt4/Qt/qbuffer.h>
#include <qt4/Qt/qimagewriter.h>
#include <stdint.h>
#include <qt4/Qt/qdatastream.h>
#include <qt4/Qt/qiodevice.h>
#include <qt4/Qt/qdebug.h>
#include <qt4/Qt/qbytearray.h>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QThread>

#include <tf2_msgs/TFMessage.h>
//#include <std_msgs/Header.h>
#include <geometry_msgs/Transform.h>
#include <std_msgs/Int8.h>

using namespace cv;

class get_depth : public QObject
{
    Q_OBJECT
public:
    explicit get_depth(QObject *parent = 0);

    void imageCb_depth(const sensor_msgs::ImageConstPtr& msg);
    void tfCb(const tf2_msgs::TFMessageConstPtr &msg);
    void resize_depth_cb(const std_msgs::Int8ConstPtr& msg);

    Mat d_dwnsmpld;
    std::vector<uchar> d_enc_buff;
    std::vector<int> d_compression_params;
    QByteArray d_img_data;

    QUdpSocket *send_left_socket;
    QUdpSocket *send_right_socket;
    QUdpSocket *send_notOver_socket;
    QTcpSocket *send_tcp;
    QHostAddress *GS_IP;
    int GS_img_port;
    int GS_img_port_overload;
    int GS_img_port_overload2;

    QUdpSocket *rec_img_socket;
    QByteArray datagram;
    bool resize_depth_flag;



public slots:


};

#endif // GET_DEPTH_H
