#include "include/capture.h"

capture::capture(QObject *parent) : QObject(parent)
{




}

void capture::imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    double exec_time = (double)cv::getTickCount();

        try
        {
          //  cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
            frame=cv_bridge::toCvShare(msg, "bgr8")->image;
//            cv::waitKey(3);

        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    emit(imageReady(frame));
    exec_time = ((double)cv::getTickCount() - exec_time)*1000./cv::getTickFrequency();
   // qDebug()<<"exec_time = "<<exec_time;

}