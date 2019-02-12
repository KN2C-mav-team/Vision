#ifndef ZBAR_DETECT
#define ZBAR_DETECT

#include <QObject>
#include <iostream>
#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QDebug>
#include "zbar.h"
#include "std_msgs/String.h"

using namespace std;
using namespace zbar;
using namespace cv;

typedef struct
{
    std::string type;
    string data;
    vector <Point> location;
} decodedObject;


class zbar_detect : public QObject
{
    Q_OBJECT
public:
    explicit zbar_detect(QObject *parent = 0);
    ros::NodeHandle zbar_nh;
    ros::Publisher zbarPub;
    Mat src;
    std_msgs::String msg;

void decode(Mat &im, std::vector<decodedObject>&decodedObjects);
void display(Mat &im, vector<decodedObject>&decodedObjects);

signals:
    void imageReady(Mat);

public slots:
    void detector(Mat img);


};

#endif // ZBAR_DETECT
