#include "capture.h"

capture::capture(QObject *parent) : QObject(parent)
{
    down_cam.open(DOWN_CAM_NUM);
    if( down_cam.isOpened() )
    {
        down_cam.set(CV_CAP_PROP_FPS, DOWN_FPS);
        down_cam.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
        down_cam.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
       // qDebug()<<"Down cam opened"; // up to here program worked
    	down_cam >> down_frame ;

       

    }
    else
    {
        qDebug()<<"could not open down_cam\n";
        exit(0);
    }

  /*  front_cam.open(FRONT_CAM_NUM);
    if( front_cam.isOpened() )
    {
        front_cam.set(CV_CAP_PROP_FPS, FRONT_FPS);
        front_cam.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
        front_cam.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//qDebug()<<"Front cam opened"; // up to here program worked
	front_cam >> front_frame ;
    }
    else
    {
        qDebug()<<"could not open front_cam\n";
        exit(0);
    }*/


}

void capture::frontFrame_ready()
{
    front_cam >> front_frame;
   // imshow("front frame",front_frame);
    emit front_image(front_frame);
}

void capture::downFrame_ready()
{
    down_cam >> down_frame ;
   // imshow("down frame",down_frame);
    emit down_image(down_frame);
    
}

