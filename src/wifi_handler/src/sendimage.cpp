#include "include/sendimage.h"


SendImage::SendImage(QObject *parent) : QObject(parent)
{
    rece = new receive();
    main_timer = new QTimer(this);
    //spin_timer = new QTimer(this);
    GS_IP = new QHostAddress("192.168.1.147");
    send_img_socket = new QUdpSocket(this);
    GS_img_port=4444;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(30);

    GS_IP_status = new QHostAddress("192.168.1.147");
    send_status_socket = new QUdpSocket(this);
    GS_status_port=5555;

    connect(this,SIGNAL(imageReadySig()),this,SLOT(send_image()));
    connect(this,SIGNAL(imageReadySig_marker()),this,SLOT(send_image_marker()));
    connect(this,SIGNAL(imageReadySig_raw()),this,SLOT(send_image_raw()));
    connect(rece,SIGNAL(status_receive()),this,SLOT(imge_handler()),Qt::DirectConnection);
connect(rece,SIGNAL(ip_set()),this,SLOT(Ip_set()),Qt::DirectConnection);
    connect(main_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    // connect(spin_timer,SIGNAL(timeout()),this,SLOT(spin_handler()));
    // spin_timer->start(30);
    main_timer->start(500);
    qDebug() << "send";
    cam=2;
}
//***************************img handler****************************
void SendImage::imge_handler()
{
    qDebug() << "handle";
    if(rece->img_handler==1)
    {
        qDebug() << "1";
        img_handle=1;
    }
    else if(rece->img_handler==2)
    {
        qDebug() << "2";
        img_handle=2;
    }
    else if(rece->img_handler==3)
    {
        qDebug() << "3";
        img_handle=3;
    }
    else
    {
        qDebug() << "0";
        img_handle=0;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//***********************img subscribers*****************************
void SendImage::imageCB(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        myframe = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        // qDebug() << "frameeeewin";
        emit imageReadySig();
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }


}

void SendImage::marker_image(const sensor_msgs::ImageConstPtr &msg1)
{
    try
    {
        myframe1 = cv_bridge::toCvCopy(msg1, sensor_msgs::image_encodings::BGR8)->image;
        emit imageReadySig_marker();
        //qDebug() << "frameeeeemark";
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }


}

void SendImage::front_image(const sensor_msgs::ImageConstPtr &msg)
{
    if(img_handle==3 && cam == 2)
    {


            try
            {
                myframe2 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
                 qDebug() << "frameeeewin";
                emit imageReadySig_raw();
            }
            catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("cv_bridge exception: %s", e.what());
                return;
            }

    }
}

void SendImage::below_image(const sensor_msgs::ImageConstPtr &msg)
{
    if(img_handle==3 && cam ==3)
    {


            try
            {
                myframe2 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
                transpose(myframe2,myframe2);
                flip(myframe2,myframe2,0);
                // qDebug() << "frameeeewin";
                emit imageReadySig_raw();
            }
            catch (cv_bridge::Exception& e)
            {
                ROS_ERROR("cv_bridge exception: %s", e.what());
                return;
            }

    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//**********************img senders***********************************
void SendImage::send_image()
{
    if(img_handle==1)
    {
        // qDebug() << "ok_window";
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
}

void SendImage::send_image_marker()
{
    if(img_handle==2)
    {
        imencode(".jpg",myframe1,enc_buff,compression_params);
        unsigned int size=enc_buff.size();
        img_data.resize(size);
        for(unsigned int i=0; i<size; i++)
            img_data[i]=(unsigned char)enc_buff[i];
        send_img_socket->writeDatagram(img_data, *GS_IP, GS_img_port);
        // qDebug() << "ok_marker";
        send_img_socket->flush();
        img_data.clear();
        enc_buff.clear();
    }
}

void SendImage::send_image_raw()
{
    if(img_handle==3)
    {
        imencode(".jpg",myframe2,enc_buff,compression_params);
        unsigned int size=enc_buff.size();
        img_data.resize(size);
        for(unsigned int i=0; i<size; i++)
            img_data[i]=(unsigned char)enc_buff[i];
        send_img_socket->writeDatagram(img_data, *GS_IP, GS_img_port);
        qDebug() << "ok_raw";
        send_img_socket->flush();
        img_data.clear();
        enc_buff.clear();
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*******************************status setters***********************
void SendImage::window_s(const wifi_handler::WinDetMsgConstPtr &msg2)
{
    win=msg2->status;
}

void SendImage::marker_s(const wifi_handler::MarkerDetMsgConstPtr &msg3)
{
    marker=msg3->status;
}

void SendImage::opt_s(const std_msgs::Int8ConstPtr &msg4)
{
    opt=msg4->data;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//**********************status sender & rosspin**********************
void SendImage::timerEvent()
{
    QDataStream SendDataPacket(&status_data,QIODevice::WriteOnly);
    SendDataPacket<< win;
    SendDataPacket<< marker;
    SendDataPacket<< opt;
    send_status_socket->writeDatagram(status_data,*GS_IP_status,GS_status_port);
    send_status_socket->flush();
    enc_buff_status.clear();
    opt =win = marker = 1;
    if(!ros::ok())
        exit(0);
    //ros::spinOnce();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//void SendImage::spin_handler()
//{
//    ros::spinOnce();
//}



void SendImage::cam_s(const std_msgs::Int8ConstPtr &msg4)
{
    cam=msg4->data;
}

void SendImage::Ip_set()
{
   GS_IP = new QHostAddress(rece->Ip);
   GS_IP_status = new QHostAddress(rece->Ip);

}
