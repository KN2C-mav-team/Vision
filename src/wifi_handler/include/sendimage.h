#ifndef SENDIMAGE_H
#define SENDIMAGE_H

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>
#include <QImageWriter>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int8.h>
#include <wifi_handler/WinDetMsg.h>
#include <wifi_handler/MarkerDetMsg.h>     
#include "include/receive.h"
using namespace cv;

class SendImage : public QObject
{
    Q_OBJECT
public:
    explicit SendImage(QObject *parent = 0);

    void imageCB(const sensor_msgs::ImageConstPtr& msg);
    void marker_image(const sensor_msgs::ImageConstPtr& msg1);
    void front_image(const sensor_msgs::ImageConstPtr& msg);
    void below_image(const sensor_msgs::ImageConstPtr& msg);
    void window_s(const wifi_handler::WinDetMsgConstPtr& msg2);
    void marker_s(const wifi_handler::MarkerDetMsgConstPtr& msg3);
    void opt_s(const std_msgs::Int8ConstPtr& msg4);
void cam_s(const std_msgs::Int8ConstPtr& msg4);
    QUdpSocket *send_img_socket;
    QHostAddress *GS_IP;
    int GS_img_port;
    std::vector<uchar> enc_buff;
    std::vector<int> compression_params;
    QByteArray img_data;
    //  QMutex mutex;

    QUdpSocket *send_status_socket;
    QHostAddress *GS_IP_status;
    int GS_status_port;
    std::vector<uchar> enc_buff_status;
    std::vector<int> compression_params_status;
    QByteArray status_data;
    //QMutex mutex;
    int imghandler;
    receive *rece;
    int cam;

private:
    Mat myframe;
    Mat myframe1;
    Mat myframe2;
    int win;
    int marker;
    int opt;
    QTimer *main_timer;
  // QTimer *spin_timer;
    int img_handle;

signals:
    void imageReadySig();
    void imageReadySig_marker();
     void imageReadySig_raw();


public slots:
    void send_image();
    void send_image_marker();
    void send_image_raw();
    void timerEvent();
    void imge_handler();
 void Ip_set();
    //void spin_handler();
};

#endif // SENDIMAGE_H
