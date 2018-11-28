/*
  ==============================================================================

    BBD.cpp
    Created: 21 Oct 2018 9:51:54pm
    Author:  Boris

  ==============================================================================
*/

#include "BBD.h"
#include <math.h>
#include "BBD_Defines.h"
#include <limits.h>

bool equalDouble (double a, double b)
{
    return (abs (a - b) <= DBL_EPSILON);
}

BDDFilter::BDDFilter (unsigned int orderIn, unsigned int orderOut)
    : mOrderIn (orderIn)
    , mOrderOut (orderOut)
    , mN (0)
    , mYBBDold (complex<double>::complex (0, 0))
    , mEps (1e-20)
    , mTn (0.0)
    , mK (0)
    , mQueueIdx (0)
{
    mpXin.resize (orderIn, complex<double>::complex(0,0));
    mpXout.resize (orderOut, complex<double>::complex(0,0));
    mpIn.resize (orderIn, complex<double>::complex(0,0));
    mpOut.resize (orderOut, complex<double>::complex(0,0));
#ifdef USE_INTERPOLATION
    mgIn.resize (orderIn * INTERP_PRECISION, complex<double>::complex(0,0));
    mgOut.resize (orderOut * INTERP_PRECISION, complex<double>::complex(0,0));
#endif
}


void BDDFilter::init ()
{
    for (unsigned int order = 0; order < mOrderIn; ++order)
    {
        mpIn[order] = pIn (order);
    }

    for (unsigned int order = 0; order < mOrderOut; ++order)
    {
        mpOut[order] = pOut (order);
    }

#ifdef USE_INTERPOLATION
    for (unsigned int order = 0; order < mOrderIn * INTERP_PRECISION; ++order)
    {
        double prec = static_cast<double>(order) / static_cast<double>(INTERP_PRECISION);
        double fractpart;
        double intpart;
        fractpart = std::modf (prec, &intpart);
        mgIn[order] = gIn (static_cast<int>(intpart), fractpart);
    }

    for (unsigned int order = 0; order < mOrderOut * INTERP_PRECISION; ++order)
    {
        double prec = static_cast<double>(order) / static_cast<double>(INTERP_PRECISION);
        double fractpart;
        double intpart;
        fractpart = std::modf (prec, &intpart);
        mgOut[order] = gOut (static_cast<int>(intpart), fractpart);
    }
#endif

}

void BDDFilter::setH0 ()
{
    mH0 = complex<double>::complex (0, 0);
    for (unsigned int m = 0; m < mOrderOut; ++m)
    {
        mH0 = mH0 + mpRout[m] / mpPout[m];
    }
    mH0 = -mH0;
}

complex<double> BDDFilter::gIn (unsigned int m, double d)
{
    complex<double> ret (0.0, 0.0);
    ret = mpRin[m] * pow (mpPin[m], d);
    double imag = ret.imag() * mSampleTime;
    double real = ret.real() * mSampleTime;
    ret = complex<double> (real, imag);
    return ret;
}

complex<double> BDDFilter::gOut (unsigned int m, double d)
{
    complex<double> ret (0, 0);
    ret = (mpRout[m] / mpPout[m]) * pow (mpPout[m], (-1.0 - d));
    return ret;
}

#ifdef USE_INTERPOLATION
complex<double> BDDFilter::gInInterp (unsigned int m, double d)
{
    double num = m + d;
    num *= INTERP_PRECISION;
    num = floor (num);
    return mgIn[num];
}

complex<double> BDDFilter::gOutInterp (unsigned int m, double d)
{
    double num = m + d;
    num *= INTERP_PRECISION;
    num = floor (num);
    return mgOut[num];
}
#endif

complex<double> BDDFilter::pIn (unsigned int m)
{
    complex<double> ret (0, 0);
    double real = mpPin[m].real ();
    double imag = mpPin[m].imag ();
    real *= mSampleTime;
    imag *= mSampleTime;
    double mag = exp (real);
    double arg = imag;
    ret = std::polar<double> (mag, arg);
    return ret;
}

complex<double> BDDFilter::pOut (unsigned int m)
{
    complex<double> ret (0, 0);
    double real = mpPout[m].real();
    double imag = mpPout[m].imag();
    real *= mSampleTime;
    imag *= mSampleTime;
    double mag = exp (real);
    double arg = imag;
    ret = std::polar<double> (mag, arg);
    return ret;
}

void BDDFilter::process ()
{
    for (unsigned int smpl = 0; smpl < mBlockSize; ++smpl)
    {
        while ((mTn < mK) || ((0 != mN % 2) && equalDouble(mTn, static_cast<double>(mK))))
        {
            double d = (mTn - (mK - 1))*mSampleTime;
            if (0 == (mN % 2))
            {
                complex<double> sum (0, 0);
                for (unsigned int m = 0; m < mOrderIn; ++m)
                {
#ifdef USE_INTERPOLATION
                    sum = sum + gInInterp (m, d) * mpXin[m];
#else
                    sum = sum + gIn(m, d)*mpXin[m]; 
#endif
                }
                mQueue[++mQueueIdx] = sum;
            }
            else
            {
                mYBBD = mQueue[mQueueIdx];
                mQueue[mQueueIdx] = complex<double> (0, 0);
                mQueueIdx--;
                mYDelta = mYBBD - mYBBDold;
                mYBBDold = mYBBD;
                for (unsigned int m = 0; m < mOrderOut; ++m)
                {
#ifdef USE_INTERPOLATION
                    mpXout[m] = mpXout[m] + gOutInterp (m, d) * mYDelta;
#else
                    mpXout[m] = mpXout[m] + gOut (m, d) * mYDelta;
#endif
                }
            }
            mN++;
            if (LLONG_MAX == mN)
            {
                mN = 0;
                mK = 0;
            }
            mTn = mN * mDelta;
        }
        // [END WHILE]
        for (unsigned int m = 0; m < mOrderIn; ++m)
        {
            mpXin[m] = mpIn [m] * mpXin[m] + std::polar<double>(static_cast<double>(mpInput[smpl]), 0.0);
        }

        complex<double> y = mH0 * mYBBDold;
        for (unsigned int m = 0; m < mOrderOut; ++m)
        {
            y = y + mpXout[m];
            mpXout[m] = mpOut[m] * mpXout[m];
        }
        mpOutput[smpl] = y.real ();
        mK++;
        if (LLONG_MAX == mK)
        {
            mK = 0;
            mN = 0;
        }
    }
}
