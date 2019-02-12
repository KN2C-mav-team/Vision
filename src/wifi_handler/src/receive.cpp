#include "include/receive.h"
int receive::img_handler;
//bool receive::camnum;
receive::receive(QObject *parent) : QObject(parent)
{
    img_data_port=9632;
    rec_data_socket = new QUdpSocket(this);
    rec_data_socket->bind(img_data_port);
    winset_pub = winset_nh.advertise<wifi_handler::WinSetMsg>("winset_output", 1);
    markerset_pub = winset_nh.advertise<wifi_handler::MarkerSetMsg>("markerset_output", 1);
    rc_pub = winset_nh.advertise<std_msgs::Int8>("rc_output", 1);
    image_pub = winset_nh.advertise<std_msgs::Int8>("image_end", 1);
    connect(rec_data_socket,SIGNAL(readyRead()),this,SLOT(receive_status()));
    qDebug() << "aaaaaa";
    camnum=1;


}

receive::~receive()
{

}

bool receive::get_camnum()
{
   return camnum;
}



void receive::receive_status()
{
    qDebug() << "rec";
    datagram.resize(rec_data_socket->pendingDatagramSize());
    rec_data_socket->readDatagram(datagram.data(), datagram.size());
    rec_data_socket->flush();
    QDataStream datastream(&datagram,QIODevice::ReadWrite);
    datastream >> start;
    qDebug() << start;
    if(start=="img")
    {
        qDebug() << "img";
        datastream >> img_handler;
        if(img_handler==1)
        {
            winset.data=0;
            winset.mode=1;
            winset_pub.publish(winset);
            markerset.data=0;
            markerset.mode=0;
            markerset_pub.publish(markerset);
            emit status_receive();
        }
        else if(img_handler==2)
        {
            winset.data=0;
            winset.mode=0;
            winset_pub.publish(winset);
            markerset.data=0;
            markerset.mode=1;
            markerset_pub.publish(markerset);
            emit status_receive();
        }
        else if(img_handler==3)
        {
            winset.data=0;
            winset.mode=0;
            winset_pub.publish(winset);
            markerset.data=0;
            markerset.mode=0;
            markerset_pub.publish(markerset);
            emit status_receive();
        }
        else
        {
            winset.data=0;
            winset.mode=0;
            winset_pub.publish(winset);
            markerset.data=0;
            markerset.mode=0;
            markerset_pub.publish(markerset);
            emit status_receive();
        }
    }
    else if(start=="win")
    {
        qDebug() << "status";
        winset.data=2;
        datastream >> swapp;
        winset.vmin=swapp;
        qDebug() << winset.vmin;
        datastream >> swapp;
        winset.vmax=swapp;
        qDebug() << winset.vmax;
        datastream >> swapp;
        winset.smin=swapp;
        qDebug() << winset.smin;
        datastream >> swapp;
        winset.erosion_element=swapp;
        qDebug() <<winset.erosion_element;
        datastream >>swapp;
        winset.erosion_kernel=swapp;
        qDebug() <<swap;
        winset.erosion_kernel=swapp;
        datastream >> swapp;
        winset.dilation_element=swapp;
        qDebug() <<winset.dilation_element;
        datastream >> swapp;
        winset.dilation_kernel=swapp;
        qDebug() <<winset.dilation_kernel;
        winset_pub.publish(winset);
    }
    else if(start=="color")
    {
        qDebug() << "color";
        winset.data=1;
        datastream >> swapp;
        winset.hist_17=swapp/2;
        qDebug() << winset.hist_17;
        datastream >> swapp;
        winset.hist_18=swapp/2;
        qDebug() << winset.hist_18;
        datastream >> swapp;
        winset.hist_19=swapp/2;
        qDebug() << winset.hist_19;
        datastream >> swapp;
        winset.hist_20=swapp/2;
        qDebug() << winset.hist_20;
        datastream >> swapp;
        winset.hist_21=swapp/2;
        qDebug() << winset.hist_21;
        datastream >> swapp;
        winset.hist_22=swapp/2;
        qDebug() << winset.hist_22;
        winset_pub.publish(winset);
    }
    else if(start=="switch")
    {
        qDebug() << "swich";
        datastream >> swich;
        if(swich==1)
        {
            winset.data=3;
            winset.swich=1;
            winset_pub.publish(winset);
            markerset.data=1;
            markerset.swich=0;
            markerset_pub.publish(markerset);

        }
        else if(swich==2)
        {
            winset.data=3;
            winset.swich=2;
            winset_pub.publish(winset);
        }
        else if(swich==3)
        {
            qDebug() << "M exit";
            markerset.data=1;
            markerset.swich=1;
            markerset_pub.publish(markerset);
        }
        else if(swich==4)
        {
            imagend.data=1;
            image_pub.publish(imagend);
            imagend.data=0;
        }
    }
    else if(start=="rc")
    {
        datastream >> rc;
        rcc.data=rc;
        rc_pub.publish(rcc);
 qDebug() << "rcccccccccccccccccccccccccccccccccccccccccccccc"<<rc;
    }
    else if (start=="mark")
    {
        datastream >> swapp;
        markerset.data=2;
        markerset.dilate=swapp;
        markerset_pub.publish(markerset);
qDebug() << "mark";
    }
    else if (start=="markmod")
    {
        datastream >> swapp;
        markerset.data=3;
        markerset.invers=swapp;
        markerset_pub.publish(markerset);
qDebug() << "mark1";
    }
    else if (start=="markac")
    {
        datastream >> swapp;
        markerset.data=4;
        markerset.accuracy=swapp;
        markerset_pub.publish(markerset);
qDebug() << "mark2";
    }
    else if(start=="rst")
    {
        datastream >> swapp;
        winset.data=4;
        winset.rst=swapp;
        winset_pub.publish(winset);
    }
    else if(start=="ip")
    {
        datastream >> Ip;
        emit ip_set();
    }

    datagram.clear();

}

