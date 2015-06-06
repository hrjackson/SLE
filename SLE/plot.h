//
//  plot.h
//  SLE
//
//  Created by Henry Jackson on 29/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__plot__
#define __SLE__plot__

#include <stdio.h>
#include <vector>
#include <complex>
#include <string>
#include <sstream>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

typedef std::complex<double> cpx;

class plot{
private:
    Mat image;
    int width;
    int height;
    int scale;
    // origin is in matrix coords in matrix coordinates
    Point origin;
    Point currentPosition;
    double border;
public:
    plot(int width, int height, int scale, double border);
    ~plot();
    void drawLine(cpx point, Scalar colour);
    void drawLine(vector<cpx> points, Scalar colour);
    void output(const char* filename);
    void show();
    void clear();
    void drawAxis();
    Point cpxToCV (cpx z);
    cpx CVTocpx(Point pt);
    // Return the min/max x or y, in complex corrdinates
    double minX();
    double maxX();
    double maxY();
    int rows();
    int cols();
};

#endif /* defined(__SLE__plot__) */
