/*
  ==============================================================================

    BBD.h
    Created: 21 Oct 2018 9:51:54pm
    Author:  Boris

  ==============================================================================
*/

#pragma once
#define USE_INTERPOLATION
#include <vector>
#include <list>
#include "complex.h"
#include <complex>

template<typename T> using vector = std::vector<T>;
template<typename T> using list = std::list<T>;
template<typename T> using complex = std::complex<T>;

class BDDFilter
{
public:
    BDDFilter (unsigned int orderIn, unsigned int orderOut);
    ~BDDFilter () {};

    inline void setInputZeros (vector<complex<double>> zeros) { mpRin = zeros; };
    inline void setInputPoles (vector<complex<double>> poles) { mpPin = poles; };
    inline void setOutputZeros (vector<complex<double>> zeros) { mpRout = zeros; };
    inline void setOutputPoles (vector<complex<double>> poles) { mpPout = poles; };

    inline void setInputPtr (float * in) { mpInput = in; };
    inline void setOutputPtr (float * out) { mpOutput = out; };

    inline float * getInputPtr () { return mpInput; };
    inline float * getOutputPtr () { return mpOutput; };

    inline void setSampleRate (float fs) { mSampleRate = fs; mSampleTime = 1.f / fs; };
    inline void setClockRate (float fclk) { mClockRate = fclk; mDelta = mSampleRate / mClockRate; };
    inline void setBlockSize (unsigned int size) { mBlockSize = size; };

    void init ();

    void setH0 ();

    void process ();

private:
    complex<double> gIn (unsigned int m, double d);
    complex<double> gOut (unsigned int m, double d);
#ifdef USE_INTERPOLATION
    complex<double> gInInterp (unsigned int m, double d);
    complex<double> gOutInterp (unsigned int m, double d);
#endif
    complex<double> pIn (unsigned int m);
    complex<double> pOut (unsigned int m);

    unsigned int mOrderIn;
    unsigned int mOrderOut;

    float mSampleRate;
    float mSampleTime;
    float mClockRate;
    unsigned int mBlockSize;

    float * mpInput;
    float * mpOutput;

    float mDelta;
    double mTn;
    unsigned long long mCounter;
    unsigned long long mK;
    double mEps;

    vector<complex<double>> mpRin;      //< zeros for input filters
    vector<complex<double>> mpRout;     //< zeros for output filters
    vector<complex<double>> mpPin;      //< poles for input filters
    vector<complex<double>> mpPout;     //< poles for output filters

    vector<complex<double>>  mpXout;
    vector<complex<double>>  mpXin;

    complex<double> mQueue[100];
    unsigned int mQueueIdx;

    complex<double> mYBBD;
    complex<double> mYBBDold;
    complex<double> mYDelta;
    complex<double> mH0;

    vector<complex<double>> mpIn;
    vector<complex<double>> mpOut;

#ifdef USE_INTERPOLATION
    vector<complex<double>> mgIn;
    vector<complex<double>> mgOut;;
#endif
};