#ifndef COLORDETECTION_H
#define COLORDETECTION_H
#include <qdebug.h>
#include <QObject>
#include <opencv2/opencv.hpp>
using namespace cv;
class ColorDetection : public QObject
{
    Q_OBJECT
public:
     enum Colors{
        RED,YELLOW,WHITE
    };
    explicit ColorDetection(QObject *parent = 0);
    Mat detectRed(Mat src,Scalar scalar1,Scalar scalar2,Scalar scalar3,Scalar scalar4);
    Mat detectColor(Mat src ,Scalar scalar1,Scalar scalar2);
    Mat backprojectionResult(Mat src ,int color);


private :
    Mat do_Hist_And_Backproj(Mat hue,int color);


signals:

public slots:
};

#endif // COLORDETECTION_H
