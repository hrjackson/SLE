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
#include <set>

#include "sle_process.h"
#include "plot.h"

using namespace std;

typedef complex<double> cpx;

class SLEAnimate {
private:
    // Line matrices
    cv::Mat_<cpx> horizontal;
    cv::Mat_<cpx> hzOriginalPos;
    cv::Mat_<bool> hzCut;
    cv::Mat_<cpx> vertical;
    cv::Mat_<cpx> vtOriginalPos;
    cv::Mat_<bool> vtCut;
    // Pixel matrix
    cv::Mat_<cpx> pixelPos;
    // Dealing with times
    double currentTime;
    set<double> times;
    set<double> frameTimes;
    double gridRes;
    double gridSpacing;
    SLE& g;
    plot &leftPlot;
    plot &rightPlot;
    cv::Mat_<cpx> generateHorizontal();
    cv::Mat_<cpx> generateVertical();
    cv::Mat_<cpx> generatePixelPos();
    /*---- Private functions ----*/
    void initialiseLeft();
    // Draw lines corresponding to ROWS in the matrix.
    void drawLines(plot& plot, cv::Mat_<cpx>& matrix);
    void timeUpdate(double time);
    void updateMatrixForward(SlitMap& h, double offset, cv::Mat_<cpx>& matrix);
    void updateMatrixReverse(SlitMap& h, double offset, cv::Mat_<cpx>& matrix);
    void plot();
public:
    SLEAnimate(double gridRes,
               double gridSpacing,
               SLE& g,
               class plot& left, class plot& right);
    bool nextFrame();
    void show();
};

#endif /* defined(__SLE__SLEAnimate__) */
