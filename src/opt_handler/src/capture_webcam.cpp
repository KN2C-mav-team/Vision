#include "capture_webcam.h"



capture_webcam::capture_webcam(QObject *parent) : QObject(parent)
{

}

void capture_webcam::imageEvent(const sensor_msgs::ImageConstPtr &msg)
{

    try
    {
        frame=cv_bridge::toCvShare(msg, "bgr8")->image;



    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
    emit(imageReady(frame));
}

capture_webcam::~capture_webcam()
{
    delete main_timer;
}

int capture_webcam::get_frame_status()
{
    return farme_status;
}

double capture_webcam::get_Diff_Time()
{
    return Diff_Time;
}

void capture_webcam::set_frame_status(int status)
{
    farme_status = status;
}

