#include "inverter.h"

inverter::inverter(QObject *parent) : QObject(parent)
{

}

void inverter::invert_channels(Mat *src, Mat *dst){
    vector<Mat> channels;
    split(*src,channels);
    for(int i=0;i<channels.size();i++){
        bitwise_not(channels[i],channels[i]);
    }
    merge(channels,*dst);
}

void inverter::invert_first_channel(Mat *src, Mat *dst){
    vector<Mat> channels;
    split(*src,channels);
    bitwise_not(channels[0],channels[0]);
    merge(channels,*dst);
}

void inverter::invert_second_channel(Mat *src, Mat *dst){
    vector<Mat> channels;
    split(*src,channels);
    bitwise_not(channels[1],channels[1]);
    merge(channels,*dst);
}

void inverter::invert_third_channel(Mat *src, Mat *dst){
    vector<Mat> channels;
    split(*src,channels);
    bitwise_not(channels[2],channels[2]);
    merge(channels,*dst);
}
