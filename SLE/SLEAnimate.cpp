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
    leftPlot.drawAxis();
    drawLines(leftPlot, horizontal);
    
    cv::Mat_<cpx> tmp = vertical.t();
    drawLines(leftPlot, tmp);
}
// TODO !!!
void SLEAnimate::drawLines(class plot& plot, cv::Mat_<cpx>& matrix){
    double offset = -stabilser.real();
    int rows = matrix.rows;
    int cols = matrix.cols;
    for (int i=0; i<rows; ++i) {
        vector<cpx> line;
        for (int j=0; j<cols; ++j) {
            line.push_back( matrix(i,j) + offset );
        }
        plot.drawLine(line, Scalar(200,200,200));
    }
    vector<cpx> offsetLine;
    offsetLine.push_back(offset - cpx(0, gridSpacing/2));
    offsetLine.push_back(offset + cpx(0, gridSpacing/2));
    plot.drawLine(offsetLine, Scalar(255,0,0));
}

cv::Mat_<cpx> SLEAnimate::generateHorizontal(){

    double width = 2*(leftPlot.maxX() - leftPlot.minX());
    double height = 2*leftPlot.maxY();
    int rows = (int)(height/gridSpacing) + 1;
    int cols = width/gridRes + 1;
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

void SLEAnimate::timeUpdate(double time){
    //cout << "time = " << time << endl;
    SlitMap h = g.slitMap(time);
    stabilser = h.inverse(stabilser);
    // Update horizontal and vertical matrices
    updateMatrixForward(h, horizontal);
    updateMatrixForward(h, vertical);
    // And the pixels, in the reverse direction
    //updateMatrixReverse(h, offset, pixelPos);
}

void SLEAnimate::updateMatrixForward(SlitMap& h,
                                     cv::Mat_<cpx>& matrix){
    for (auto it = matrix.begin(); it != matrix.end(); ++it) {
        *it = h.inverse((*it));
    }
}

void SLEAnimate::updateMatrixReverse(SlitMap& h,
                                     cv::Mat_<cpx>& matrix){
    for (auto it = matrix.begin(); it != matrix.end(); ++it) {
        *it = h((*it));
    }
}


void SLEAnimate::plot() {
    rightPlot.clear();
    drawLines(rightPlot, horizontal);
    cv::Mat_<cpx> tmp = vertical.t();
    drawLines(rightPlot, tmp);
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
    // Initialise the line matrices
    horizontal = generateHorizontal();
    //cout << "The horizontal matrix:" << endl;
    //cout << horizontal << endl;
    hzOriginalPos = horizontal;
    vertical = generateVertical();
    //cout << "The vertical matrix:" << endl;
    //cout << vertical << endl;
    vtOriginalPos = vertical;
    // Initialise the stabilisation point to somewhere far away
    // on the imaginary axis
    stabilser = cpx(0, 50);
    // Initialise the pixel matrix
    pixelPos = generatePixelPos();
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
        for (auto it = times.lower_bound(currentTime); *it < nextTime; ++it) {
            timeUpdate(*it);
        }
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
    leftPlot.output("left.jpg");
    rightPlot.output("right.jpg");
    
    
    // Get times for loop
    std::vector<double> times = g.FrameTimes();
    
    // Set up filenames
    std::string strLeft = "D:\\sleOutput\\left\\";
    std::string strRight = "D:\\sleOutput\\right\\";
    std::string ltName;
    std::string rtName;
    std::stringstream ss;
    
    ss << std::setfill('0') << std::setw(4);
    ss << frame;
    ltName = strLeft + ss.str() + ".jpg";
    rtName = strRight + ss.str() + ".jpg";
    ss.str(std::string());
    ss.clear();
    
    leftPlot.output(ltName.c_str());
    rightPlot.output(rtName.c_str());
}
































