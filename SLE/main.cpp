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
#include <string>
#include <sstream>
#include <iomanip>

#include "cairo.h"
#include "stochastic_processes.h"
#include "sle_process.h"
#include "plot.h"

int main(int argc, const char * argv[]) {
    // Parameters
    double kappa = 2;
    double t_end = 1;
    double tolerance = 0.02;
    double dtMin = 0;
    int numFrames = 31;
    
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
    
    // Get times for loop
    std::vector<double> times = g.FrameTimes();
    
    // Set up filenames
    std::string strForward = "./forward/";
    std::string strReverse = "./reverse/";
    std::string frdName;
    std::string rvsName;
    std::stringstream ss;
    
    int frame = 0;
    
    for (auto it = times.begin(); it != times.end(); ++it) {
        
        ss << std::setfill('0') << std::setw(4);
        ss << frame++;
        frdName = strForward + ss.str() + ".png";
        rvsName = strReverse + ss.str() + ".png";
        ss.str(std::string());
        ss.clear();
        
        
        plot forward(width, height, scale);
        plot reverse(width, height, scale);
        
        forward.drawSLE(g, *it);
        reverse.drawReverseSLE(g, *it);
        
        
        
        forward.output(frdName.c_str());
        reverse.output(rvsName.c_str());
    }
    
    return 0;
}
