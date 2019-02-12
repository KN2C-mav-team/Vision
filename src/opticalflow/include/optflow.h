#ifndef OPTFLOW_H
#define OPTFLOW_H

#include "QThread"
#include "opencv2/opencv.hpp"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <std_msgs/Int8.h>
#include <opticalflow/OpticalMsg.h>
#include <QDebug>
//#define _GRAPH//



#define MAX_COUNT               20
#define THRESH_REFRESH          8

#define winSize                 Size(15,15)//Size(31,31)
#define subPixWinSize           Size(5,5)//Size(10,10)



using namespace cv;
using namespace std;


class OptFlow: public QObject
{
    Q_OBJECT

public:
    explicit OptFlow(QObject *parent = 0);
    ~OptFlow();


    void imageCb_bgr(const sensor_msgs::ImageConstPtr& msg);

    ros::Publisher status_pub;
    ros::NodeHandle opt_nh;
    std_msgs::Int8 status;
    Mat colorframe, grayframe;
    ros::Publisher output_pub;
    opticalflow::OpticalMsg output;


    Mat prevGray, image;
    vector<Point2f> points[2];
    TermCriteria termcrit;
    Point2f point;


    bool needToInit;
    bool done;

    int Features_counter;//count Features state true

    double delta_x, delta_y;

    void OptFlow_LK();

private:
    //Quad_Board *transmiter;

public slots:
    void getImage();

signals:
    void imageReadySig();
};

#endif // OPTFLOW_H
