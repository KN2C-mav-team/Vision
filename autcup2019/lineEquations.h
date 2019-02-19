#ifndef LINEEQUATIONS_H
#define LINEEQUATIONS_H

#include <QObject>
#include <includes.h>

using namespace cv;
using namespace std;

class LineEquations : public QObject
{
    Q_OBJECT
public:
    explicit LineEquations(QObject *parent = 0);

    void find_pole_equation(const Point ,const double ,
                            double & ,double &);
    void find_line_equation(const Point , const Point , double &, double &);
    Point find_twolines_collusion(const double afactor, const double bfactor,
                                  const double apolefactor, const double bpolefactor);
    double point_length(Point i1,Point i2);
    Point findMeanPoint(const vector<Point>);

signals:

public slots:
};

#endif // LINEEQUATIONS_H
