#include "capture.h"

Mat Capture::frame;

Capture::Capture(QObject *parent) : QObject(parent)
{



}


Capture::~Capture()
{

}

void Capture::imageCb_bgr(const sensor_msgs::ImageConstPtr &msg)
{

    qDebug() << "cap: " <<QThread::currentThreadId();
    cv_bridge::CvImagePtr cv_ptr;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        frame=cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        exit(0);
    }

    emit imageReadySig(frame);
    //  qDebug()<<"frame emitted !!!";


}


