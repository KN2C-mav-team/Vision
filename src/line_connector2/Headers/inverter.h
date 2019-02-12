#ifndef INV_H
#define INV_H

#include "includes.h"

using namespace cv;

class Inverter{
private:

public:
    Inverter();
    void invert_channels(Mat *,Mat *);
    void invert_first_channel(Mat *,Mat *);
    void invert_second_channel(Mat *,Mat *);
    void invert_third_channel(Mat *,Mat *);
};


#endif
