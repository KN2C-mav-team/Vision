#ifndef HANDLER_H
#define HANDLER_H
#include <qthread.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "colordetection.h"
#include "udetection.h"
#include <QObject>
#include <stdio.h>
#include <iostream>
using namespace cv;
using namespace std;

class Handler : public QObject
{
    Q_OBJECT
public:
    explicit Handler(QObject *parent = 0);



private:
    Mat raw_frame;
    Mat yellow,red;
    Mat reduced_noise;
    int up_down; //if 0 down
    ColorDetection *detector;
    UDetection *uDetector;
    //QTimer *timer;
    void makeTrackbar();
    ColorDetection::Colors compareContoursColor(vector<Point> red_c,vector<Point> yellow_c);
    ColorDetection::Colors findFirstGate(Mat frame);
    ColorDetection::Colors chooseColor(Rect ROI,int y);
    static void on_trackbar( int value, void* );
    static void Erosion( int, void* );
    static void Dilation( int, void* );

    static void wait ( int seconds ) ;


signals:

    void _SIGNAL_detectYellowRedFrame();
    void _SIGNAL_reduceNoise();
    Mat _SIGNAL_Hist_And_Backproj(Mat src);
    void gateCallBack(int movement);
    void gateStatus(int movement);
public slots:
    void imageCallback(cv::Mat frame);
    void detectYellowRedFrame();
    void reduceNoise();
};


#endif // HANDLER_H
