#ifndef QUAD_BOARD_H
#define QUAD_BOARD_H

#include <QObject>
#include "qdebug.h"
#include "ros/ros.h"
#include <QtExtSerialPort/qextserialport.h>
#include <QThread>
#include <QUdpSocket>
#include <QDataStream>
#include <serial_handler/MarkerDetMsg.h>
#include <serial_handler/WinDetMsg.h>
#include <serial_handler/depthToSerial.h>
#include <serial_handler/OpticalMsg.h>
#include <QTimer>
#include <QMutex>
#include <std_msgs/Int8.h>
#define _DEV_SERIAL            "/dev/ttyS4"
//

class Quad_Board : public QObject
{
    Q_OBJECT
public:
    explicit Quad_Board(QObject *parent = 0);
    ~Quad_Board();

    ros::NodeHandle flight_nh;
    ros::Publisher flight_pub;
    ros::Publisher camnum_pub;
    void sendata();
    void depthCB(const serial_handler::depthToSerial &msg);
    int Turn_back;
    void windowCB(const serial_handler::WinDetMsgConstPtr& msg);
    void marker(const serial_handler::MarkerDetMsgConstPtr& msg);
    void optical(const serial_handler::OpticalMsgConstPtr& msg);
    void rc(const std_msgs::Int8ConstPtr& msg);
    union _ch2int
    {
        int16_t real;
        char byte[2];
    };


    typedef union _ch2int  ch2int;



    void Mpc_decode(u_int8_t _data);
    void Empty_Data();
    void Fill_Data(uchar num , ... );
    void Send_Data();

    QUdpSocket *send_data_socket;
    QHostAddress *GS_IP;
    int GS_data_port;
    std::vector<uchar> enc_buff;
    std::vector<int> compression_params;
    QByteArray dataa;
    int wstatus;

    QUdpSocket *orb_socket;
    int orb_port;
    QByteArray orb;
    int cx;
    int cy;
    int cz;
    int cw;
     int sx;
     int sy;
    int sz;
    int sw;
     int orbflag;
     int rccc;
    std_msgs::Int8 flight;
    std_msgs::Int8 cam;
QString Ip;
QByteArray start;
public:

    bool ready;
    char decode_stage;
    char sum;
    char received_packet_length;
    int integer_received_counter;
    int data_num;
    int data_recieved[32];
    int16_t data[32];
    int j;
    int Len;
    int pack_started;
    QByteArray data_send;
    uchar Num;
    uchar check_Sum;
    QextSerialPort *serial_port;
    ch2int conv;
QTimer *main_timer;
QMutex mut;
int test_count;
bool mode_flag;
int camnum;

typedef struct
{
     int delta_x;
     int delta_y;
     int scale;
     int status;
}window_str;

window_str window_st;


typedef struct
{
    int delta_x;
    int delta_y;
    int status;
}marker_str;

marker_str marker_st;

typedef struct
{
    int delta_x;
    int delta_y;
    int status;
}optical_str;

optical_str optical_st;

typedef struct
{
     int lowc_x;
     int hic_x;
     int lowc_y;
     int hic_y;
     int lowc_z;
     int hic_z;
     int lowc_w;
     int hic_w;
     int lows_x;
     int his_x;
     int lows_y;
     int his_y;
     int lows_z;
     int his_z;
     int lows_w;
     int his_w;
}orb_str;

typedef struct
{
     int c_x;

     int c_y;

     int c_z;

     int c_w;

     int s_x;

     int s_y;

     int s_z;

     int s_w;

}orb_stru;

orb_str orbb;
public slots:
    void readData();
    void orbsend();
void timerEvent();

};


#endif // QUAD_BOARD_H
