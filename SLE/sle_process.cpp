//
//  sle_process.cpp
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "sle_process.h"
#include <cmath>

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
    double dx = (*b)(t)[0] - (*b)(tOld)[0];
    double dt = t - tOld;
    double alpha;
    double v = pow(dx, 2)/dt;
    if (dx > 0) {
        alpha = 1/2 - sqrt(v/(v+16))/2;
    } else {
        alpha = 1/2 + sqrt(v/(v+16))/2;
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

SLE::SLE(BrownianMotion* b, double kappa, double t_end, double tolerance){
    this->b = b;
    this->kappa = kappa;
    
    // Initialise maps
    SlitMap id(0.5, 0);
    h.insert(std::pair<double, SlitMap>{0.0, id});
    z.insert(std::pair<double, std::complex<double>>{0.0, 0.0+0.0i});
    
    double t = 0;
    double tOld;
    double dt = sqrt(tolerance);
    double dx;
    double alpha;
    SlitMap candH(0.5, 0);
    std::complex<double> candZ;
    
    while (t < t_end) {
        tOld = t;
        t = t + dt;
        dx = (*b)(t)[0] - (*b)(tOld)[0];
        alpha = angle(t, tOld);
        candH.setDt(dt);
        candH.setAlpha(alpha);
        candZ = pointEval(candH(0));
        
        // We'll sort out adaptivity later
        h.insert(std::pair<double, SlitMap>{t, candH});
        z.insert(std::pair<double, std::complex<double>>{t, candZ});
    }
}










