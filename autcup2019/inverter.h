#ifndef INVERTER_H
#define INVERTER_H

#include <QObject>
#include "includes.h"

using namespace cv;

class inverter : public QObject
{
    Q_OBJECT
public:
    explicit inverter(QObject *parent = 0);

    void invert_channels(Mat *,Mat *);
    void invert_first_channel(Mat *,Mat *);
    void invert_second_channel(Mat *,Mat *);
    void invert_third_channel(Mat *,Mat *);

signals:

public slots:
};

#endif // INVERTER_H
