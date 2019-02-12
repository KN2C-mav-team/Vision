#ifndef SENDIMAGE_H
#define SENDIMAGE_H

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>
#include <QImageWriter>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int8.h>

using namespace cv;

class SendImage : public QObject
{
    Q_OBJECT
public:
    explicit SendImage(QObject *parent = 0);

    void raw_image(const sensor_msgs::ImageConstPtr& msg);
void prog_end(const std_msgs::Int8ConstPtr& msg);
    QUdpSocket *send_img_socket;
    QHostAddress *GS_IP;
    int GS_img_port;
    std::vector<uchar> enc_buff;
    std::vector<int> compression_params;
    QByteArray img_data;
  // QTimer *spin_timer;



private:
    Mat myframe;


signals:
    void imageReadySig();

public slots:
    void send_image();
  // void spin_handler();

};

#endif // SENDIMAGE_H
