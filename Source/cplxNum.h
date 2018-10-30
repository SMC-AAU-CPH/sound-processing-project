/*
  ==============================================================================

    cplxNum.h
    CmReated: 21 Oct 2018 9:52:24pm
    Author:  Boris

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdexcept>

template <class T> 
class cplxNum
{
public:
    cplxNum () : mRe (static_cast<T>(0)), mIm (static_cast<T>(0)) { };
    cplxNum (T re, T im) : mRe (re), mIm (im) { mRe = re; mIm = im; convertToExp (); };
    ~cplxNum () {};

    cplxNum<T> expCplx (cplxNum<T> num);

    inline T getReal () { convertToRect ();  return mRe; };
    inline T getImaginary () { convertToRect (); return mIm; };

    inline double getMagnitude () { convertToExp ();  return mMag; };
    inline double getAngle () { convertToExp ();  return mAngle; };

    inline void setReal (T re) { mRe = re; convertToExp (); };
    inline void setImaginary (T im) { mIm = im; convertToExp (); };

    inline void setRect (T r, T i) { mRe = r; mIm = i; convertToExp (); };
    inline void setExp (T mag, T arg) { mMag = mag; mAngle = arg; convertToRect (); };

    cplxNum operator+(const cplxNum& z)
    {
        cplxNum ret;
        ret.mRe = this->mRe + z.mRe;
        ret.mIm = this->mIm + z.mIm;
        ret.convertToExp ();
        return ret;
    }

    cplxNum operator-(const cplxNum& z)
    {
        cplxNum ret;
        ret.mRe = this->mRe - z.mRe;
        ret.mIm = this->mIm - z.mIm;
        ret.convertToExp ();
        return ret;
    }

    cplxNum operator*(const cplxNum& z)
    {
        cplxNum ret;
        ret.mRe = this->mRe * z.mRe - this->mIm * z.mIm;
        ret.mIm = this->mRe * z.mIm + this->mIm * z.mRe;
        ret.convertToExp ();
        return ret;
    }

    cplxNum operator/(const cplxNum& z)
    {
        cplxNum ret;
        T den = z.mRe * z.mRe + z.mIm * z.mIm;
        ret.mRe = this->mRe * z.mRe + this->mIm * z.mIm;
        ret.mRe = ret.mRe / den;
        ret.mIm = this->mIm * z.mRe - this->mRe * z.mIm;
        ret.mIm = ret.mIm / den;
        ret.convertToExp ();
        return ret;
    }

    cplxNum operator^(const cplxNum& num)
    {
        cplxNum ret;
        double mag = pow (this->mMag, num.mRe) * exp (-this->mAngle * num.mIm);
        double ang = this->mAngle * num.mRe + num.mIm * log (this->mMag);
        ret.setExp (mag, ang);
        return ret;
    }

    template <typename X> cplxNum operator^(const X& x)
    {
        cplxNum ret;
        double mag = pow (this->mMag, static_cast<double>(x));
        double ang = this->mAngle * x;
        ret.setExp (mag, ang);
        return ret;
    }

    template <typename X> cplxNum operator+(const X& x)
    {
        cplxNum ret;
        T add = static_cast<T>(x);
        ret.mRe = this->mRe + add;
        ret.mIm = this->mIm;
        ret.convertToExp ();
        return ret;
    }

    template <typename X> cplxNum operator-(const X& x)
    {
        cplxNum ret;
        T sub = static_cast<T>(x);
        ret.mRe = this->mRe - sub;
        ret.mIm = this->mIm;
        ret.convertToExp ();
        return ret;
    }

    template <typename X> cplxNum operator*(const X& x)
    {
        cplxNum ret;
        T mult = static_cast<T>(x);
        ret.mRe = this->mRe * mult;
        ret.mIm = this->mIm * mult;
        ret.convertToExp ();
        return ret;
    }

    template <typename X> cplxNum operator/(const X& x)
    {
        cplxNum ret;
        T div = static_cast<T>(x);
        ret.mRe = this->mRe / div;
        ret.mIm = this->mIm / div;
        ret.convertToExp ();
        return ret;
    }

    cplxNum& operator-()
    {
        this->mIm = -this->mIm;
        this->mRe = -this->mRe;
        this->convertToExp();
        return *this;
    }


    friend std::ostream& operator << (std::ostream& out, const cplxNum& num)
    {
        out << num.mRe << " + i" << num.mIm << "\n";
        return out;
    }

    inline void printExp ()
    {
        std::cout << mMag << " * e^(i" << mAngle << ") (in degrees: " << mAngle * 180 / M_PI << ")\n";
    };

private:
    void convertToExp ();
    void convertToRect ();

    double arg ();
    double mag ();

    T mRe;
    T mIm;

    double mMag;
    double mAngle;
};

template<class T>
inline cplxNum<T> cplxNum<T>::expCplx (cplxNum<T> num)
{
    // r*e^((iP)*(a+ib)) = r*e^(-P*b)*e^(i*a*P)
    cplxNum<T> ret;
    double mag = pow (mMag, num.getReal ()) * exp (-mAngle * num.getImaginary ());
    double ang = mAngle * num.getReal () + num.getImaginary () * log (mMag);
    ret.setExp (mag, ang);
    return ret;
}

template<class T>
inline double cplxNum<T>::arg ()
{
    double verySmallNumber = 0.0000000000001;
    double ret;
    double x = static_cast<double>(this->mRe);
    double y = static_cast<double>(this->mIm);

    if (0 < x)
    {
        ret = atan (y / x);
    }
    else if (0 > x && 0 <= y)
    {
        ret = atan (y / x) + M_PI;
    }
    else if (0 > x && 0 > y)
    {
        ret = atan (y / x) - M_PI;
    }
    else if (verySmallNumber > x && 0 < y)
    {
        ret = M_PI / 2.0;
    }
    else if (verySmallNumber > x && 0 > y)
    {
        ret = -M_PI / 2.0;
    }
    else
    {
        ret  = 0.0;
    }

    return ret;
}

template<class T>
inline double cplxNum<T>::mag ()
{
    double ret;
    double x = static_cast<double>(this->mRe);
    double y = static_cast<double>(this->mIm);
    ret = x * x + y * y;
    ret = sqrt (ret);
    return static_cast<T>(ret);
}

template<class T>
inline void cplxNum<T>::convertToExp ()
{
    mMag = mag ();
    mAngle = arg ();
}

template<class T>
inline void cplxNum<T>::convertToRect ()
{
    mRe = mMag * static_cast<T>(cos (static_cast<double>(mAngle)));
    mIm = mMag * static_cast<T>(sin (static_cast<double>(mAngle)));
}
