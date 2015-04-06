#pragma once

#include "sherwood_mex.h"
#include <string>
#include <math.h>

// This file defines some IFeatureResponse implementations used by the example code in
// Classification.h, DensityEstimation.h, etc. Note we represent IFeatureResponse
// instances using simple structs so that all tree data can be stored
// contiguously in a linear array.

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  class Random;

  // Generate a normal distributed number given a uniform random number generator
  // Box-muller transform.
  float randn(Random& random) 
  {
    float u = (2*random.NextDouble()) - 1; 
    float v = (2*random.NextDouble()) - 1; 
    float r = u * u + v * v;

    if (r == 0 || r > 1) return randn(random);

    float c = sqrt(-2 * log(r) / r);
    return u * c;
  }

  class AxisAlignedFeatureResponse
  {
    unsigned int axis;

  public:
    AxisAlignedFeatureResponse() 
    : axis(-1)
    {}

    AxisAlignedFeatureResponse(unsigned int axis)
    : axis(axis)
    {}

    static AxisAlignedFeatureResponse CreateRandom( Random& random, 
                                                    unsigned int dimensions,
                                                    std::vector<Stats>& featureStats)
    {
      return AxisAlignedFeatureResponse(random.Next(0, dimensions));
    }

    unsigned int Axis() const
    {
      return axis;
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const  {
      const DataPointCollection& concreteData = (DataPointCollection&)(data);
      return concreteData.GetDataPoint(sampleIndex)[axis];
    }
    
    std::string ToString() const;
  };

  class AxisAlignedFeatureResponseNormalized
  {
    unsigned int axis;
    float mean;
    float stdev;

  public:
    AxisAlignedFeatureResponseNormalized() 
    : axis(-1)
    {}

    AxisAlignedFeatureResponseNormalized(unsigned int axis, float mean, float stdev)
    : axis(axis), mean(mean), stdev(stdev)
    {}

    static AxisAlignedFeatureResponseNormalized CreateRandom( Random& random, 
                                                    unsigned int dimensions,
                                                    std::vector<Stats>& featureStats)
    {
      unsigned int axis = random.Next(0, dimensions);
      return AxisAlignedFeatureResponseNormalized(axis, featureStats[axis].mean,  featureStats[axis].stdev);
    }

    unsigned int Axis() const
    {
      return axis;
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const  {
      const DataPointCollection& concreteData = (DataPointCollection&)(data);
      return (concreteData.GetDataPoint(sampleIndex)[axis] - mean)/stdev;
    }
    
    std::string ToString() const;
  };

  class RandomHyperplaneFeatureResponse
  {
  public:
    unsigned dimensions;
    std::vector<float> n;

    RandomHyperplaneFeatureResponse() {
      dimensions = 0;      
    }

    RandomHyperplaneFeatureResponse(  Random& random, 
                                      unsigned int dimensions,
                                      std::vector<Stats> featureStats) 
    : dimensions(dimensions)
    {
      n.resize(dimensions);

      // Normal distributed numbers to gives an unbiased random unit vector.
      for (unsigned int c = 0; c < dimensions; c++) {
        n[c] = randn(random); 
      }
    }

    static RandomHyperplaneFeatureResponse CreateRandom(Random& random, 
                                                        unsigned int dimensions,
                                                        std::vector<Stats>& featureStats)
    {
      return RandomHyperplaneFeatureResponse(random, dimensions, featureStats);
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int index) const
    {
      const DataPointCollection& concreteData = (const DataPointCollection&)(data);   
      
      float response = n[0] * concreteData.GetDataPoint(index)[0];
      for (unsigned int c = 1; c < dimensions; c++) {
        response += n[c] * concreteData.GetDataPoint(index)[c];  
      }

      return response;
    }
  };  

  class RandomHyperplaneFeatureResponseNormalized
  {
  public:
    unsigned dimensions;
    std::vector<float> n;
    std::vector<Stats> featureStats;

    RandomHyperplaneFeatureResponseNormalized() {
      dimensions = 0;      
    }

    RandomHyperplaneFeatureResponseNormalized(  Random& random, 
                                      unsigned int dimensions,
                                      std::vector<Stats> featureStats) 
    : dimensions(dimensions), featureStats(featureStats)
    {
      n.resize(dimensions);

      // Normal distributed numbers to gives an unbiased random unit vector.
      for (unsigned int c = 0; c < dimensions; c++) {
        n[c] = randn(random); 
      }
    }

    static RandomHyperplaneFeatureResponseNormalized CreateRandom(Random& random, 
                                                        unsigned int dimensions,
                                                        std::vector<Stats>& featureStats)
    {
      return RandomHyperplaneFeatureResponseNormalized(random, dimensions, featureStats);
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int index) const
    {
      const DataPointCollection& concreteData = (const DataPointCollection&)(data);   
      
      float response = n[0] * ((concreteData.GetDataPoint(index)[0] - featureStats[0].mean) / featureStats[0].stdev);
      for (unsigned int c = 1; c < dimensions; c++) {
        response += n[c] * ((concreteData.GetDataPoint(index)[c] - featureStats[c].mean) / featureStats[c].stdev);  
      }

      return response;
    }
  };	

} } }