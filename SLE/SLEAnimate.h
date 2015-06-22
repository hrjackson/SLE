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
#include <iterator>
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
    /*--- CUDA data ---*/
    double* dt;
    double* shifts;
    
    /*--- general data ---*/
    // Line matrices
    cpx* horizontal;
    int horizontalRows;
    int horizontalCols;
    //cv::Mat_<cpx> horizontal;
    cv::Mat hzColour;
    cpx* vertical;
    int verticalRows;
    int verticalCols;
    //cv::Mat_<cpx> vertical;
    cv::Mat vtColour;
	// Height the lines in the grid go up to
    double lineHeight;
	// Width the lines in the grid go out to, dist from centre
    double lineWidth;
    // Background matrices
    cpx* pxOriginal;
    int pxOriginalRows;
    int pxOriginalCols;
    cpx* pxNow;
    //cv::Mat_<cpx> pxOriginal;
    //cv::Mat_<cpx> pxNow;
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
    cpx stabiliserReverse;
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
    cpx* generateHorizontal();
    cpx* generateVertical();
    //cv::Mat_<cpx> generatePixelPos();
    void initialiseLeft();
    // Convert a complex number in the "colour patch" to its
    // corresponding colour
    Vec3b cpxToColour(cpx z, bool shader);
    // Convert a whole matrix of points to their corresponding colours
    Mat generateColours(cpx* points, int pointsRows, int pointsCols, bool shader);
    // Draw lines corresponding to ROWS in the matrix.
    void drawLines(plot& plot,
                   cpx* matrix,
                   int matrixRows,
                   int matrixCols,
                   Mat& colours);
    void drawLinesTranspose(plot& plot,
                   cpx* matrix,
                   int matrixRows,
                   int matrixCols,
                   Mat& colours);
    void drawColours(plot& plot,
                     cpx* points,
                     int pointsRows,
                     int pointsCols);
    void updateMatrixForward(int start, int end,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
    void updateValueForward(int start, int end,
                            cpx inValue,
                            cpx& outValue);
    cpx slitMapInverse(int index, cpx inValue);
    
    void updateMatrixForward(SlitMap& h,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
    void updateMatrixForward(vector<SlitMap>& h,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
    
    void updateMatrixReverse(int start, int end,
                             double offset,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
    void updateValueReverse(int start, int end,
                            double offset,
                            cpx inValue,
                            cpx& outValue);
    cpx slitMap(int index, cpx inValue);
    
    
    void updateMatrixReverse(SlitMap& h,
                             double offset,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
    void updateMatrixReverse(vector<SlitMap>& h,
                             double offset,
                             cpx* inMat,
                             cpx* outMat,
                             int rows,
                             int cols);
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
    ~SLEAnimate();
};

#endif /* defined(__SLE__SLEAnimate__) */
