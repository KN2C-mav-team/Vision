#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

//    camera.moveToThread(&cam_thread);
//    opt.moveToThread(&opt_thread);
//    gh.moveToThread(&h_thread);
    ph.moveToThread(&ph_thread);
    QR.moveToThread(&qr_thread);
//    line.moveToThread(&line_thread);
    robot.moveToThread(&robot_thread);


    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< string >("string");
    qRegisterMetaType< std::vector<cv::Point> >("std::vector<cv::Point>");
    connect(&timer,SIGNAL(timeout()),&camera,SLOT(frontFrame_ready()));
    connect(&timer,SIGNAL(timeout()),&camera,SLOT(downFrame_ready()));
    connect(&camera,SIGNAL(down_image_line(cv::Mat)),&ph,SLOT(imageCallBack(cv::Mat)));
//    connect(&camera,SIGNAL(front_image(cv::Mat)),&gh,SLOT(imageCallback(cv::Mat)));
    connect(&camera,SIGNAL(down_image_qr(cv::Mat)),&QR,SLOT(imageCallback(cv::Mat)));
//    connect(&camera,SIGNAL(down_image(cv::Mat)),&opt,SLOT(timerEvent(cv::Mat)));

    connect(&QR,SIGNAL(QR_signal(string,string,std::vector<cv::Point>,int)),&ph,SLOT(Callback(string,string,std::vector<cv::Point>,int)));
//    connect(&opt,SIGNAL(optSig(int,int)),&robot,SLOT(optFlow_CB(int,int)));
//    connect(&gh,SIGNAL(gateCallBack(int)),&robot,SLOT(gateSerial_CB(int)));
    connect(&ph,SIGNAL(connectorPkg(double,double)),&robot,SLOT(lineSerial_CB(double,double)));

  //  h_thread.start();
    qr_thread.start();
    ph_thread.start();
   // qDebug()<<"MW_T"<<QThread::currentThreadId();
//    cam_thread.start();
//    opt_thread.start();
    robot_thread.start();
    timer.start(20);

}

void MainWindow::process()
{
qDebug()<<"RUNN"<<opt_thread.isRunning();
}



MainWindow::~MainWindow()
{

}
