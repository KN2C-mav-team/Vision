#include "include/get_rgb.h"

get_rgb::myHeader get_rgb::myHead;

get_rgb::get_rgb(QObject *parent) : QObject(parent)
{

    rgb_compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    rgb_compression_params.push_back(80);

    GS_IP = new QHostAddress("192.168.1.115"); //115
    send_rgb_socket = new QUdpSocket();
    GS_img_port=3333;
    resize_rgb_flag=true;

    send_rgb_socket->connectToHost(*GS_IP, GS_img_port);


}

void get_rgb::imageCb_bgr(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

	/*if (resize_rgb_flag)
	{
    		resize(cv_ptr->image,cv_ptr->image,Size(320,240),0,0,0);
		qDebug() << "**** resizing rgb";
	}*/

    imencode(".jpg",cv_ptr->image,rgb_enc_buff,rgb_compression_params);
    unsigned int size=rgb_enc_buff.size();
    rgb_img_data.resize(size);
    for(unsigned int i=0; i<size; i++)
        rgb_img_data[i]=(unsigned char)rgb_enc_buff[i];


    QDataStream SendDataPacket(&datagram,QIODevice::WriteOnly);
    QByteArray StartPacket = "St";

    myHead.seq=msg->header.seq;    
    myHead.frame_id=msg->header.frame_id.data();
    myHead.height=msg->height;
    myHead.width=msg->width;
    myHead.encoding=msg->encoding.data();
    myHead.is_bigendian=msg->is_bigendian;
    myHead.step=msg->step;
//qDebug()<<"rgb step  "<<myHead.step<<"rgb height  "<<myHead.height<<"rgb width  "<<myHead.width;
myHead.step=1920;
myHead.height=480;
myHead.width=640;
    SendDataPacket<<StartPacket;
    SendDataPacket<<myHead.seq;
    SendDataPacket<< myHead.frame_id;
    SendDataPacket<<myHead.height;
    SendDataPacket<<myHead.width;
    SendDataPacket<<myHead.encoding;
    SendDataPacket<<myHead.is_bigendian;
    SendDataPacket<<myHead.step;

    SendDataPacket<<distort;
    SendDataPacket<<bin_x;
    SendDataPacket<<bin_y;
    SendDataPacket<<roi_x_off;
    SendDataPacket<<roi_y_off;
    SendDataPacket<<roi_h;
    SendDataPacket<<roi_w;
    SendDataPacket<<roi_do;
    SendDataPacket<<info_seq;

    SendDataPacket<<rgb_img_data;

    qDebug() << "rgb data size: " <<datagram.size();  //68 Byte

    send_rgb_socket->write(datagram);
    send_rgb_socket->flush();

    rgb_img_data.clear();
    rgb_enc_buff.clear();
    datagram.clear();

    // Update GUI Window
    //cv::imshow("Image window BGR", cv_ptr->image);
    //cv::waitKey(1);


}

void get_rgb::camInfoCb_bgr(const sensor_msgs::CameraInfoConstPtr &msg)
{

    distort=msg->distortion_model.data();
    bin_x=msg->binning_x;
    bin_y=msg->binning_y;
    roi_x_off=msg->roi.x_offset;
    roi_y_off=msg->roi.y_offset;
    roi_h=msg->roi.height;
    roi_w= msg->roi.width;
    roi_do=msg->roi.do_rectify;
    info_seq=msg->header.seq;

}

void get_rgb::camInfoCb_depth(const sensor_msgs::CameraInfoConstPtr &msg)
{
    depth_seq=msg->header.seq;
}

void get_rgb::resize_rgb_cb(const std_msgs::Int8ConstPtr& msg)
{
   resize_rgb_flag=!resize_rgb_flag;
qDebug() << "rgb resie flaggggggggggg:		" <<  resize_rgb_flag;
}

get_rgb::myHeader get_rgb::get_header()
{
    return myHead;
}


