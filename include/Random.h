#pragma once

// This files defines the Random class, used throughout the forest training
// framework for random number generation.

#include <time.h>
#include <cstdlib>
#include <iostream> 

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
// RAND_MAX on visual studio is just 2^15 which is way too small
// This code also avoid the issue that srand is not thread safe.
class Random
{
public:   
    Random() : seed( (unsigned int)(time(NULL))), a(214013), c(2531011), m(2147483648)  
    {}

    Random(unsigned int s) : seed(s), a(214013), c(2531011), m(2147483648) 
    {}

    int Next() { 
      return( seed = ( a * seed + c ) % m ); 
    }

    double NextDouble()
    {
      return (double)(Next())/m;
    }
 
    int Next(int minValue, int maxValue)
    {
      return minValue + Next()%(maxValue-minValue);
    }

private:
    const int a, c;
    const unsigned int m;
    unsigned int seed;
};

} } }
