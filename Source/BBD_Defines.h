/*
  ==============================================================================

    BBD_Defines.h
    Created: 23 Oct 2018 2:41:17pm
    Author:  Boris

  ==============================================================================
*/

#pragma once
#include <vector>
#include "cplxNum.h"
template<typename T> using vector = std::vector<T>;

static const auto N = 10;
static const auto FCLK = 50e3;
static const auto ORDERIN = 5;
static const auto ORDEROUT = 5;

static const vector<cplxNum<double>> RIN = 
{
    cplxNum<double>::cplxNum(251589, 0) 
    , cplxNum<double>::cplxNum(-130428, -4165) 
    , cplxNum<double>::cplxNum(-130428, 4165) 
    , cplxNum<double>::cplxNum(4634, -22873) 
    , cplxNum<double>::cplxNum(4634, 22873) 
};

static const vector<cplxNum<double>> ROUT = 
{
    cplxNum<double>::cplxNum(5092, 0) 
    , cplxNum<double>::cplxNum(11256, -99566) 
    , cplxNum<double>::cplxNum(11256, 99566) 
    , cplxNum<double>::cplxNum(-13802, -24606) 
    , cplxNum<double>::cplxNum(-13802, 24606) 
};

static const vector<cplxNum<double>> PIN = 
{
    cplxNum<double>::cplxNum(-46580, 0) 
    , cplxNum<double>::cplxNum(-55482, 25082) 
    , cplxNum<double>::cplxNum(-55482, -25082) 
    , cplxNum<double>::cplxNum(-26929, -59437) 
    , cplxNum<double>::cplxNum(-26929, 59437) 
};

static const vector<cplxNum<double>> POUT = 
{
    cplxNum<double>::cplxNum(-176261, 0) 
    , cplxNum<double>::cplxNum(-51468, 21437) 
    , cplxNum<double>::cplxNum(-51468, -21437) 
    , cplxNum<double>::cplxNum(-26276, -59699) 
    , cplxNum<double>::cplxNum(-26276, 59699) 
};
