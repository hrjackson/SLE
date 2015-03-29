//
//  sle_process.h
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__sle_process__
#define __SLE__sle_process__

#include <stdio.h>
#include <vector>
#include <complex>
#include <map>
#include "stochastic_processes.h"

class SlitMap {
private:
    double alpha;
    double dt;
public:
    SlitMap(double alpha, double dt);
    std::complex<double> operator()(std::complex<double> z);
    void setAlpha(double newAlpha);
    void setDt(double newDt);
};


class SLE {
private:
    /*---- Data ----*/
    BrownianMotion* b;
    double kappa;
    std::map<double, SlitMap> h;
    std::map<double, std::complex<double>> z;
    int numFrames;
    // A vector of times which are guaranteed to be hit by adaptive process
    std::vector<double> admissibleTimes;
    /*---- Functions ----*/
    double angle(double t, double tOld);
    std::complex<double> pointEval(std::complex<double> z);
    void singleUpdate(double dt,
                      double& t,
                      SlitMap& candH,
                      std::complex<double>& candZ,
                      double& moved);
    std::vector<double> findAdmissibleTimes(double t_end);
    void constructProcess(double t_end, double tolerance);
    void adaptiveIncrement(double t_start,
                           double t_end,
                           double tolerance,
                           SlitMap& candH,
                           std::complex<double>& candZ);
public:
    SLE(BrownianMotion* b,
        double kappa,
        double t_end,
        double tolerance,
        double dtMin,
        int numFrames);
    double operator()(double time, std::vector<double> point);
    std::vector<double> getTimes();
    std::vector<double> getTimesFromZ();
    std::vector<std::complex<double>> getCurve();
};




#endif /* defined(__SLE__sle_process__) */
