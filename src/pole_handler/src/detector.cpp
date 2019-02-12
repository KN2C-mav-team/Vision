#include "include/capture.h"

capture::capture(QObject *parent) : QObject(parent)
{




}

void capture::imageCallback(const sensor_msgs::ImageConstPtr &msg)
{

    //time_ros = ros::Time::now();
        try
        {
          //  cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
/           frame=cv_bridge::toCvShare(msg, "bgr8")->image;

//            while(k<1)
//            {
//                namedWindow( "back", WINDOW_AUTOSIZE);
//                createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "back",
//                                &erosion_elem, 2);
//                //
//                createTrackbar( "Erosion Kernel size:\n 2n +1", "back",
//                                &erosion_size, 21);

//                //            /// Create Dilation Trackbar
//                //            createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "back",
//                //                            &dilation_elem, 2);

//                createTrackbar( "Dilation Kernel size:\n 2n +1", "back",
//                                &dilation_size, 21);
//                createTrackbar( "Vmin", "back", &vmin, 256, 0 );
//                createTrackbar( "Vmax", "back", &vmax, 256, 0 );
//                createTrackbar( "Smin", "back", &smin, 256, 0 );
//                k++;
//            }
//            //                GaussianBlur( frame, gaus_frame, Size(7, 7), 0, 0);
//            //                imshow("gaus_frame",gaus_frame);
//            //                medianBlur(frame,med_frame,5);
//            //                imshow("med_frame",med_frame);
//            //                boxFilter(frame, box_frame,-1,Size(5, 5));
//            //                imshow("box_frame",box_frame);
            //                frame=gaus_frame;
            //                // box is not good
           // process();
            cv::waitKey(3);
    //exec_time = ((double)cv::getTickCount() - exec_time)*1000./cv::getTickFrequency();
   // time_ros2 = ros::Time::now();

   // qDebug()<<"duration = "<<time_ros2.toSec()-time_ros.toSec();

        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    emit(frame);

}
