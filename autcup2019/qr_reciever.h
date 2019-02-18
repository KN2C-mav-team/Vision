#ifndef QR_RECIEVER_H
#define QR_RECIEVER_H

#include <QObject>
#include <includes.h>

class qr_reciever : public QObject
{
    Q_OBJECT
public:
    explicit qr_reciever(QObject *parent = 0);



private:

    void decoder(Mat &);
    void publish(const string , const string , const vector<Point> , int );

signals:
    void QR_signal(const string , const string , const vector<Point> , int );

public slots:
    void imageCallback(Mat raw_image);
};

#endif // QR_RECIEVER_H