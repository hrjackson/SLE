//
//  SLEAnimate.cpp
//  SLE
//
//  Created by Henry Jackson on 22/04/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "SLEAnimate.cuh"
#include <iostream>

#define TILE_WIDTH 32

/*--- CUDA Error checking ---*/
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}


////////////////////////////////////////////////////////////////////////////////
//// SLEAnimate private member function definitions ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void SLEAnimate::initialiseLeft(){
    drawColours(leftPlot, pxOriginal, pxOriginalRows, pxOriginalCols);
    drawLines(leftPlot, horizontal, horizontalRows, horizontalCols, hzColour);
    drawLinesTranspose(leftPlot, vertical, verticalRows, verticalCols, vtColour);
    
    leftPlot.drawAxis();
}

void SLEAnimate::drawLines(class plot& plot,
                           cpx* matrix,
                           int matrixRows,
                           int matrixCols,
                           Mat& colours){
    double offset = -stabilser.real();
    for (int i=0; i<matrixRows; ++i) {
        vector<cpx> line;
        for (int j=0; j<matrixCols-1; ++j) {
            line = vector<cpx>{matrix[i*matrixCols + j] + offset, matrix[i*matrixCols + j + 1] + offset};
            Vec3b col = colours.at<Vec3b>(i,j);
            Scalar colScalar = Scalar( (double)col[0], (double)col[1], (double)col[2] );
            plot.drawLine(line, colScalar);
        }
    }
    // Draws the little blue thing at the bottom of the screen
    vector<cpx> offsetLine;
    offsetLine.push_back(offset - cpx(0, gridSpacing/2));
    offsetLine.push_back(offset + cpx(0, gridSpacing/2));
    plot.drawLine(offsetLine, Scalar(255,0,0));
}

void SLEAnimate::drawLinesTranspose(class plot& plot,
                           cpx* matrix,
                           int matrixRows,
                           int matrixCols,
                           Mat& colours){
    double offset = -stabilser.real();
    Mat tmpColours = colours.t();
    for (int j=0; j<matrixCols; ++j) {
        vector<cpx> line;
        for (int i=0; i<matrixRows-1; ++i) {
            line = vector<cpx>{matrix[i*matrixCols + j] + offset, matrix[(i+1)*matrixCols + j] + offset};
            Vec3b col = colours.at<Vec3b>(i,j);
            Scalar colScalar = Scalar( (double)col[0], (double)col[1], (double)col[2] );
            plot.drawLine(line, colScalar);
        }
    }
    // Draws the little blue thing at the bottom of the screen
    vector<cpx> offsetLine;
    offsetLine.push_back(offset - cpx(0, gridSpacing/2));
    offsetLine.push_back(offset + cpx(0, gridSpacing/2));
    plot.drawLine(offsetLine, Scalar(255,0,0));
}

void SLEAnimate::drawColours(class plot& plot,
                             cpx* points,
                             int pointsRows,
                             int pointsCols){
    Mat colours = generateColours(points, pointsRows, pointsCols, false);
    for (int i = 0; i < pointsRows; ++i) {
        for (int j = 0; j < pointsCols; ++j){
            plot.colour(i, j, colours.at<Vec3b>(i,j) );
        }
    }
}

cpx* SLEAnimate::generateHorizontal(){

    double width = 2*(leftPlot.maxX() - leftPlot.minX());
    double height = 2*leftPlot.maxY();
    horizontalRows = (int)(height/gridSpacing);
    horizontalCols = (int)(width/gridRes);
    cpx* result = new cpx [horizontalRows*horizontalCols];
    cpx heightIncrement(0, gridSpacing);
    cpx widthIncrement(gridRes, 0);
    
    for (int i=0; i<horizontalRows; ++i) {
        for (int j=0; j<horizontalCols; ++j) {
            result[horizontalCols*i + j] = heightIncrement*(double)(i+1)
                          + widthIncrement*(double)j - width/2;
        }
    }
    return result;
}

cpx* SLEAnimate::generateVertical() {
    double width = 2*(leftPlot.maxX() - leftPlot.minX());
    double height = 2*leftPlot.maxY();
    verticalRows = (int)(height/gridRes) + 1;
    verticalCols = (int)(width/gridSpacing) + 2;
    cpx* result;
    result = new cpx [verticalRows*verticalCols];
    cpx heightIncrement(0, gridRes);
    cpx widthIncrement(gridSpacing, 0);
    
    for (int i=0; i<verticalRows; ++i) {
        for (int j=0; j<verticalCols; ++j) {
            result[i*verticalCols +j] = heightIncrement*(double)i
                          + widthIncrement*(double)j - width/2;
        }
    }
    return result;
}

/*
cv::Mat_<cpx> SLEAnimate::generatePixelPos() {
    int rows = rightPlot.rows();
    int cols = rightPlot.cols();
    cv::Mat_<cpx> result( rows, cols );
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            result(i,j) = rightPlot.CVTocpx(Point(i,j));
        }
    }
    return result;
}
*/

Vec3b SLEAnimate::cpxToColour(cpx z, bool shader) {
    Vec3b result;
    double x = z.real();
    double y = z.imag();
    
    // The height of the point, relative to the maximum
    double yProp = y/lineHeight;
    if (yProp < 0) yProp = 0;
    int yCV = int((darkRows-1)*(1-yProp));
    
    // Similar for x, but x can be negative
    double xProp = (x/lineWidth + 1)/2;
	if (xProp > 1) xProp = 1;
    int xCV = int((darkCols-1)*xProp);
    
    if (shader) {
        result = dark.at<Vec3b>(yCV, xCV);
    } else {
        result = light.at<Vec3b>(yCV, xCV);
    }
    return result;
}

Mat SLEAnimate::generateColours(cpx* points,
                                int pointsRows,
                                int pointsCols,
                                bool shader) {
    Mat result = Mat::Mat(pointsRows, pointsCols, CV_8UC3, Scalar(255,255,255));
    for (int i = 0; i < pointsRows; ++i){
        for (int j = 0; j < pointsCols; ++j) {
            result.at<Vec3b>(i,j) = cpxToColour( points[i*pointsCols +j], shader);
        }
    }
    return result;
}

void SLEAnimate::updateMatrixForward(int start, int end,
                                     cpx* inMat,
                                     cpx* outMat,
									 cpx* outMatHost,
                                     int rows,
                                     int cols){
	dim3 dimGrid((cols - 1) / TILE_WIDTH + 1, (rows - 1) / TILE_WIDTH + 1, 1);
	dim3 dimBlock(TILE_WIDTH, TILE_WIDTH, 1);
	updateMatrixForwardGPU<<<dimGrid, dimBlock>>>(start, end, inMat, outMat, dtDevice, shiftsDevice, rows, cols);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaMemcpy(outMatHost, outMat, rows*cols*sizeof(cpx), cudaMemcpyDeviceToHost));
}

void SLEAnimate::updateValueForward(int start, int end,
                        cpx inValue,
                        cpx& outValue){
    outValue = slitMapInverse(start, inValue);
    for (int i = start + 1; i < end; ++i) {
        outValue = slitMapInverse(i, outValue);
    }
}

cpx SLEAnimate::slitMapInverse(int index, cpx inValue){
    //cout << shifts[index] << " " << dt[index] << endl;
    cpx outValue = sqrt( -(inValue - shifts[index])*(inValue - shifts[index]) - 4*dt[index]) * cpx(0,1);
    return outValue;
}

void SLEAnimate::updateMatrixReverse(int start, int end,
                                     double offset,
                                     cpx* inMat,
                                     cpx* outMat,
									 cpx* outMatHost,
                                     int rows,
                                     int cols)
{
	dim3 dimGrid( (cols-1)/TILE_WIDTH + 1, (rows-1)/TILE_WIDTH + 1, 1);
	dim3 dimBlock(TILE_WIDTH, TILE_WIDTH, 1);
	updateMatrixReverseGPU<<<dimGrid, dimBlock>>>(start, end, offset, inMat, outMat, dtDevice, shiftsDevice, rows, cols);
	gpuErrchk(cudaPeekAtLastError());
	cudaMemcpy(outMatHost, outMat, rows*cols*sizeof(cpx), cudaMemcpyDeviceToHost);
}

void SLEAnimate::updateValueReverse(int start, int end,
                                    double offset,
                                    cpx inValue,
                                    cpx& outValue){
    outValue = slitMap(end-1, inValue - offset);
    for (int i = end-2; i >= start; --i) {
        outValue = slitMap(i, outValue);
    }
}

cpx SLEAnimate::slitMap(int index, cpx inValue){
    return sqrt(4*dt[index] - inValue*inValue)*cpx(0,1) + shifts[index];
}

void SLEAnimate::updateMatrixForward(SlitMap& h,
                                     cpx* inMat,
                                     cpx* outMat,
                                     int rows,
                                     int cols){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outMat[i*cols + j] = h.inverse(inMat[i*cols + j]);
        }
    }
}

void SLEAnimate::updateMatrixReverse(SlitMap& h,
                                     double offset,
                                     cpx* inMat,
                                     cpx* outMat,
                                     int rows,
                                     int cols){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outMat[i*cols + j] = h(inMat[i*cols + j] - offset);
        }
    }
}

void SLEAnimate::plot() {
    rightPlot.clear();
    drawColours(rightPlot, pxNow, pxOriginalRows, pxOriginalCols);
    drawLines(rightPlot, horizontal, horizontalRows, horizontalCols, hzColour);
    drawLinesTranspose(rightPlot, vertical, verticalRows, verticalCols, vtColour);
    if (currentTime > 0) {
        leftPlot.drawLine(g.forwardLine( *(--frameTimes.lower_bound(currentTime)),
                                        currentTime),
                          Scalar(255,0,0));
    }
    rightPlot.drawAxis();
}

////////////////////////////////////////////////////////////////////////////////
//// SLEAnimate member function definitions ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SLEAnimate::SLEAnimate(double gridRes,
                       double gridSpacing,
                       SLE& g,
                       class plot& left,
                       class plot& right)
:gridRes(gridRes), gridSpacing(gridSpacing),
g(g), leftPlot(left), rightPlot(right) {
    /*--- Initialise CUDA data ---*/
	numMaps = g.numMaps();
	int numBits = numMaps*sizeof(double);
    dt = g.times();

	cudaMalloc((void**)&dtDevice, numBits);
	cudaMemcpy(dtDevice, dt, numBits, cudaMemcpyHostToDevice);

    shifts = g.shifts();
	cudaMalloc((void**)&shiftsDevice, numBits);
	cudaMemcpy(shiftsDevice, shifts, numBits, cudaMemcpyHostToDevice);
    
    // Import the colour matrices
    dark = imread("D:\\sleOutput\\col\\dark.png", CV_LOAD_IMAGE_COLOR);
    //dark = imread("/Users/Henry/tmp/colours/dark.png", CV_LOAD_IMAGE_COLOR);
	light = imread("D:\\sleOutput\\col\\light.png", CV_LOAD_IMAGE_COLOR);
    //light = imread("/Users/Henry/tmp/colours/light.png", CV_LOAD_IMAGE_COLOR);
    darkRows = dark.rows;
    darkCols = dark.cols;
    lightRows = light.rows;
    lightCols = light.cols;
    
    // Initialise the line matrices
    lineHeight = 2*leftPlot.maxY();
    lineWidth = 2*leftPlot.maxX();
    horizontal = generateHorizontal();
    hzColour = generateColours(horizontal, horizontalRows, horizontalCols, true);
    vertical = generateVertical();
    vtColour = generateColours(vertical, verticalRows, verticalCols, true);

	// Copy line matrices to device
	cudaMalloc((void**)&horizontalDevice, horizontalRows*horizontalCols*sizeof(cpx));
	cudaMemcpy(horizontalDevice, horizontal, 
		horizontalRows*horizontalCols*sizeof(cpx), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&verticalDevice, verticalRows*verticalCols*sizeof(cpx));
	cudaMemcpy(verticalDevice, vertical,
		verticalRows*verticalCols*sizeof(cpx), cudaMemcpyHostToDevice);
    
    // Initialise pixel position matrix
    pxOriginal = leftPlot.points();
    pxOriginalRows = leftPlot.pointsRows();
    pxOriginalCols = leftPlot.pointsCols();
    pxNow = leftPlot.points();

	// Copy pixel matrices to device
	cudaMalloc((void**)&pxOriginalDevice, pxOriginalRows*pxOriginalCols*sizeof(cpx));
	cudaMemcpy(pxOriginalDevice, pxOriginal, 
		pxOriginalRows*pxOriginalCols*sizeof(cpx), cudaMemcpyHostToDevice);
	cudaMalloc((void**)&pxNowDevice, pxOriginalRows*pxOriginalCols*sizeof(cpx));
	cudaMemcpy(pxNowDevice, pxNow,
		pxOriginalRows*pxOriginalCols*sizeof(cpx), cudaMemcpyHostToDevice);
    
    // Initialise the stabilisation point to somewhere far away
    // on the imaginary axis
    stabilser = cpx(0, 1000);
    stabiliserReverse = cpx(0, 1000);
    // Initialise the pixel matrix
    //pixelPos = generatePixelPos();
    // Initialise the times
    times = g.getOrderedTimes();
    frameTimes = g.orderedFrameTimes();
    currentTime = 0;
    initialiseLeft();
    plot();
}

bool SLEAnimate::nextFrame() {
    auto nextTimePtr = frameTimes.upper_bound(currentTime);
    if (nextTimePtr != frameTimes.end() ) {
        double nextTime = *nextTimePtr;
        vector<SlitMap> gridMaps;
        vector<SlitMap> pixelMaps;
        auto start = times.begin();
        auto lower = times.lower_bound(currentTime);
        auto upper = times.lower_bound(nextTime);
        int gridStart = (int)distance(start, lower);
        int end = (int)distance(start, upper);
        
        updateMatrixForward(gridStart, end,
                            horizontalDevice,
                            horizontalDevice,
							horizontal,
                            horizontalRows,
                            horizontalCols);
        updateMatrixForward(gridStart, end,
                            verticalDevice,
                            verticalDevice,
							vertical,
                            verticalRows,
                            verticalCols);
        
        SlitMap h = g.slitMap(0);
        for (auto it = times.lower_bound(currentTime); *it < nextTime; ++it) {
            h = g.slitMap(*it);
            gridMaps.push_back(h);
            stabilser = h.inverse(stabilser);
            stabiliserReverse = h(stabiliserReverse);
            //cout << "h dt " << h.getDt() << " h dx " << h.getOffset() << endl;
            //cout << "a dt " << dt[i] << " a dx " << shifts[i] << endl;
        }
        
        cout << "Vector length: " << gridMaps.size() << endl;
        
        for (auto it = times.begin(); *it < nextTime; ++it) {
            h = g.slitMap(*it);
            pixelMaps.push_back(h);
        }
        
        //updateMatrixForward(gridMaps, horizontal, horizontal, horizontalRows, horizontalCols);
        //updateMatrixForward(gridMaps, vertical, vertical, horizontalRows, horizontalCols);
        
        double offset = -stabilser.real();
        
        //updateMatrixReverse(pixelMaps, offset, pxOriginal, pxNow, pxOriginalRows, pxOriginalCols);
        
        updateMatrixReverse(0, end, offset, pxOriginalDevice, pxNowDevice, pxNow, pxOriginalRows, pxOriginalCols);
        
        currentTime = nextTime;
        plot();
        return true;
    }
    else {
        return false;
    }
}


void SLEAnimate::show() {
    leftPlot.show();
    rightPlot.show();
}

void SLEAnimate::output(int frame) {

    // Set up filenames
    std::string strLeft = "D:\\sleOutput\\left\\";
    std::string strRight = "D:\\sleOutput\\right\\";
    //std::string strLeft = "/Users/henry/tmp/left/";
    //std::string strRight = "/Users/henry/tmp/right/";
    std::string ltName;
    std::string rtName;
    std::stringstream ss;
    
    ss << std::setfill('0') << std::setw(4);
    ss << frame;
    ltName = strLeft + ss.str() + ".png";
    rtName = strRight + ss.str() + ".png";
    ss.str(std::string());
    ss.clear();
    
    leftPlot.output(ltName.c_str());
    rightPlot.output(rtName.c_str());
}

SLEAnimate::~SLEAnimate(){
    delete[] horizontal;
    delete[] vertical;
    delete [] pxOriginal;
    delete[] pxNow;
}
































