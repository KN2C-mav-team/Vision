#include <include/quad_board.h>//

//
Quad_Board::Quad_Board(QObject *parent) : QObject(parent)
{
    QString portName = QLatin1String(_DEV_SERIAL);
    flight_pub = flight_nh.advertise<std_msgs::Int8>("flight_mode", 1);
    camnum_pub = flight_nh.advertise<std_msgs::Int8>("camnum", 1);
    Turn_back=0;
    serial_port =  new QextSerialPort(QString(portName), QextSerialPort::EventDriven);
    serial_port->close();
    serial_port->setPortName(_DEV_SERIAL);
    serial_port->open(QIODevice::ReadWrite);
    serial_port->setBaudRate(BAUD57600);//
    serial_port->setParity(PAR_NONE);
    serial_port->setFlowControl(FLOW_OFF);
    serial_port->setDataBits(DATA_8);
    serial_port->setStopBits(STOP_1);
 
    serial_port->setDtr(0);
    serial_port->setRts(0);

    ready = false;
    pack_started=0;
    decode_stage = 0;
    sum = 0;
    j=0;
    integer_received_counter = 0;
    Num = 0;

    if( serial_port->isOpen())
    {
        qDebug("Serial Port Opened  .");
    }
    else
    {
        qDebug("Serial Port NOT Opened!!!!!!!");

    }

    window_st.delta_x=0;
    window_st.delta_y=0;
    window_st.scale=0;
    window_st.status=5;

    marker_st.delta_x=0;
    marker_st.delta_y=0;
    marker_st.status=5;

    optical_st.delta_x=0;
    optical_st.delta_y=0;
    optical_st.status=0;

    orbflag=0;
    mode_flag=1;

    orb_port=5496;
    orb_socket = new QUdpSocket(this);
    orb_socket->bind(orb_port);
     connect(orb_socket,SIGNAL(readyRead()),this,SLOT(orbsend()));

    GS_IP = new QHostAddress("192.168.1.147");
    send_data_socket = new QUdpSocket(this);
    GS_data_port=8745;
    //chera comment????????????????????????????????
    connect(serial_port, SIGNAL(readyRead()), this, SLOT(readData())); //chera nmire tu ready data???? bad chera hamash bayad chmod bezanam vase usb k bezare?
    //??????????????????????????????????????????????
    // main_timer = new QTimer(this);
    //connect(main_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));


    // main_timer->start(33);

    test_count=0;
    camnum=0;

}

Quad_Board::~Quad_Board()
{

}

void Quad_Board::timerEvent()
{

    qDebug() << "*timer*";
    test_count++;
    Fill_Data(8,(int)test_count,(int)test_count,(int)test_count,(int)test_count,(int)5,(int)6,(int)7,(int)8);
    if (test_count>65000)
    {
        test_count=0;
    }
}

void Quad_Board::sendata()
{
    if (!ros::ok)
        exit(0);

    QDataStream SendDataPacket(&dataa,QIODevice::WriteOnly);
    wstatus=data[0];
    SendDataPacket<< wstatus;
    //SendDataPacket<< data[1];
    send_data_socket->writeDatagram(dataa,*GS_IP,GS_data_port);
    send_data_socket->flush();
    //qDebug() <<"get      "<< wstatus ;//<<data[1];

}

void Quad_Board::depthCB(const serial_handler::depthToSerial &msg){
    Turn_back=1; //ye ja 0 kon
}

void Quad_Board::rc(const std_msgs::Int8ConstPtr &msg)
{
    rccc=msg->data;
qDebug() << "serialllllllllllllllllllll "<<rccc;
}
void Quad_Board::windowCB(const serial_handler::WinDetMsgConstPtr &msg)
{
    //Fill_Data(5,msg->delta_x,msg->delta_y,msg->scale,msg->status,Turn_back);
    qDebug() << "window: " << msg->delta_x  << msg->delta_y  << "                               " << msg->scale  << msg->status << Turn_back;

    window_st.delta_x=msg->delta_x;
    window_st.delta_y=msg->delta_y;
    window_st.scale=msg->scale;
    window_st.status=msg->status;
    orbflag=2;
}

void Quad_Board::marker(const serial_handler::MarkerDetMsgConstPtr &msg)
{
    //if(window_st.status==5)  //?????????????
    {
        //Fill_Data(3,msg->delta_x,msg->delta_y,msg->status);
        marker_st.delta_x=msg->delta_x;
        marker_st.delta_y=msg->delta_y;
        marker_st.status=msg->status;
        qDebug() << "marker: " << msg->delta_x  << msg->delta_y  << msg->status;
    }


}


void Quad_Board::optical(const serial_handler::OpticalMsgConstPtr &msg)
{

    //mut.lock();
    if(!ros::ok())
        exit(0);

    window_str serial_W;
    serial_W.delta_x=window_st.delta_x;
    serial_W.delta_y=window_st.delta_y;
    serial_W.scale=window_st.scale;
    serial_W.status=window_st.status;
    marker_str serisl_M;
    serisl_M.delta_x=marker_st.delta_x;
    serisl_M.delta_y= marker_st.delta_y;
    serisl_M.status= marker_st.status;

    optical_st.delta_x=msg->delta_x;
    optical_st.delta_y=msg->delta_y;
    optical_st.status=msg->status;

    orb_stru serial_orb;
    serial_orb.c_x=cx;
    serial_orb.c_y=cy;
    serial_orb.c_z=cz;
    serial_orb.c_w=cw;
    serial_orb.s_x=sx;
    serial_orb.s_y=sy;
    serial_orb.s_z=sz;
    serial_orb.s_w=sw;

    //    orb_str serial_orb;
    //    serial_orb.lowc_x=orbb.lowc_x;
    //    serial_orb.lowc_y=orbb.lowc_y;
    //    serial_orb.lowc_z=orbb.lowc_z;
    //    serial_orb.lowc_w=orbb.lowc_w;
    //    serial_orb.hic_x=orbb.hic_x;
    //    serial_orb.hic_y=orbb.hic_y;
    //    serial_orb.hic_z=orbb.hic_z;
    //    serial_orb.hic_w=orbb.hic_w;
    //    serial_orb.lows_x=orbb.lows_x;
    //    serial_orb.lows_y=orbb.lows_y;
    //    serial_orb.lows_z=orbb.lows_z;
    //    serial_orb.lows_w=orbb.lows_w;
    //    serial_orb.his_x=orbb.his_x;
    //    serial_orb.his_y=orbb.his_y;
    //    serial_orb.his_z=orbb.his_z;
    //    serial_orb.his_w=orbb.his_w;


    int rcc;
    rcc=rccc;

    Fill_Data(20,(int)optical_st.delta_x,(int)optical_st.delta_y,(int)optical_st.status,(int)serial_W.delta_x,(int)serial_W.delta_y,(int)serial_W.status,(int)serial_W.scale,(int)Turn_back,(int)serisl_M.delta_x,(int)serisl_M.delta_y,(int)serisl_M.status,(int)serial_orb.c_x,(int)serial_orb.c_y,(int)serial_orb.c_z,(int)serial_orb.c_w,(int)serial_orb.s_x,(int)serial_orb.s_y,(int)serial_orb.s_z,(int)serial_orb.s_w,(int)rcc);
    qDebug() << "opt_x: " << optical_st.delta_x;
    //Fill_Data(28,(int)optical_st.delta_x,(int)optical_st.delta_y,(int)optical_st.status,(int)serial_W.delta_x,(int)serial_W.delta_y,(int)serial_W.status,(int)serial_W.scale,(int)Turn_back,(int)serisl_M.delta_x,(int)serisl_M.delta_y,(int)serisl_M.status,(int)serial_orb.lowc_x,(int)serial_orb.hic_x,(int)serial_orb.lowc_y,(int)serial_orb.hic_y,(int)serial_orb.lowc_z,(int)serial_orb.hic_z,(int)serial_orb.lowc_w,(int)serial_orb.hic_w,(int)serial_orb.lows_x,(int)serial_orb.his_x,(int)serial_orb.lows_y,(int)serial_orb.his_y,(int)serial_orb.lows_z,(int)serial_orb.his_z,(int)serial_orb.lows_w,(int)serial_orb.his_w,(int)rcc);


}

void Quad_Board::Mpc_decode(u_int8_t _data)
{
    switch(pack_started)
    {

    case 0:
        if(_data == 0xff)
            pack_started = 1;
        else
            pack_started = 0;

        //qDebug() << "ff1";

        break;

    case 1:
        if(_data == 0xff)
            pack_started = 2;
        else
            pack_started = 0;

        //qDebug() << "ff2";

        break;

    case 2:
        Len =_data - 3;
        pack_started = 3;

        //qDebug() << "case2";

        break;

    case 3:
        if(_data == 0)
            pack_started = 4;
        else
            pack_started = 0;

        //qDebug() << "case3";

        break;


    case 4:

        if( Len == 0)
        {

            sum =  sum + (u_int8_t)_data;
            if( sum == 0)
            {
                sum=0;
                pack_started =0;
                ready =1;
                data_num =  j;
                j=0;
            }
            else
            {
                j=0;
                sum=0;
                pack_started =0;
            }

        }
        else
        {
            sum =  sum + (u_int8_t)_data;
            conv.byte[0] = _data;
            pack_started =5;
        }

        //qDebug() << "case4";

        break;
    case 5:
        sum =  sum + (u_int8_t)_data;
        conv.byte[1] = _data;
        pack_started =4;
        data[j] =  conv.real;
        Len =  Len - 2;
        j++;

        //        qDebug() << "j  	" << j-1 << "		data		" << data[j-1];
        //qDebug() << "case5";

        break;

    }



}



void Quad_Board::Empty_Data()
{
    Num = 0;
    check_Sum = 0;
    data_send.clear();
    data_send.append(QByteArray::fromHex("FF"));
    data_send.append(QByteArray::fromHex("FF"));

}

void Quad_Board::Fill_Data(uchar num,...)
{

    va_list arguments;
    va_start( arguments, num );

    Empty_Data();
    data_send.append(((num * 2) + 3));
    data_send.append(Num);
    for(int i = 0; i < num; i++)
    {
        conv.real=(uint)va_arg( arguments, int );
        //qDebug()<<conv.real;
        check_Sum = check_Sum + (uchar)conv.byte[0] + (uchar)conv.byte[1];
        data_send.append(conv.byte[0]);
        data_send.append(conv.byte[1]);
    }
    Send_Data();

    va_end ( arguments );

}

void Quad_Board::Send_Data()
{
    data_send.append((~(check_Sum) + 1));
    serial_port->write(data_send);
    //qDebug()<<data_send.toHex();
    serial_port->flush();

}
void Quad_Board::readData()
{
    //  qDebug()<<"**read";

    QByteArray _data = serial_port->readAll();
    unsigned char a;

    //qDebug() << "**data:  " << _data.toHex();


    for (int i=0;i<_data.size();i++)
    {
        a=_data.at(i) ;
        Mpc_decode(a);
    }

    sendata();

    //qDebug() << "***************read: " << data[2];

    if(data[2]==4 && mode_flag==1)
    {
        mode_flag=0;
        //  qDebug() << "^^^^^^^^^^^^^^^^^flight mode : 4";
        flight.data=4;
        flight_pub.publish(flight);
    }

    else if (data[2]==0)
    {
        // qDebug() << "^^^^^^^^^^^^^^^^^flight mode : 0";
        mode_flag=1;
    }

    if(camnum != data[3])
    {
       cam.data=data[3];
       camnum_pub.publish(cam);
       camnum=data[3];
    }


    //Fill_Data(3,1,2,3);

qDebug() << "^^^^^^^^^^^^^^^^^flight mode : 0"<< data[3];
}

void Quad_Board::orbsend()
{
qDebug() << "rccccccccccccccccccccccc";
    orb.resize(orb_socket->pendingDatagramSize());
    orb_socket->readDatagram(orb.data(), orb.size());
    orb_socket->flush();
    QDataStream qorb(&orb,QIODevice::ReadWrite);
qorb>>start;
if(start=="orb")
{
    qorb>> cx;
    qorb>> cy;
    qorb>> cz;
    qorb>> cw;
    qorb>> sx;
    qorb>> sy;
    qorb>> sz;
    qorb>> sw;
    orb.clear();
    orbflag=1;
    qDebug() << "ord recieve";
}
else if(start=="ip")
{
qorb>> Ip;
GS_IP = new QHostAddress(Ip);
qDebug() << "ipppppppppppppppppppppppppppppppppppppppppppp";
}

    //    orbb.lowc_x=cx & 0xffff;
    //    orbb.hic_x=(cx >> 16) & 0xffff;
    //    orbb.lowc_y=cy & 0xffff;
    //    orbb.hic_y=(cy >> 16) & 0xffff;
    //    orbb.lowc_z=cz & 0xffff;
    //    orbb.hic_z=(cz >> 16) & 0xffff;
    //    orbb.lowc_w=cw & 0xffff;
    //    orbb.hic_w=(cw >> 16) & 0xffff;
    //    orbb.lows_x=sx & 0xffff;
    //    orbb.his_x=(sx >> 16) & 0xffff;
    //    orbb.lows_y=sy & 0xffff;
    //    orbb.his_y=(sy >> 16) & 0xffff;
    //    orbb.lows_z=sz & 0xffff;
    //    orbb.his_z=(sz >> 16) & 0xffff;
    //    orbb.lows_w=sw & 0xffff;
    //    orbb.his_w=(sw >> 16) & 0xffff;
    //Fill_Data(8,cx,cy,cz,cw,sx,sy,sz,sw);
}




