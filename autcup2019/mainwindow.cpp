#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),&camera,SLOT(frontFrame_ready()));
    connect(timer,SIGNAL(timeout()),&camera,SLOT(downFrame_ready()));
    connect(&camera,SIGNAL(down_image(Mat)),&ph,SLOT(imageCallBack(Mat)));
    connect(&camera,SIGNAL(down_image(Mat)),&QR,SLOT(imageCallback(Mat)));
    connect(&camera,SIGNAL(down_image(Mat)),&opt,SLOT(timerEvent(Mat)));
    connect(&QR,SIGNAL(QR_signal(string,string,vector<Point>,int)),&ph,SLOT(Callback(string,string,vector<Point>,int)));
    connect(&ph,SIGNAL(connectorPkg(double,double)),&robot,SLOT(lineSerial_CB(double,double)));
    connect(&opt,SIGNAL(optSig(int,int)),&robot,SLOT(optFlow_CB(int,int));
    timer->start(2);


}

MainWindow::~MainWindow()
{

}
