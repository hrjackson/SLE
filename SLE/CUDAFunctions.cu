//
//  CUDAfunctions.cu
//  SLE
//
//  Created by Henry Jackson on 19/06/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "CUDAfunctions.cuh"

/*--- Reverse Functions ---*/

__global__ void updateMatrixReverseGPU(int start, int end,
	                                double offset,
									cpx* inMat, cpx* outMat,
									double* dt, double* shift,
									int rows, int cols)
{
	int Row = blockIdx.y*blockDim.y + threadIdx.y;
	int Col = blockIdx.x*blockDim.x + threadIdx.x;

	if (Row < rows && Col < cols){
		updateValueReverse(start, end, offset, inMat[Row*cols + Col], outMat[Row*cols + Col], dt, shift);
	}
}

__device__ cpx slitMap(int index, cpx inValue, double* dt, double* shifts){
	return sqrt(4 * dt[index] - inValue*inValue)*cpx(0, 1) + shifts[index];
}

__device__ void updateValueReverse(int start, int end,
	double offset,
	cpx inValue,
	cpx& outValue,
	double* dt, double* shift)
{
	outValue = slitMap(end - 1, inValue - offset, dt, shift);
	for (int i = end - 2; i >= start; --i) {
		outValue = slitMap(i, outValue, dt, shift);
	}
}

/*--- Forward Functions ---*/

__global__ void updateMatrixForwardGPU(int start, int end,
	cpx* inMat, cpx* outMat,
	double* dt, double* shift,
	int rows, int cols)
{
	int Row = blockIdx.y*blockDim.y + threadIdx.y;
	int Col = blockIdx.x*blockDim.x + threadIdx.x;

	if (Row < rows && Col < cols){
		updateValueForward(start, end, inMat[Row*cols + Col], outMat[Row*cols + Col], dt, shift);
	}
}

__device__ void updateValueForward(int start, int end,
	cpx inValue,
	cpx& outValue,
	double* dt, double* shift)
{
	outValue = slitMapInverse(start, inValue, dt, shift);
	for (int i = start + 1; i < end; ++i) {
		outValue = slitMapInverse(i, outValue, dt, shift);
	}
}

__device__ cpx slitMapInverse(int index, cpx inValue, double* dt, double* shifts){
	cpx outValue = sqrt(-(inValue - shifts[index])*(inValue - shifts[index]) - 4 * dt[index]) * cpx(0, 1);
	return outValue;
}