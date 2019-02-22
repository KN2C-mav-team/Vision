#ifndef UDETECTION_H
#define UDETECTION_H
#include <opencv2/opencv.hpp>
#include "colordetection.h"
#include <qdebug.h>
#include <math.h>
#include <qthread.h>
using namespace cv;
using namespace std;

class UDetection :public QObject
{

public:
    UDetection(QObject *parent=0);
    bool detectU_shapes(Mat src);
    static bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);
    vector<Point> getYellow_contour() const;
    vector<Point> getRed_contour() const;

    void setColor_tag(const ColorDetection::Colors &value);

    void setCroped(bool value);

    void setUp_down(int value);


private:

    ColorDetection::Colors color_tag;

    bool croped;
    int up_down; //if 0 down
    vector<Point> red_contour;
    vector<Point> yellow_contour;
    static bool is_90Deg(Vec4i l1 ,Vec4i l2);

    static double angle( Point pt1, Point pt2  );

     vector<Vec4i> detectLines(Mat src);
     static bool isSlopeEqual(Vec4i l1, Vec4i l2);
     static bool isSlopeEqual(Point start_pt1,Point end_pt1, Point start_pt2,
                              Point end_pt2 , Point start_pt3,Point end_pt3);
     static bool isHorizental(Vec4i l);
     static bool isVertical(Vec4i l);
     bool detectBoundedRects(Mat src);
     bool detectEdges(Mat src);
     int  calculateLength(Vec4i line);
     bool isU(vector<Point> points);
     bool detecGate(Mat src);
     bool detectUBySecondForm(vector<Point> points);
     bool isInsideEmpty(Mat src);
     int  findMaxLength(vector<Vec4i> lines);
     bool findMinLength(vector<Point> points);
     bool isClose4Vertex(vector<Vec4i> lines);//is rctangle
     Point findCenterPoint(vector<Point> points);
     bool hasIntersectionPoints(Point p1,Point p2);
     bool hasIntersectionWithOthers(Point p,vector<Vec4i> lines,int index1,int index2);

};

#endif // UDETECTION_H
