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
#include "sle_process.h"

using namespace cv;

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
    Point cpxToCV (std::complex<double> z);
public:
    plot(int width, int height, int scale, double border);
    ~plot();
    void drawLine(std::complex<double> point, Scalar colour);
    void drawLine(std::vector<std::complex<double>> points, Scalar colour);
    void drawSLE(SLE& g, double time);
    void drawReverseSLE(SLE& g, double time);
    void drawUnCentredReverseSLE(SLE& g, double time);
    void output(const char* filename);
    void show();
};

void generateFrames(int width, int height, int scale, SLE& g);

#endif /* defined(__SLE__plot__) */
