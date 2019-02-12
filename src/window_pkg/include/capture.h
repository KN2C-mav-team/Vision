#ifndef Capture_H
#define Capture_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <algorithm>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sstream>
#include <stdint.h>
#include <qt4/Qt/qdebug.h>


using namespace cv;
using namespace std;

class Capture : public QObject
{
    Q_OBJECT
public:
    explicit Capture(QObject *parent = 0);
    ~Capture();

    void imageCb_bgr(const sensor_msgs::ImageConstPtr& msg);

    double exec_time;

    static Mat frame;
    bool imshow_mode;
    int time_sleep,numCam;
    QMutex mutex;

private:


signals:
    void imageReadySig(Mat);

public slots:


};

#endif // Capture_H
