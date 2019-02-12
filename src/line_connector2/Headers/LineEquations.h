#ifndef LIQ_H
#define LIQ_H

#include "Headers/includes.h"
using namespace cv;

class LineEquations{

public:
    LineEquations(){
    }
    void find_pole_equation(const Point ,const double ,
                            double & ,double &);
    void find_line_equation(const Point , const Point , double &, double &);
    Point find_twolines_collusion(const double afactor, const double bfactor,
                                 const double apolefactor, const double bpolefactor);
    double point_length(Point i1,Point i2);
private:


};

#endif
