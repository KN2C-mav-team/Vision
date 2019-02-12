#include <include/spin.h>

Spin::Spin() : QThread()
{
    qDebug() << "Thread started!";
}

void Spin::run()
{
    qDebug() << "run!";
    ros::spin();

}
