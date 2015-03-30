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
#include "plot.h"

int main(int argc, const char * argv[]) {
    // Parameters
    double kappa = 2;
    double t_end = 1;
    double tolerance = 0.01;
    double dtMin = 0;
    int numFrames = 1800;
    
    int width = 1920;
    int height = 1080;
    int scale = 500;
    
    
    // Start
    std::mt19937_64 generator;
    generator.seed(2);
    std::vector<double> init(1, 0.0);
    std::vector<double> mid(1, -0.6);
    std::vector<double> end(1, 0.2);
    BrownianMotion B(init, generator);
    B.setValue(0.5, mid);
    B.setValue(1, end);
    SLE g(&B, kappa, t_end, tolerance, dtMin, numFrames);
    
    generateFrames(width, height, scale, g);
    
    
    return 0;
}
