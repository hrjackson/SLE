//
//  main.cpp
//  SLE
//
//  Created by Henry Jackson on 10/02/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include <iostream>
#include <vector>
#include <complex>

#include "cairo.h"
#include "stochastic_processes.h"
#include "sle_process.h"

int main(int argc, const char * argv[]) {
    // Parameters
    double kappa = 1;
    double t_end = 1;
    double tolerance = 0.01;
    
    
    // Start
    std::vector<double> init(1, 0.0);
    BrownianMotion B(init);
    SLE g(&B, kappa, t_end, tolerance);
    
    std::vector<std::complex<double>> curve = g.getCurve();
    
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2000, 1000);
    cr = cairo_create(surface);
    cairo_scale (cr, 500, 500);
    
    
    //cairo_rectangle(cr, 0.5, 0.5, 0.25, 0.25);
    //cairo_stroke(cr);
    
    // Make the background white
    cairo_rectangle(cr, 0, 0, 4, 2);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);
    

    // Draw in the SLE
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width (cr, 0.01);

    cairo_move_to (cr, 2, 2);
    for (int i = 1; i < curve.size(); i++) {
        cairo_line_to(cr, curve[i].real()+2, 2-curve[i].imag());
    }
    cairo_stroke(cr);
    
    /*
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 40.0);
    
    cairo_move_to(cr, 50, 50);
    cairo_show_text(cr, "Hello GitHub!");
    */
    
    // Write output and clean up
    cairo_surface_write_to_png(surface, "image.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    
    return 0;
}
