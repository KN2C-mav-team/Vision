#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),&camera,SLOT(frontFrame_ready()));
    connect(timer,SIGNAL(timeout()),&camera,SLOT(downFrame_ready()));
    connect(&camera,SIGNAL(down_image(Mat)),&ph,SLOT(imageCallBack(Mat)));
    connect(&camera,SIGNAL(down_image(Mat)),&QR,SLOT(imageCallback(Mat)));
    connect(&QR,SIGNAL(QR_signal(string,string,vector<Point>,int)),&ph,SLOT(Callback(string,string,vector<Point>,int)));
    timer->start(2);


}

MainWindow::~MainWindow()
{

}
