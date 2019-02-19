#include "lineEquations.h"

LineEquations::LineEquations(QObject *parent) : QObject(parent)
{

}
//finding pole line of detected line with having the center point
void LineEquations::find_pole_equation(const Point center,const double a,
                                       double & pole_a,double &pole_b){
    pole_a = (-1/a);
    pole_b = (center.y - (pole_a*center.x));
}


//finding detected line equation
void LineEquations::find_line_equation(const Point p1, const Point p2, double &a, double &b){
    double delta_x = p2.x - p1.x;
    double delta_y = p2.y - p1.y;
    a = delta_y / delta_x;
    b = (p1.y - (delta_y/delta_x)*p1.x);
}

Point LineEquations::find_twolines_collusion(const double afactor, const double bfactor,
                                             const double apolefactor, const double bpolefactor){
    Point cul;
    double a = afactor;
    double e = bfactor;
    double c = apolefactor;
    double f = bpolefactor;
    cul.x = (f-e)/(c-a);
    cul.y = (a*f - e*c)/(c-a);
    return cul;

}

double LineEquations::point_length(Point i1,Point i2){
    return (sqrt( (i1.x - i2.x) * (i1.x - i2.x)
                  +
                  (i1.y - i2.y) * (i1.y - i2.y)
                  ));
}


Point LineEquations::findMeanPoint(const vector<Point> points){
    Point mean;
    int mean_x = 0;
    int mean_y = 0;
    for(unsigned int i=0;i<points.size();i++){
        mean_x += points[i].x;
        mean_y += points[i].y;
    }
    mean.x =  mean_x / points.size();
    mean.y =  mean_y / points.size();
    return mean;
}


