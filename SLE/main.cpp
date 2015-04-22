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

#include "stochastic_processes.h"
#include "sle_process.h"
#include "plot.h"

int main(int argc, const char * argv[]) {
    // Parameters
    double kappa = 6;
    double t_end = 1.0;
    double tolerance = 0.005;
    double dtMin = 0.0000000000001;
    int numFrames = 10;
    
    int width = 2880;
    int height = 1800;
    int scale = height/2;
    double border = 0.03;
    
    
    // Start
    std::mt19937_64 generator;
    generator.seed(5);
    std::vector<double> init(1, 0.0);
    std::vector<double> mid(1, -0.6);
    std::vector<double> end(1, 0.2);
    BrownianMotion B(init, generator);
    
    std::cout.precision(40);
    
    SLE g(&B, kappa, t_end, tolerance, dtMin, numFrames);
    
    plot pl(width, height, scale, border);
    pl.drawSLE(g, 1.0);
    //pl.show();
    pl.output("output.jpg");
    //generateFrames(width, height, scale, g);
    
    
    return 0;
}
