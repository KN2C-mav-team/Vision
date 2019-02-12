#ifndef GET_RGB_H
#define GET_RGB_H

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
#include <QThread>
#include <std_msgs/Int8.h>

using namespace cv;

class get_rgb : public QObject
{
    Q_OBJECT
public:
    explicit get_rgb(QObject *parent = 0);

    void imageCb_bgr(const sensor_msgs::ImageConstPtr& msg);
    void camInfoCb_bgr(const sensor_msgs::CameraInfoConstPtr &msg);
    void camInfoCb_depth(const sensor_msgs::CameraInfoConstPtr &msg);
    void resize_rgb_cb(const std_msgs::Int8ConstPtr& msg);

    Mat rgb_dwnsmpld;
    std::vector<uchar> rgb_enc_buff;
    std::vector<int> rgb_compression_params;
    QByteArray rgb_img_data;

    QUdpSocket *send_rgb_socket;
    QHostAddress *GS_IP;
    int GS_img_port;

    QUdpSocket *rec_img_socket;
    QByteArray datagram;    

    QByteArray distort;
    uint32_t bin_x;
    uint32_t bin_y;
    uint32_t roi_x_off;
    uint32_t roi_y_off;
    uint32_t roi_h;
    uint32_t roi_w;
    bool roi_do;
    uint32_t info_seq;
    uint32_t sec_time;
    uint32_t nsec_time;

    typedef struct
    {
        uint32_t seq;
        QByteArray frame_id;
        uint32_t height;
        uint32_t width;
        QByteArray encoding;
        uint8_t is_bigendian;
        uint32_t step;
    }myHeader;

    static myHeader get_header();

    uint32_t depth_seq;
bool resize_rgb_flag;


private:
    static myHeader myHead;

signals:

public slots:


};

#endif // GET_RGB_H

