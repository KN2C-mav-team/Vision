#ifndef RECEIVE_H
#define RECEIVE_H

#include <QObject>
#include <QUdpSocket>
#include <opencv2/opencv.hpp>
#include <wifi_handler/WinSetMsg.h>
#include <wifi_handler/MarkerSetMsg.h>
#include <nodelet/nodelet.h>
#include "std_msgs/String.h"
#include <sstream>
#include <std_msgs/Int8.h>
#include <ros/ros.h>

using namespace cv;

class receive : public QObject
{
    Q_OBJECT
public:
    explicit receive(QObject *parent = 0);
    ~receive();

    bool get_camnum();
    ros::NodeHandle winset_nh;
    ros::Publisher winset_pub;
    ros::Publisher markerset_pub;
    ros::Publisher rc_pub;
    ros::Publisher image_pub;

    wifi_handler::WinSetMsg winset;
    wifi_handler::MarkerSetMsg markerset;

    QUdpSocket *rec_data_socket;
    int img_data_port;
    QByteArray datagram;
    static int img_handler;
    int swich;
    int rc;
    QByteArray start;
    std_msgs::Int8 rcc;
    std_msgs::Int8 imagend;
    int swapp;
    bool camnum;
    //int cam;
QString Ip;

signals:
    void status_receive();
    void window_set();
void ip_set();
public slots:
    void receive_status();
};

#endif // RECEIVE_H
