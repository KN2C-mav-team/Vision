#ifndef HDETECTION_H
#define HDETECTION_H
#include <opencv2/opencv.hpp>
#include <QObject>
#include <qdebug.h>
using namespace cv;
class HDetection : public QObject
{
    Q_OBJECT
public:

    struct myclassY {
        bool operator() (Point pt1, Point pt2) { return (pt1.y < pt2.y);}
    } myobject1;


    explicit HDetection(QObject *parent = nullptr);
private :
    void detectH();
    void static on_trackbar( int, void* );
    double getSlope(Point p1,Point p2);

signals:
    void landingPadCallBack(int land);
public slots:
    void hLandingCallBack(cv::Mat img);

};

#endif // HDETECTION_H
