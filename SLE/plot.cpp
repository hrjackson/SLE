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
    originIm = (double)(height)/(double)(scale);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
}

void plot::drawLine(std::vector<std::complex<double>> points){
    std::complex<double> start = points.front();
    cairo_move_to(cr, start.real() + originRe, originIm - start.imag());
    for (auto it = ++points.begin(); it != points.end(); ++it) {
        cairo_line_to(cr, (*it).real() + originRe, originIm - (*it).imag());
    }
}

void plot::drawSLE(SLE& g, double time){
    std::vector<std::complex<double>> line = g.forwardLine(time);
    this->plot::drawLine(line);
}

void plot::drawReverseSLE(SLE &g, double time){
    std::vector<std::complex<double>> line = g.reverseLine(time);
    this->plot::drawLine(line);
}

void plot::output(const char* filename){
    cairo_stroke(cr);
    cairo_surface_write_to_png(surface, filename);
}