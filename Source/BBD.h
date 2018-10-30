/*
  ==============================================================================

    BBD.h
    Created: 21 Oct 2018 9:51:54pm
    Author:  Boris

  ==============================================================================
*/

#pragma once
#include <vector>
#include <list>
#include "cplxNum.h"
template<typename T> using vector = std::vector<T>;
template<typename T> using list = std::list<T>;

class BDDFilter
{
public:
    BDDFilter (unsigned int orderIn, unsigned int orderOut);
    ~BDDFilter () {};

    inline void setInputZeros (vector<cplxNum<double>> zeros) { mpRin = zeros; };
    inline void setInputPoles (vector<cplxNum<double>> poles) { mpPin = poles; };
    inline void setOutputZeros (vector<cplxNum<double>> zeros) { mpRout = zeros; };
    inline void setOutputPoles (vector<cplxNum<double>> poles) { mpPout = poles; };

    inline void setInputPtr (float * in) { mpInput = in; };
    inline void setOutputPtr (float * out) { mpOutput = out; };

    inline float * getInputPtr () { return mpInput; };
    inline float * getOutputPtr () { return mpOutput; };

    inline void setSampleRate (float fs) { mSampleRate = fs; mSampleTime = 1.f / fs; };
    inline void setClockRate (float fclk) { mClockRate = fclk; mDelta = mSampleRate / mClockRate; };
    inline void setBlockSize (unsigned int size) { mBlockSize = size; };

    void setH0 ();

    void process ();

private:
    cplxNum<double> gIn (unsigned int m, double d);
    cplxNum<double> gOut (unsigned int m, double d);
    cplxNum<double> pIn (unsigned int m);
    cplxNum<double> pOut (unsigned int m);

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

    vector<cplxNum<double>> mpRin;      //< zeros for input filters
    vector<cplxNum<double>> mpRout;     //< zeros for output filters
    vector<cplxNum<double>> mpPin;      //< poles for input filters
    vector<cplxNum<double>> mpPout;     //< poles for output filters

    vector<cplxNum<double>>  mpXout;
    vector<cplxNum<double>>  mpXin;

    list<cplxNum<double>> mQueue;

    cplxNum<double> mYBBD;
    cplxNum<double> mYBBDold;
    cplxNum<double> mYDelta;
    cplxNum<double> mH0;
};