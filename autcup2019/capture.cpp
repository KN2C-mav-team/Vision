#include "capture.h"

capture::capture(QObject *parent) : QObject(parent)
{

    down_cam.open(DOWN_CAM_NUM);
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
    

    front_cam.open(FRONT_CAM_NUM);
    if( front_cam.isOpened() )
    {
        front_cam.set(CV_CAP_PROP_FPS, FRONT_FPS);
        front_cam.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
        front_cam.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
        //qDebug()<<"Front cam opened";
        front_cam >> front_frame ;
    }
    else
    {
        qDebug()<<"could not open front_cam\n";
        exit(0);
    }


    //QString front_address="/media/odroid/MARYAM/logs/";
    QString front_address="/home/kimia/Desktop/";
    //front_address.append(QTime::currentTime().toString());
    front_address.append("front_cap.avi");
    front_video = VideoWriter(front_address.toStdString(),CV_FOURCC('M','J','P','G'),10, Size(FRAME_WIDTH,FRAME_HEIGHT),true);

   // QString down_address="/home/odroid/cam_log/";
     QString down_address="/home/kimia/Desktop/";
    //down_address.append(QTime::currentTime().toString());
    down_address.append("down_cap.avi");
    down_video = VideoWriter(down_address.toStdString(),CV_FOURCC('M','J','P','G'),10, Size(FRAME_WIDTH,FRAME_HEIGHT),true);

}

void capture::frontFrame_ready()
{
    front_cam >> front_frame;
    //front_video.write(front_frame);
    //imshow("front frame",front_frame);
    emit front_image(front_frame);

}

void capture::downFrame_ready()
{
    qDebug()<<"CAM Thread"<<thread()->currentThreadId();
    Mat l;
    down_cam >> down_frame ;
   // down_video.write(down_frame);
    l = down_frame.clone();
    //imshow("down frame",down_frame);
    emit down_image(down_frame);
    emit down_image_land(down_frame);
    emit down_image_line(l);
    emit down_image_qr(l);
    
}





