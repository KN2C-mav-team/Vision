#include "include/sendimage.h"

SendImage::SendImage(QObject *parent) : QObject(parent)
{
   // spin_timer = new QTimer(this)
    GS_IP = new QHostAddress("192.168.1.115");
    send_img_socket = new QUdpSocket(this);
    GS_img_port=7897;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50);


    QObject::connect(this,SIGNAL(imageReadySig()),this,SLOT(send_image()));
//    QObject::connect(spin_timer,SIGNAL(timeout()),this,SLOT(spin_handler()));
//    spin_timer->start(10);
    qDebug() << "send";
}

void SendImage::raw_image(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        myframe = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        qDebug() << "frameeeewin";
        emit imageReadySig();
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }


}

void SendImage::prog_end(const std_msgs::Int8ConstPtr &msg)
{
exit(0);
}

void SendImage::send_image()
{

    qDebug() << "ok_window";
    resize(myframe,myframe,Size(160,120));
    imencode(".jpg",myframe,enc_buff,compression_params);
    unsigned int size=enc_buff.size();
    img_data.resize(size);
    for(unsigned int i=0; i<size; i++)
        img_data[i]=(unsigned char)enc_buff[i];
    send_img_socket->writeDatagram(img_data, *GS_IP, GS_img_port);
    send_img_socket->flush();
    img_data.clear();
    enc_buff.clear();

}
//void SendImage::spin_handler()
//{
//    ros::spinOnce();
//}

