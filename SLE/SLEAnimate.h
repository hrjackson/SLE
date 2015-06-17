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
	// Height the lines in the grid go up to
    double lineHeight;
	// Width the lines in the grid go out to, dist from centre
    double lineWidth;
    // Background matrices
    cv::Mat_<cpx> pxOriginal;
    cv::Mat_<cpx> pxNow;
    // Colour matrices
    cv::Mat dark;
    int darkRows;
    int darkCols;
    Mat light;
    int lightRows;
    int lightCols;
    //cv::Mat light;
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
    //cv::Mat_<cpx> generatePixelPos();
    void initialiseLeft();
    // Convert a complex number in the "colour patch" to its
    // corresponding colour
    Vec3b cpxToColour(cpx z, bool shader);
    // Convert a whole matrix of points to their corresponding colours
    Mat generateColours(Mat_<cpx>& points, bool shader);
    // Draw lines corresponding to ROWS in the matrix.
    void drawLines(plot& plot, cv::Mat_<cpx>& matrix, Mat& colours);
    void drawColours(plot& plot, Mat_<cpx>& points);
    void timeUpdate(double time);
    void updateMatrixForward(SlitMap& h,
                             Mat_<cpx>& inMat,
                             Mat_<cpx>& outMat);
    void updateMatrixForward(vector<SlitMap>& h,
                             Mat_<cpx>& inMat,
                             Mat_<cpx>& outMat);
    void updateMatrixReverse(SlitMap& h,
                             Mat_<cpx>& inMat,
                             Mat_<cpx>& outMat);
    void updateMatrixReverse(vector<SlitMap>& h,
                             Mat_<cpx>& inMat,
                             Mat_<cpx>& outMat);
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
