#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <capture.h>
#include <QTimer>
#include <includes.h>




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0 );
    ~MainWindow();
    capture camera;
    QTimer *timer;
    ProgramHandeler ph;
    qr_reciever QR;

};

#endif // MAINWINDOW_H
