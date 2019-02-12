#ifndef OPTFLOW_H
#define OPTFLOW_H

#include <QtExtSerialPort/qextserialport.h>
#include "capture_webcam.h"
#include "quad_board.h"
#include "QThread"
#include "QDebug"
#include "QTime"
#include <opt_handler/opt_Flow.h>

//#define _GRAPH
#define _Serial


#define MAX_COUNT               20
#define THRESH_REFRESH          8

#define winSize                 Size(17,17)
#define subPixWinSize           Size(5,5)

using namespace cv;
using namespace std;


class OptFlow: public QObject
{
    Q_OBJECT

public:
    explicit OptFlow(QObject *parent = 0);
    ~OptFlow();


    Mat colorframe, grayframe;
    QTimer *main_timer;
    capture_webcam cap;
    ros::NodeHandle opt_fNH;
    ros::Publisher opt_fPub;
    QTime d;

    Mat prevGray, image;
    vector<Point2f> points[2];
    TermCriteria termcrit;
    Point2f point;
opt_handler::opt_Flow opt_data;

    bool needToInit;
    bool done;

    size_t Features_counter;//count Features state true

    double delta_x, delta_y;

    void OptFlow_LK();

private:
    Quad_Board *transmiter;

public slots:
    void timerEvent(Mat raw_image);
};

#endif // OPTFLOW_H
