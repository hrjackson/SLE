//
//  plot.cpp
//  SLE
//
//  Created by Henry Jackson on 29/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "plot.h"

////////////////////////////////////////////////////////////////////////////////
//// Plot member function definitions //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

plot::plot(int width, int height, int scale)
:width(width), height(height), scale(scale){
    originRe = (double)(width)/(double)(2*scale);
    originIm = 0.95*(double)(height)/(double)(scale);
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);
    cairo_scale (cr, scale, scale);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    
    // Make the background white
    cairo_rectangle(cr, 0, 0, (double)(width)/scale, (double)(height)/scale);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);
    
    // Draw the real axis
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.005);
    cairo_move_to(cr, 0.05*originRe, originIm);
    cairo_line_to(cr, 1.95*originRe, originIm);
    cairo_move_to(cr, originRe, originIm);
    cairo_line_to(cr, originRe, 1.01*originIm);
    cairo_stroke(cr);
}

plot::~plot(){
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void plot::drawLine(std::vector<std::complex<double>> points){
    if (points.size() != 0){
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width (cr, 0.002);
        std::complex<double> start = points.front();
        cairo_move_to(cr, start.real() + originRe, originIm - start.imag());
        for (auto it = ++points.begin(); it != points.end(); ++it) {
            cairo_line_to(cr, (*it).real() + originRe, originIm - (*it).imag());
        }
        cairo_stroke(cr);
    }
}

void plot::drawSLE(SLE& g, double time){
    std::vector<std::complex<double>> line = g.forwardLine(time);
    drawLine(line);
}

void plot::drawReverseSLE(SLE &g, double time){
    std::vector<std::complex<double>> line = g.reverseLine(time);
    drawLine(line);
}

void plot::drawUnCentredReverseSLE(SLE &g, double time){
    std::vector<std::complex<double>> line = g.unCentredReverseLine(time);
    drawLine(line);
}

void plot::output(const char* filename){
    cairo_surface_write_to_png(surface, filename);
}

////////////////////////////////////////////////////////////////////////////////
//// Generate frame funcion ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void generateFrames(int width, int height, int scale, SLE& g){
    
    // Get times for loop
    std::vector<double> times = g.FrameTimes();
    
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
