#include "include/get_depth.h"
#include <stdio.h>
#include <stdlib.h> 
#include "include/get_rgb.h"

get_depth::get_depth(QObject *parent) : QObject(parent)
{
    d_compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    d_compression_params.push_back(9);

    GS_IP = new QHostAddress("192.168.1.115"); //10.42.0.1
    send_left_socket = new QUdpSocket();
    send_right_socket = new QUdpSocket();
    send_notOver_socket = new QUdpSocket();
    GS_img_port=2222;
    GS_img_port_overload=6666;
    GS_img_port_overload2=7777;

    send_left_socket->connectToHost(*GS_IP, GS_img_port_overload);
    send_right_socket->connectToHost(*GS_IP, GS_img_port_overload2);
    send_notOver_socket->connectToHost(*GS_IP, GS_img_port);

    resize_depth_flag=true;


}


void get_depth::imageCb_depth(const sensor_msgs::ImageConstPtr& msg)
{

    cv_bridge::CvImagePtr cv_ptr;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
    }

    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    //medianBlur(cv_ptr->image,cv_ptr->image,5);

/*if (resize_depth_flag)
{
    resize(cv_ptr->image,cv_ptr->image,Size(320,240),0,0,0);
    qDebug() << "****resizing depth";

}*/

   // Mat chanal(d_dwnsmpld.rows,d_dwnsmpld.cols,CV_8UC4,d_dwnsmpld.data);

    imencode(".png",cv_ptr->image,d_enc_buff,d_compression_params);
    unsigned int size=d_enc_buff.size();
    d_img_data.resize(size);
    for(unsigned int i=0; i<size; i++)
        d_img_data[i]=(unsigned char)d_enc_buff[i];


    QDataStream SendDataPacket(&datagram,QIODevice::WriteOnly);
    QByteArray StartPacket = "St";

    uint32_t seq=msg->header.seq;
    QByteArray frame_id=msg->header.frame_id.data();
    uint32_t height=msg->height;
    uint32_t width=msg->width;
    QByteArray encoding=msg->encoding.data();
    uint8_t is_bigendian=msg->is_bigendian;
    uint32_t step=msg->step;
	//qDebug()<<"original depth step  "<<step<<"original depth height  "<<height<<"original depth width   "<<width;
    step=1280;
    height=480;
width=640;
SendDataPacket<<StartPacket;
    SendDataPacket<<seq;
    SendDataPacket<<frame_id;
    SendDataPacket<<height;
    SendDataPacket<<width;
    SendDataPacket<<encoding;
    SendDataPacket<<is_bigendian;
    SendDataPacket<<step;
    SendDataPacket<<d_img_data;

    qDebug() << "depth data size: " <<datagram.size();  //68 Byte

    if (datagram.size()>64000)
    {
        qDebug("overload");

        int half_size=datagram.size()/2;
        QByteArray left_half=datagram.left(half_size);
        QByteArray right_half=datagram.right(datagram.size()-half_size);

        send_left_socket->write(left_half);
        send_left_socket->flush();

        send_right_socket->write(right_half);
        send_right_socket->flush();

        left_half.clear();
        right_half.clear();
    }

    else
    {
	qDebug("not overload");
        send_notOver_socket->write(datagram);
        send_notOver_socket->flush();
    }

    d_img_data.clear();
    d_enc_buff.clear();
    datagram.clear();


    //cv::imshow("Image window depth", cv_ptr->image);
    //cv::waitKey(1);

}

void get_depth::resize_depth_cb(const std_msgs::Int8ConstPtr& msg)
{
   resize_depth_flag=!resize_depth_flag;
qDebug() << "depth flaggggggggggggggggggg:	" <<  resize_depth_flag;
}


