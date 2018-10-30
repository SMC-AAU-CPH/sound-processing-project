/*
  ==============================================================================

    BBD.cpp
    Created: 21 Oct 2018 9:51:54pm
    Author:  Boris

  ==============================================================================
*/

#include "BBD.h"
#include <math.h>

BDDFilter::BDDFilter (unsigned int orderIn, unsigned int orderOut)
    : mOrderIn (orderIn)
    , mOrderOut (orderOut)
    , mCounter (0)
    , mYBBDold (cplxNum<double>::cplxNum (0, 0))
    , mEps (1e-20)
    , mTn (0.0)
    , mK (0)
{
    mpXin.resize (orderIn, cplxNum<double>::cplxNum(0,0));
    mpXout.resize (orderOut, cplxNum<double>::cplxNum(0,0));
}

void BDDFilter::setH0 ()
{
    mH0 = cplxNum<double>::cplxNum (0, 0);
    for (unsigned int m = 0; m < mOrderOut; ++m)
    {
        mH0 = mH0 + mpRout[m] / mpPout[m];
    }
    mH0 = -mH0;
}

cplxNum<double> BDDFilter::gIn (unsigned int m, double d)
{
    cplxNum<double> ret (0, 0);
    ret = mpRin[m] * (mpPin[m] ^ d);
    ret = ret * mSampleTime;
    return ret;
}

cplxNum<double> BDDFilter::gOut (unsigned int m, double d)
{
    cplxNum<double> ret (0, 0);
    ret = (mpRout[m] / mpPout[m]) * (mpPout[m] ^ (-1.0 - d));
    return ret;
}

cplxNum<double> BDDFilter::pIn (unsigned int m)
{
    cplxNum<double> ret (0, 0);
    double real = mpPin[m].getReal ();
    double imag = mpPin[m].getImaginary ();
    real *= mSampleTime;
    imag *= mSampleTime;
    double mag = exp (real);
    double arg = imag;
    ret.setExp (mag, arg);
    return ret;
}

cplxNum<double> BDDFilter::pOut (unsigned int m)
{
    cplxNum<double> ret (0, 0);
    double real = mpPout[m].getReal ();
    double imag = mpPout[m].getImaginary ();
    real *= mSampleTime;
    imag *= mSampleTime;
    double mag = exp (real);
    double arg = imag;
    ret.setExp (mag, arg);
    return ret;
}

void BDDFilter::process ()
{
    mTn = 0.0;
    mK = 0;
    for (unsigned int smpl = 0; smpl < mBlockSize; ++smpl)
    {
        while ((mTn< mK) || ((0 > mCounter % 2) && (abs (mTn - mK) > mEps)))
        {
            double d = (mTn - (mK - 1))*mSampleTime;
            if (0 == (mCounter % 2))
            {
                cplxNum<double> sum (0, 0);
                for (unsigned int m = 0; m < mOrderIn; ++m)
                {
                    sum = sum + gIn (m, d)*mpXin[m]; 
                }
                mQueue.push_back (sum);
            }
            else
            {
                mYBBD = mQueue.back ();
                mQueue.pop_back ();
                mYDelta = mYBBD - mYBBDold;
                mYBBDold = mYBBD;
                for (unsigned int m = 0; m < mOrderOut; ++m)
                {
                    mpXout[m] = mpXout[m] + gOut (m, d) * mYDelta;
                }
            }
            mCounter++;
            mTn += mDelta;
        }
        for (unsigned int m = 0; m < mOrderIn; ++m)
        {
            mpXin[m] = pIn (m) * mpXin[m] + mpInput[smpl];
        }
        cplxNum<double> y = mH0 * mYBBDold;
        for (unsigned int m = 0; m < mOrderOut; ++m)
        {
            y = y + mpXout[m];
            mpXout[m] = pOut (m)*mpXout[m];
        }
        mpOutput[smpl] = y.getReal ();
        mK++;
    }
}
