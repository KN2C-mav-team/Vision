#include "capture.h"

capture::capture(QObject *parent) : QObject(parent)
{

    down_cam.open("/home/danial/Webcam/2019-02-27-000859.webm");
    if( down_cam.isOpened() )
    {
        down_cam.set(CV_CAP_PROP_FPS, DOWN_FPS);
        down_cam.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
        down_cam.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
       // qDebug()<<"Down cam opened";
        down_cam >> down_frame ;



    }
    else
    {
        qDebug()<<"could not open down_cam\n";
        exit(0);
    }

//    front_cam.open(FRONT_CAM_NUM);
//    if( front_cam.isOpened() )
//    {
//        front_cam.set(CV_CAP_PROP_FPS, FRONT_FPS);
//        front_cam.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
//        front_cam.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
//    //qDebug()<<"Front cam opened";
//    front_cam >> front_frame ;
//    }
//    else
//    {
//        qDebug()<<"could not open front_cam\n";
//        exit(0);
//    }


}

void capture::frontFrame_ready()
{
    front_cam >> front_frame;

    //imshow("front frame",front_frame);
    emit front_image(front_frame);
}

void capture::downFrame_ready()
{
 //   qDebug()<<"CAM Thread"<<thread()->currentThreadId();
    Mat l;
    down_cam >> down_frame ;
    l = down_frame.clone();
    //imshow("down frame",down_frame);
  //  emit down_image(down_frame);
    emit down_image_line(l);
    emit down_image_qr(l);

}


