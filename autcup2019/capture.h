#ifndef CAPTURE_H
#define CAPTURE_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <includes.h>


#define FRONT_CAM_NUM 1
#define DOWN_CAM_NUM 0

#define FRONT_FPS 30
#define DOWN_FPS 60

#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240


using namespace cv;

class capture : public QObject  
{
    Q_OBJECT  
public:
    explicit capture(QObject *parent = 0);

    VideoCapture front_cam,down_cam;//,test_cam;
    VideoWriter front_video;
    VideoWriter down_video;
    Mat front_frame;
    //Mat test_frame;
    Mat down_frame;
    Mat down_frame_line;
    Mat down_frame_qr;



signals:

    void front_image(cv::Mat );
    void down_image_land(cv::Mat );
    void down_image(cv::Mat );
    void down_image_line(cv::Mat );
    void down_image_qr(cv::Mat );

public slots:
    void frontFrame_ready();
    void downFrame_ready();

};

#endif // CAPTURE_H

