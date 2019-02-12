#ifndef SPIN_H
#define SPIN_H

#include <QObject>
#include "qdebug.h"
#include "ros/ros.h"
#include <QtExtSerialPort/qextserialport.h>
#include <QThread>
#include <QUdpSocket>
#include <QDataStream>
#include <QTimer>




class Spin : public QThread
{
public:
  Spin();

  void run();

private:


};

#endif // SPIN_H
