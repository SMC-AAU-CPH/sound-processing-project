/*
  ==============================================================================

    BBD_Defines.h
    Created: 23 Oct 2018 2:41:17pm
    Author:  Boris

  ==============================================================================
*/

#pragma once
#include <vector>
#include <complex>
template<typename T> using vector = std::vector<T>;

static const auto N = 10;
static const auto FCLK = 50e3;
static const auto ORDERIN = 5;
static const auto ORDEROUT = 5;
static const auto INTERP_PRECISION = 1000;

static const vector<complex<double>> RIN = 
{
    complex<double>::complex(251589, 0) 
    , complex<double>::complex(-130428, -4165) 
    , complex<double>::complex(-130428, 4165) 
    , complex<double>::complex(4634, -22873) 
    , complex<double>::complex(4634, 22873) 
};

static const vector<complex<double>> ROUT = 
{
    complex<double>::complex(5092, 0) 
    , complex<double>::complex(11256, -99566) 
    , complex<double>::complex(11256, 99566) 
    , complex<double>::complex(-13802, -24606) 
    , complex<double>::complex(-13802, 24606) 
};

static const vector<complex<double>> PIN = 
{
    complex<double>::complex(-46580, 0) 
    , complex<double>::complex(-55482, 25082) 
    , complex<double>::complex(-55482, -25082) 
    , complex<double>::complex(-26929, -59437) 
    , complex<double>::complex(-26929, 59437) 
};

static const vector<complex<double>> POUT = 
{
    complex<double>::complex(-176261, 0) 
    , complex<double>::complex(-51468, 21437) 
    , complex<double>::complex(-51468, -21437) 
    , complex<double>::complex(-26276, -59699) 
    , complex<double>::complex(-26276, 59699) 
};
