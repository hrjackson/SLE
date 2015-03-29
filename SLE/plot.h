//
//  plot.h
//  SLE
//
//  Created by Henry Jackson on 29/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__plot__
#define __SLE__plot__

#include <stdio.h>
#include <vector>
#include <complex>

#include "cairo.h"
#include "sle_process.h"

class plot{
private:
    int width;
    int height;
    int scale;
    double originRe;
    double originIm;
    cairo_surface_t *surface;
    cairo_t *cr;
public:
    plot(int width, int height, int scale);
    void drawLine(std::vector<std::complex<double>> points);
    void drawSLE(SLE& g, double time);
    void drawReverseSLE(SLE& g, double time);
    void output(const char* filename);
};

#endif /* defined(__SLE__plot__) */
