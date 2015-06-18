//
//  SLEAnimate.cpp
//  SLE
//
//  Created by Henry Jackson on 22/04/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "SLEAnimate.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
//// SLEAnimate private member function definitions ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void SLEAnimate::initialiseLeft(){
    drawColours(leftPlot, pxOriginal);
        drawLines(leftPlot, horizontal, hzColour);
    
    cv::Mat_<cpx> tmp = vertical.t();
    Mat tmpColour = vtColour.t();
    drawLines(leftPlot, tmp, tmpColour);
    
    leftPlot.drawAxis();
}

void SLEAnimate::drawLines(class plot& plot, cv::Mat_<cpx>& matrix, Mat& colours){
    double offset = -stabilser.real();
    //double offset = 0.0;
    int rows = matrix.rows;
    int cols = matrix.cols;
    for (int i=0; i<rows; ++i) {
        vector<cpx> line;
        for (int j=0; j<cols-1; ++j) {
            line = vector<cpx>{matrix(i,j) + offset, matrix(i,j+1) + offset};
            Vec3b col = colours.at<Vec3b>(i,j);
            Scalar colScalar = Scalar( (double)col[0], (double)col[1], (double)col[2] );
            plot.drawLine(line, colScalar);
        }
    }
    // Draws the little blue thing at the bottom of the screen
    vector<cpx> offsetLine;
    offsetLine.push_back(offset - cpx(0, gridSpacing/2));
    offsetLine.push_back(offset + cpx(0, gridSpacing/2));
    plot.drawLine(offsetLine, Scalar(255,0,0));
}

void SLEAnimate::drawColours(class plot& plot, Mat_<cpx>& points){
    int rows = points.rows;
    int cols = points.cols;
    Mat colours = generateColours(points, false);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j){
            plot.colour(i, j, colours.at<Vec3b>(i,j) );
        }
    }
}

cv::Mat_<cpx> SLEAnimate::generateHorizontal(){

    double width = 2*(leftPlot.maxX() - leftPlot.minX());
    double height = 2*leftPlot.maxY();
    int rows = (int)(height/gridSpacing);
    int cols = width/gridRes;
    cv::Mat_<cpx> result(rows, cols);
    cpx heightIncrement(0, gridSpacing);
    cpx widthIncrement(gridRes, 0);
    
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            result(i,j) = heightIncrement*(double)(i+1)
                          + widthIncrement*(double)j - width/2;
        }
    }
    return result;
}

cv::Mat_<cpx> SLEAnimate::generateVertical() {
    double width = 2*(leftPlot.maxX() - leftPlot.minX());
    double height = 2*leftPlot.maxY();
    int rows = (int)(height/gridRes) + 1;
    int cols = width/gridSpacing + 2;
    cv::Mat_<cpx> result(rows, cols);
    cpx heightIncrement(0, gridRes);
    cpx widthIncrement(gridSpacing, 0);
    
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            result(i,j) = heightIncrement*(double)i
                          + widthIncrement*(double)j - width/2;
        }
    }
    return result;
}

/*
cv::Mat_<cpx> SLEAnimate::generatePixelPos() {
    int rows = rightPlot.rows();
    int cols = rightPlot.cols();
    cv::Mat_<cpx> result( rows, cols );
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            result(i,j) = rightPlot.CVTocpx(Point(i,j));
        }
    }
    return result;
}
*/

Vec3b SLEAnimate::cpxToColour(cpx z, bool shader) {
    Vec3b result;
    double x = z.real();
    double y = z.imag();
    
    // The height of the point, relative to the maximum
    double yProp = y/lineHeight;
    if (yProp < 0) yProp = 0;
    int yCV = int((darkRows-1)*(1-yProp));
    
    // Similar for x, but x can be negative
    double xProp = (x/lineWidth + 1)/2;
	if (xProp > 1) xProp = 1;
    int xCV = int((darkCols-1)*xProp);
    
    if (shader) {
        result = dark.at<Vec3b>(yCV, xCV);
    } else {
        result = light.at<Vec3b>(yCV, xCV);
    }
    return result;
}

Mat SLEAnimate::generateColours(Mat_<cpx>& points, bool shader) {
    Mat result = Mat::Mat(points.rows, points.cols, CV_8UC3, Scalar(255,255,255));
    for (int i = 0; i < points.rows; ++i){
        for (int j = 0; j < points.cols; ++j) {
            result.at<Vec3b>(i,j) = cpxToColour(points.at<cpx>(i,j), shader);
        }
    }
    return result;
}

void SLEAnimate::updateMatrixForward(SlitMap& h,
                                     Mat_<cpx>& inMat,
                                     Mat_<cpx>& outMat){
    for (auto in = inMat.begin(), out = outMat.begin();
         in != inMat.end();
         ++in, ++out) {
        *out = h.inverse((*in));
    }
}

void SLEAnimate::updateMatrixForward(vector<SlitMap>& h,
                                     Mat_<cpx>& inMat,
                                     Mat_<cpx>& outMat){
    for (auto it = h.begin(); it != h.end(); ++it) {
        updateMatrixForward(*it, inMat, outMat);
    }
    
}

void SLEAnimate::updateMatrixReverse(SlitMap& h,
                                     Mat_<cpx>& inMat,
                                     Mat_<cpx>& outMat){
    for (auto in = inMat.begin(), out = outMat.begin();
         in != inMat.end();
         ++in, ++out) {
        *out = h((*in));
    }
}

void SLEAnimate::updateMatrixReverse(vector<SlitMap>& h,
                                     Mat_<cpx>& inMat,
                                     Mat_<cpx>& outMat){
    auto it = h.rbegin();
    updateMatrixReverse(*it, inMat, outMat);
    ++it;
    while (it != h.rend()) {
        //cout << inMat.at<cpx>(10,10) << " " << outMat.at<cpx>(10,10) << endl;
        updateMatrixReverse( *it, outMat, outMat);
        ++it;
        //cout << inMat.at<cpx>(10,10) << " " << outMat.at<cpx>(10,10) << endl;
    }
    
}

void SLEAnimate::plot() {
    rightPlot.clear();
    drawColours(rightPlot, pxNow);
    drawLines(rightPlot, horizontal, hzColour);
    cv::Mat_<cpx> tmp = vertical.t();
    Mat tmpColour = vtColour.t();
    drawLines(rightPlot, tmp, tmpColour);
    if (currentTime > 0) {
        leftPlot.drawLine(g.forwardLine( *(--frameTimes.lower_bound(currentTime)),
                                        currentTime),
                          Scalar(255,0,0));
    }
    rightPlot.drawAxis();
}

////////////////////////////////////////////////////////////////////////////////
//// SLEAnimate member function definitions ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SLEAnimate::SLEAnimate(double gridRes,
                       double gridSpacing,
                       SLE& g,
                       class plot& left,
                       class plot& right)
:gridRes(gridRes), gridSpacing(gridSpacing),
g(g), leftPlot(left), rightPlot(right) {
    // Import the colour matrices
    //dark = imread("D:\\sleOutput\\col\\dark.png", CV_LOAD_IMAGE_COLOR);
    dark = imread("/Users/Henry/tmp/colours/dark.png", CV_LOAD_IMAGE_COLOR);
    light = imread("/Users/Henry/tmp/colours/light.png", CV_LOAD_IMAGE_COLOR);
    darkRows = dark.rows;
    darkCols = dark.cols;
    lightRows = light.rows;
    lightCols = light.cols;
    
    // Initialise the line matrices
    lineHeight = 2*leftPlot.maxY();
    lineWidth = 2*leftPlot.maxX();
    horizontal = generateHorizontal();
    hzColour = generateColours(horizontal, true);
    vertical = generateVertical();
    vtColour = generateColours(vertical, true);
    
    // Initialise pixel position matrix
    pxOriginal = leftPlot.points();
    //cout << pxOriginal << endl;
    pxNow = leftPlot.points();
    
    // Initialise the stabilisation point to somewhere far away
    // on the imaginary axis
    stabilser = cpx(0, 1000);
    stabiliserReverse = cpx(0, 1000);
    // Initialise the pixel matrix
    //pixelPos = generatePixelPos();
    // Initialise the times
    times = g.getOrderedTimes();
    frameTimes = g.orderedFrameTimes();
    currentTime = 0;
    initialiseLeft();
    plot();
}

bool SLEAnimate::nextFrame() {
    auto nextTimePtr = frameTimes.upper_bound(currentTime);
    if (nextTimePtr != frameTimes.end() ) {
        double nextTime = *nextTimePtr;
        vector<SlitMap> gridMaps;
        vector<SlitMap> pixelMaps;
        SlitMap h = g.slitMap(0);
        for (auto it = times.lower_bound(currentTime); *it < nextTime; ++it) {
            h = g.slitMap(*it);
            gridMaps.push_back(h);
            stabilser = h.inverse(stabilser);
            stabiliserReverse = h(stabiliserReverse);
        }
        for (auto it = times.begin(); *it < nextTime; ++it) {
            h = g.slitMap(*it);
            pixelMaps.push_back(h);
        }
        
        updateMatrixForward(gridMaps, horizontal, horizontal);
        updateMatrixForward(gridMaps, vertical, vertical);
        
        double offset = -stabilser.real();
        Mat_<cpx> pxOffset = pxOriginal - offset;
        
        updateMatrixReverse(pixelMaps, pxOffset, pxNow);
        
        currentTime = nextTime;
        plot();
        return true;
    }
    else {
        return false;
    }
}


void SLEAnimate::show() {
    leftPlot.show();
    rightPlot.show();
}

void SLEAnimate::output(int frame) {

    // Set up filenames
    //std::string strLeft = "D:\\sleOutput\\left\\";
    //std::string strRight = "D:\\sleOutput\\right\\";
    std::string strLeft = "/Users/henry/tmp/left/";
    std::string strRight = "/Users/henry/tmp/right/";
    std::string ltName;
    std::string rtName;
    std::stringstream ss;
    
    ss << std::setfill('0') << std::setw(4);
    ss << frame;
    ltName = strLeft + ss.str() + ".png";
    rtName = strRight + ss.str() + ".png";
    ss.str(std::string());
    ss.clear();
    
    leftPlot.output(ltName.c_str());
    rightPlot.output(rtName.c_str());
}
































