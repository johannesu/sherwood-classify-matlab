#pragma once

// This file defines some IFeatureResponse implementations used by the example code in
// Classification.h, DensityEstimation.h, etc. Note we represent IFeatureResponse
// instances using simple structs so that all tree data can be stored
// contiguously in a linear array.

#include <string>
#include "Sherwood.h"
#include "DataPointCollection.h"
#include <math.h>

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  class Random;

  // Generate a normal distributed number given a uniform random number generator
  // Box-muller transform.
  float randn(Random& random);

  /// <summary>
  /// A feature that orders data points using one of their coordinates,
  /// i.e. by projecting them onto a coordinate axis.
  /// </summary>
  class AxisAlignedFeatureResponse
  {
    unsigned int axis_;

  public:
    AxisAlignedFeatureResponse()
    {
      axis_ = -1;
    }

    /// <summary>
    /// Create an AxisAlignedFeatureResponse instance for the specified axis.
    /// </summary>
    /// <param name="axis">The zero-based index of the axis.</param>
    AxisAlignedFeatureResponse(unsigned int axis)
    {
      axis_ = axis;
    }

    /// <summary>
    /// Create an AxisAlignedFeatureResponse instance with a random choice of axis.
    /// </summary>
    /// <param name="randomNumberGenerator">A random number generator.</param>
    /// <returns>A new AxisAlignedFeatureResponse instance.</returns>
    static AxisAlignedFeatureResponse CreateRandom(Random& random);
    static AxisAlignedFeatureResponse CreateRandom(Random& random, unsigned int dimensions);

    unsigned int Axis() const
    {
      return axis_;
    }

    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const;

    std::string ToString() const;
  };

  /// <summary>
  /// A feature that orders data points using a linear combination of their
  /// coordinates, i.e. by projecting them onto a given direction vector.
  /// </summary>
  class RandomHyperplaneFeatureResponse
  {
  public:
    unsigned dimensions;
    std::vector<float> n;

    RandomHyperplaneFeatureResponse() {
      dimensions = 0;      
    }


    /// <summary>
    /// Create a RandomHyperplaneFeatureResponse instance for the specified direction vector.
    /// </summary>
    /// <param name="dx">The first element of the direction vector.</param>
    /// <param name="dx">The second element of the direction vector.</param> 
    RandomHyperplaneFeatureResponse(Random& random, unsigned int dimensions) 
    : dimensions(dimensions)
    {
      n.resize(dimensions);

      // Normal distributed numbers to gives an unbiased random unit vector.
      for (unsigned int c = 0; c < dimensions; c++)
        n[c] = randn(random); 
    }
    /// <summary>
    /// Create a RandomHyperplaneFeatureResponse instance with a random direction vector.
    /// </summary>
    /// <param name="randomNumberGenerator">A random number generator.</param>
    /// <returns>A new RandomHyperplaneFeatureResponse instance.</returns>
    static RandomHyperplaneFeatureResponse CreateRandom(Random& random);
    static RandomHyperplaneFeatureResponse CreateRandom(Random& random, unsigned int dimensions);
    // IFeatureResponse implementation
    float GetResponse(const IDataPointCollection& data, unsigned int index) const;
  };	

} } }
