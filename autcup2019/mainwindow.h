#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <capture.h>
#include <QTimer>
#include <includes.h>
//#include <QThread.h>

Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(string)
Q_DECLARE_METATYPE(std::vector<cv::Point>)


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0 );
    ~MainWindow();
    capture camera;
    QTimer timer;
    ProgramHandeler ph;
    Handler gh;//gate handler
    qr_reciever QR;
    OptFlow opt;
    Quad_Board robot;
    LineEquations line;
    HDetection landingPad;
    QThread cam_thread;
    QThread line_thread;
    QThread h_thread;
    QThread qr_thread;
    QThread ph_thread;
    QThread opt_thread;
    QThread robot_thread;
    QThread hLand_thread;
public slots:

void process();


};

#endif // MAINWINDOW_H

