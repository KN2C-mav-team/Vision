#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <capture.h>
#include <QTimer>
#include <includes.h>
#include <qthread.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0 );
    ~MainWindow();
    capture camera;
    QTimer *timer;
    QTimer *timer2;
    ProgramHandeler ph;
    Handler gh;//gate handler
    qr_reciever QR;
    OptFlow opt;
    Quad_Board robot;
    LineEquations line;
    QThread *cam_thread;
    QThread *line_thread;
    QThread *h_thread;
    QThread *qr_thread;
    QThread *ph_thread;
    QThread *opt_thread;
    QThread *robot_thread;

};

#endif // MAINWINDOW_H
