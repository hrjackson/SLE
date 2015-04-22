//
//  SLEAnimate.h
//  SLE
//
//  Created by Henry Jackson on 22/04/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__SLEAnimate__
#define __SLE__SLEAnimate__

#include <stdio.h>
#include <vector>
#include <complex>

#include "sle_process.h"
#include "plot.h"

using namespace std;

typedef complex<double> cpx;

class SLEAnimate {
private:
    cv::Mat_<cpx> horizontal;
    cv::Mat_<cpx> hzOriginalPos;
    cv::Mat_<bool> hzCut;
    cv::Mat_<cpx> vertical;
    cv::Mat_<cpx> vtOriginalPos;
    cv::Mat_<bool> vtCut;
    cv::Mat_<cpx> pixelPos;
    double currentTime;
    double gridRes;
    double gridSpacing;
    SLE& g;
    plot &leftPlot;
    plot &rightPlot;
    cv::Mat_<cpx> generateHorizontal();
    cv::Mat_<cpx> generateVertical();
    cv::Mat_<cpx> generatePixelPos();
public:
    SLEAnimate(double gridRes,
               double gridSpacing,
               SLE& g,
               plot& left, plot& right);
    void nextFrame();
    void plot();
    void show();
};

#endif /* defined(__SLE__SLEAnimate__) */
