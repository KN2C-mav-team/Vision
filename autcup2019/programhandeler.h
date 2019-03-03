#ifndef PROGRAMHANDELER_H
#define PROGRAMHANDELER_H

#include <QObject>

#include "includes.h"
//#include "LineEquations.h"
//#define MINIMUM_RECTANGLE_AREA 200
//#define MAXIMUM_RECTANGLE_AREA 2800
#define QUAD_STARTING_DIRECTION 'N'
#define STARTING_QUEUED_MISSION 1
#define MAX_SCALAR 255
#define MAX_EPSILON 20
#define MAX_ERODE 10
#define MAX_DILATE 10
#define STARTING_ERODE 0
#define STARTING_DILATE 1
#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'W'
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240

#define LEFT_X_OFFSET 0
#define LEFT_Y_OFFSET 0

#define BASE_LEFT_X_OFFSET 0
#define BASE_LEFT_Y_OFFSET 0


#define RIGHT_X_OFFSET 0
#define RIGHT_Y_OFFSET 0

#define BASE_RIGHT_X_OFFSET 0
#define BASE_RIGHT_Y_OFFSET 0
#define ROTATION_TIME 3500

//#define TIME_ROUNTING

#define DEBUG
//#define HIER_EN
#define SAY_QR_DATA

//#define LOG
//#define DELAY
//#define VIDEO_LOG


using namespace cv;
using namespace std;

static const string WINDOW = "Image window";

class ProgramHandeler : public QObject
{
    Q_OBJECT
public:
    explicit ProgramHandeler(QObject *parent = 0);


private:

    void whiteThsOut(Mat &);
    void blackThsOut(Mat &);
    void handleBlurs(Mat &);
    void makeOneLine(Mat &);
    void publish(const double, const double);
    void selectNeededPoints(vector<Point> &,Mat &);
    void handleRouting(Mat &, Mat &, vector<Point> &);
    void secondHandleRouting(Mat &, Mat &, vector<Point> &);
    void selectRightAndDownPoints(vector<Point> &);
    void selectLeftAndDownPoints(vector<Point> &);
    void selectUpAndDownPoints(vector<Point> &);
    void connectPoints(Mat& ,vector<Point> &);
    void takeNewDirections();
    void setAngel(Point ,Point);
    void choosePoints(vector<vector<Point> >contours, vector<Vec4i>,
                      vector<Point>&, Mat &);
    void sortByY(vector<Point>&);
    void mergePointsByNearest(vector<Point>, vector<Point> &found_points);
    bool computeBlackArea(vector <Point> ,Mat );
    bool checkRect(vector<Point>);
    bool isInsideBlackArea(vector<Point>, Mat & ,
                           vector<vector<Point> >& , vector<Vec4i>& );
    bool reachedTargetPoint(const Point target);
    bool detectedLinesOf(string , const vector<Point>);
    bool RightAndDownPointsIsValid(const vector<Point>);
    bool LeftAndDownPointsIsValid(const vector<Point>);
    string findDirection();
    Mat findWhiteContours(Mat &);
    Point handleFinalPoints(vector<Point>&);
    QTime time;
    QTime lock_time;

    bool detected_qr;
    bool lock_qr;
    bool lock_points;
    bool qr_rect_found;
    bool optical_flow_lock;
    bool rotation_initiated;
    bool straight_lock;
    bool onlyOnePointDetected;
    bool rotate_clock_lock;
    bool lock_wanted_lines;

    int mission_counter;
    char next_mission;
    int queued_mission;

    int qr_flag;
    int MAXIMUM_RECTANGLE_AREA;
    int MINIMUM_RECTANGLE_AREA;
    int erode_factor;
    int mission;
    int rotation_angle = -90;
    int old_rotation_angle = 0;
    int dilate_factor;
    int max_L,min_L;
    int max_H,min_H;
    int max_S,min_S;
    int black_max_L,black_min_L;
    int black_max_H,black_min_H;
    int black_max_S,black_min_S;
    int hassan = 0;
    int epsilon;
    double dist;
    double ang;
    char qr_data;
    //string qr_data;
    string qr_dir;
    string old_qr_dir = "forward";
    char quad_current_direction;
    //string quad_current_direction;
    Point rotationTarget;
    Point rotationBase;
    Point straightBase;
    Point straightTarget;
    Point image_center;
    Point image_center_down;
    Point qr_center;
    vector<Point> saved_points;
    vector<Point> qr_rect;
    vector<Point> selectedRotationPoints;
    vector<Point> selectedStraightPoints;
    vector<Point2f> optical_old_points;
    vector<Point> final_found_points;
    Mat old_frame;

signals:
    void connectorPkg(double, double, int);

public slots:
    void imageCallBack(cv::Mat raw_image);
    void Callback(const string , const string , const std::vector<cv::Point> , int);
    void gateStatusCallBack(int movement);

};

#endif // PROGRAMHANDELER_H

