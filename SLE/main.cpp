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
#include <random>

#include "stochastic_processes.h"
#include "sle_process.h"
#include "plot.h"
#include "SLEAnimate.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // Parameters
	vector<double> kappa = { 1 };
    double t_end = 0.99;
    double tolerance = 0.05;
    double dtMin = 0.001;
    int numFrames = 18;
    
    int width = 2880;
    int height = 1800;
    int scale = height/2;
    double border = 0.03;
    
    double gridRes = 0.01;
    double gridSpacing = 0.25;
    
    
    // Start
	BrownianMotion B(vector < double > {0.0});
    
    std::cout.precision(20);
    
	for (auto kap = kappa.begin(); kap != kappa.end(); ++kap)
	{


		SLE g(&B, *kap, t_end, tolerance, dtMin, numFrames);

		plot plotLeft(width, height, scale, border);
		plot plotRight(width, height, scale, border);
		plot plotReverse(width, height, scale, border);

		std::cout << "Just about to initialise the animation object" << std::endl;
		SLEAnimate sa(gridRes, gridSpacing, g, plotLeft, plotRight);
		std::cout << "finished that" << std::endl;
		//sa.plotForward();
		//sa.plotReverse();

		for (int i = 0; i < numFrames - 1; ++i) {
			sa.output(i);
			std::cout << "Frame " << i << std::endl;
			sa.nextFrame();
			//sa.output();
		}
		sa.output(numFrames - 1);
	}
    
    return 0;
}
