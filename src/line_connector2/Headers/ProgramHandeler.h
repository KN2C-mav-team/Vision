#ifndef PHNDL_H
#define PHNDL_H

#include "Headers/includes.h"
#include "Headers/LineEquations.h"
//offset : 500
#define MINIMUM_RECTANGLE_AREA 200
#define MAXIMUM_RECTANGLE_AREA 1800

using namespace cv;
using namespace std;

static const string WINDOW = "Image window";

class ProgramHandeler{
public:

    ProgramHandeler();
    ~ProgramHandeler();
    void imageCallBack(const sensor_msgs::ImageConstPtr&);
    void Callback(const line_connector2::qr_msg::ConstPtr& );

    ros::NodeHandle nh_;
    ros::Subscriber subscriber;
    image_transport::Subscriber image_sub;
    image_transport::ImageTransport it_;
    cv_bridge::CvImagePtr cv_ptr;

private:

    void whiteThsOut(Mat &);
    void blackThsOut(Mat &);
    void handleBlurs(Mat &);
    void makeOneLine(Mat &);
    void makeOneLine2(Mat &);
    void publish(const double,const double);
    void selectNeededPoints(vector<Point> &,Mat &);
    void handleRouting(Mat &, Mat &, vector<Point> &);
    void selectRightAndDownPoints(vector<Point> &);
    void selectLeftAndDownPoints(vector<Point> &);
    void selectUpAndDownPoints(vector<Point> &);
    void connectPoints(Mat& ,vector<Point> &);
    void takeNewDirections();
    void setAngel(Point ,Point);
    bool computeBlackArea(vector <Point> ,Mat );
    bool checkRect(vector<Point>);
    bool isInsideBlackArea(vector<Point>, Mat & ,
                           vector<vector<Point>>& , vector<Vec4i>& );
    bool reachedTargetPoint(const Point target);
    string findDirection();
    Mat findWhiteContours(Mat &, Mat & ,Mat&);
    Point handleFinalPoints(vector<Point>&);
    QTime time;

    bool detected_qr;
    bool lock_qr;
    bool lock_points;
    bool qr_rect_found;
    bool optical_flow_lock;
    bool rotation_initiated;
    int erode_factor;
    int dilate_factor;
    int max_L,min_L;
    int max_H,min_H;
    int max_S,min_S;
    int black_max_L,black_min_L;
    int black_max_H,black_min_H;
    int black_max_S,black_min_S;
    int epsilon;
    double dist;
    double ang;
    string qr_data;
    string quad_current_direction;
    Point rotationTarget;
    Point rotationBase;
    Point image_center;
    Point qr_center;
    vector<Point> saved_points;
    vector<Point> qr_rect;
    vector<Point> selectedRotationPoints;
    vector<Point2f> optical_old_points;
    vector<Point> final_found_points;
    Mat old_frame;
    ros::Publisher publisher;
};

#endif
