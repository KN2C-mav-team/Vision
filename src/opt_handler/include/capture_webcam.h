#ifndef CAPTURE_WEBCAM_H
#define CAPTURE_WEBCAM_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>

#define _DEV_VIDEO              0
#define _VIDEO_FPS              90

#define _FRAME_WIDTH            160
#define _FRAME_HEIGTH           120

using namespace cv;
using namespace std;

class capture_webcam : public QObject
{
    Q_OBJECT
public:
    explicit capture_webcam(QObject *parent = 0);
    ~capture_webcam();

    Mat frame;

    int get_frame_status();
    double get_Diff_Time();
    void set_frame_status(int status);

private:


    VideoCapture cap;
    double scale;

    QTimer *main_timer;

    bool farme_status;
    long int start_time;
    double Diff_Time;

signals:
    void imageReady(Mat);

public slots:
    void imageEvent(const sensor_msgs::ImageConstPtr& msg);
};

#endif // CAPTURE_WEBCAM_H
