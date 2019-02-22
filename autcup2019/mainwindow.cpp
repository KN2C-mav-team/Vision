#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timer = new QTimer(this);	
   h_thread =new QThread();
    ph_thread =new QThread();   
    opt_thread =new QThread();
    qr_thread =new QThread();
    cam_thread = new QThread();
    line_thread =  new QThread();
    robot_thread =  new QThread();

    gh.moveToThread(h_thread);
    QR.moveToThread(qr_thread);
    ph.moveToThread(ph_thread);
    opt.moveToThread(opt_thread);
    line.moveToThread(line_thread);
    camera.moveToThread(cam_thread);
    robot.moveToThread(robot_thread);



    connect(timer,SIGNAL(timeout()),&camera,SLOT(frontFrame_ready()),Qt::ConnectionType::DirectConnection);
    connect(timer,SIGNAL(timeout()),&camera,SLOT(downFrame_ready()),Qt::ConnectionType::DirectConnection);
    connect(&camera,SIGNAL(down_image_line(Mat)),&ph,SLOT(imageCallBack(Mat)),Qt::ConnectionType::DirectConnection);
    connect(&camera,SIGNAL(front_image(Mat)),&gh,SLOT(imageCallback(Mat)),Qt::ConnectionType::DirectConnection);
//    connect(&camera,SIGNAL(down_image_qr(Mat)),&QR,SLOT(imageCallback(Mat)),Qt::ConnectionType::DirectConnection);
    connect(&camera,SIGNAL(down_image(Mat)),&opt,SLOT(timerEvent(Mat)),Qt::ConnectionType::DirectConnection);
 //   connect(&QR,SIGNAL(QR_signal(string,string,vector<Point>,int)),&ph,SLOT(Callback(string,string,vector<Point>,int)),Qt::ConnectionType::DirectConnection);
    connect(&opt,SIGNAL(optSig(int,int)),&robot,SLOT(optFlow_CB(int,int)),Qt::ConnectionType::DirectConnection);
   connect(&gh,SIGNAL(gateCallBack(int)),&robot,SLOT(gateSerial_CB(int)),Qt::ConnectionType::DirectConnection);
   connect(&ph,SIGNAL(connectorPkg(double,double)),&robot,SLOT(lineSerial_CB(double,double)),Qt::ConnectionType::DirectConnection);

    timer->start(2);
    h_thread->start();
    qr_thread->start();
    ph_thread->start();
    cam_thread -> start();
    opt_thread->start();

}

MainWindow::~MainWindow()
{

}
