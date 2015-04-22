//
//  plot.cpp
//  SLE
//
//  Created by Henry Jackson on 29/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "plot.h"

////////////////////////////////////////////////////////////////////////////////
//// Plot private member function definitions //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//// Plot member function definitions //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

plot::plot(int width, int height, int scale, double border)
:width(width), height(height), scale(scale), border(border) {
    origin = Point(width/2, (1-border)*(double)height);
    int thickness = 2;
    
    // Initialise the plot
    image =  Mat::Mat(height, width, CV_8UC3, Scalar(255,255,255));
    
    // Draw the real axis
    cv::line(image,
             Point( border*width, (1-border)*height ),
             Point( (1-border)*width, (1-border)* height),
             Scalar(0,0,0),
             thickness);
    // And the little mark at the origin
    cv::line(image,
             origin,
             Point(width/2, (1-2.0*border/3.0)*(double)height),
             Scalar(0,0,0),
             thickness);
    // And at +- 1
    Point mOne = cpxToCV(cpx(-1,0));
    Point pOne = cpxToCV(cpx(1,0));
    cv::line(image,
             mOne,
             Point(mOne.x, (1-3.0*border/4.0)*height),
             Scalar(0,0,0),
             thickness);
    cv::line(image,
             pOne,
             Point(pOne.x, (1-3.0*border/4.0)*height),
             Scalar(0,0,0),
             thickness);
}

plot::~plot(){

}

void plot::drawLine(cpx point, Scalar colour) {
    Point endPoint = cpxToCV(point);
    line(image,
         currentPosition,
         endPoint,
         colour,
         1,
         CV_AA);
    currentPosition = endPoint;
}

void plot::drawLine(vector<cpx> points, Scalar colour=Scalar(0,0,0)){
    if (points.size() != 0){
        currentPosition = cpxToCV(points.front());
        for (auto it = ++points.begin(); it != points.end(); ++it) {
            drawLine((*it), colour);
        }
    }
}

void plot::drawSLE(SLE& g, double time){
    vector<cpx> line = g.forwardLine(time);
    drawLine(line);
}

void plot::drawReverseSLE(SLE &g, double time){
    vector<cpx> line = g.reverseLine(time);
    drawLine(line);
}

void plot::drawUnCentredReverseSLE(SLE &g, double time){
    vector<cpx> line = g.unCentredReverseLine(time);
    drawLine(line);
}

void plot::show(){
    cv::imshow("window", image);
    cv::waitKey();
}

void plot::output(const char* filename) {
    cv::imwrite(filename, image);
}

Point plot::cpxToCV(cpx z) {
    Point result = Point(origin.x + z.real()*scale, origin.y - z.imag()*scale);
    return result;
}

cpx plot::CVTocpx(Point pt) {
    cpx result = cpx( ( (double)pt.x - (double)origin.x )/scale,
                      ( (double)origin.y - (double)pt.y )/scale );
    return result;
}

double plot::minX(){
    return (double)(origin.x - width)/(double)scale;
}

double plot::maxX(){
    return -minX();
}

double plot::maxY(){
    return (double)(origin.y)/(double)scale;
}

int plot::rows(){
    return height;
}

int plot::cols(){
    return width;
}



////////////////////////////////////////////////////////////////////////////////
//// Generate frame funcion ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
void generateFrames(int width, int height, int scale, SLE& g){
    
    // Get times for loop
    vector<double> times = g.FrameTimes();
    
    // Set up filenames
    std::string strForward = "./forward/";
    //std::string strReverse = "./reverseUN/";
    std::string frdName;
    //std::string rvsName;
    std::stringstream ss;
    
    int frame = 0;
    
    for (auto it = times.begin(); it != times.end(); ++it) {
        
        ss << std::setfill('0') << std::setw(4);
        ss << frame++;
        frdName = strForward + ss.str() + ".png";
        //rvsName = strReverse + ss.str() + ".png";
        ss.str(std::string());
        ss.clear();
        
        
        plot forward(width, height, scale);
        //plot reverse(width, height, scale);
        
        forward.drawSLE(g, *it);
        //reverse.drawUnCentredReverseSLE(g, *it);
        
        forward.output(frdName.c_str());
        //reverse.output(rvsName.c_str());
    }
}
*/