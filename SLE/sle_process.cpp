//
//  sle_process.cpp
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "sle_process.h"
#include <cmath>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
//// SlitMap member functions //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SlitMap::SlitMap(double alpha, double dt){
    this->alpha = alpha;
    this->dt = dt;
};

std::complex<double> SlitMap::operator()(std::complex<double> z){
    std::complex<double> lh = z + 2*sqrt(dt*(1-alpha)/alpha);
    std::complex<double> rh = z - 2*sqrt(dt*alpha/(1-alpha));
    return pow(lh, 1-alpha)*pow(rh, alpha);
};

void SlitMap::setAlpha(double newAlpha){
    alpha = newAlpha;
}

void SlitMap::setDt(double newDt){
    dt = newDt;
}



////////////////////////////////////////////////////////////////////////////////
//// SLE private member functions //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

double SLE::angle(double t, double tOld){
    double dx = ((*b)(t)[0] - (*b)(tOld)[0])*sqrt(kappa);
    double dt = t - tOld;
    double alpha;
    double v = pow(dx, 2)/dt;
    if (dx > 0) {
        alpha = 0.5 - sqrt(v/(v+16))/2;
    } else {
        alpha = 0.5 + sqrt(v/(v+16))/2;
    }
    return alpha;
};

std::complex<double> SLE::pointEval(std::complex<double> z){
    std::complex<double> result = z;
    for (auto rit = h.rbegin(); rit!= h.rend(); rit++) {
        result = (rit->second)(result);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////
//// SLE member functions //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SLE::SLE(BrownianMotion* b, double kappa, double t_end, double tolerance, double dtMin){
    this->b = b;
    this->kappa = kappa;
    
    // Initialise maps
    SlitMap id(0.5, 0);
    h.insert(std::pair<double, SlitMap>(0.0, id));
    z.insert(std::pair<double, std::complex<double>>(0.0, 0.0+0.0i));
    
    double t = 0;
    double tOld;
    double dt = 0.1; //sqrt(tolerance);
    double dx;
    double alpha;
    SlitMap candH(0.5, 0);
    std::complex<double> candZ;
    
    while (t < t_end) {
        tOld = t;
        t = t + dt;
        dx = ((*b)(t)[0] - (*b)(tOld)[0])*sqrt(kappa);
        alpha = angle(t, tOld);
        candH.setDt(dt);
        candH.setAlpha(alpha);
        candZ = pointEval(candH(0));
        //std::complex<double> test = z[tOld];
        
        double moved = abs(candZ - z[tOld]);
        
        while ( (moved > tolerance) | (moved < tolerance/2) ) {
            if ( moved > tolerance ) {
                dt = 0.8*dt;
            }
            else {
                dt = 1.2*dt;
            }
            
            t = tOld + dt;
            dx = ((*b)(t)[0] - (*b)(tOld)[0])*sqrt(kappa);
            alpha = angle(t, tOld);
            candH.setDt(dt);
            candH.setAlpha(alpha);
            candZ = pointEval(candH(0));
            
            moved = abs(candZ - z[tOld]);
            
            if (dt < dtMin) {
                moved = 3*tolerance/4;
            }
        }
        h.insert(std::pair<double, SlitMap>{t, candH});
        z.insert(std::pair<double, std::complex<double>>{t, candZ});
        std::cout << t << std::endl;
    }
}

std::vector<double> SLE::getTimes(){
    std::vector<double> result;
    for (auto it = h.begin(); it!=h.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

std::vector<double> SLE::getTimesFromZ(){
    std::vector<double> result;
    for (auto it = z.begin(); it!=z.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

std::vector<std::complex<double>> SLE::getCurve(){
    std::vector<std::complex<double>> result;
    for (auto it = z.begin(); it!=z.end(); it++) {
        result.push_back(it->second);
    }
    return result;
}







