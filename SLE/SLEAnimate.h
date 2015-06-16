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
using namespace cv;

typedef complex<double> cpx;

class SLEAnimate {
private:
    // Line matrices
    cv::Mat_<cpx> horizontal;
    cv::Mat hzColour;
    cv::Mat_<cpx> vertical;
    cv::Mat vtColour;
    double lineHeight;
    double lineWidth;
    // Background matrices
    cv::Mat_<cpx> pxOriginal;
    cv::Mat_<cpx> pxNow;
    // Colour matrices
    cv::Mat dark;
    int darkRows;
    int darkCols;
    //cv::Mat light;
    // Pixel matrix: holds the original position of the pixels
    cv::Mat_<cpx> pixelPos;
    // Stabilisation point. Should stay in roughly the same place,
    // so we can use its real part to offset the plots
    cpx stabilser;
    // Dealing with times
    double currentTime;
    set<double> times;
    set<double> frameTimes;
    double gridRes;
    double gridSpacing;
    SLE& g;
    plot &leftPlot;
    plot &rightPlot;
	/*---- Private functions ----*/
    cv::Mat_<cpx> generateHorizontal();
    cv::Mat_<cpx> generateVertical();
    cv::Mat_<cpx> generatePixelPos();
    void initialiseLeft();
    // Convert a complex number in the "colour patch" to its
    // corresponding colour
    Vec3b cpxToColour(cpx z);
    // Convert a whole matrix of points to their corresponding colours
    Mat generateColours(Mat_<cpx>& points);
    // Draw lines corresponding to ROWS in the matrix.
    void drawLines(plot& plot, cv::Mat_<cpx>& matrix, Mat& colours);
    void timeUpdate(double time);
    void updateMatrixForward(SlitMap& h, cv::Mat_<cpx>& matrix);
    void updateMatrixReverse(SlitMap& h, cv::Mat_<cpx>& matrix);
    void plot();
public:
    SLEAnimate(double gridRes,
               double gridSpacing,
               SLE& g,
               class plot& left, class plot& right);
    bool nextFrame();
    void show();
    void output(int frame);
	void plotForward();
};

#endif /* defined(__SLE__SLEAnimate__) */
