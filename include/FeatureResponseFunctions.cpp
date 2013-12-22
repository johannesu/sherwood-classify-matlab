#include "FeatureResponseFunctions.h"
#include <cmath>
#include <sstream>

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  float randn(Random& random) 
  {
    float u = (2*random.NextDouble()) - 1; 
    float v = (2*random.NextDouble()) - 1; 
    float r = u * u + v * v;

    if (r == 0 || r > 1) return randn(random);

    float c = sqrt(-2 * log(r) / r);
    return u * c;
  }

  AxisAlignedFeatureResponse AxisAlignedFeatureResponse ::CreateRandom(Random& random, unsigned int dimensions)
  {
    return AxisAlignedFeatureResponse(random.Next(0, dimensions));
  }

  float AxisAlignedFeatureResponse::GetResponse(const IDataPointCollection& data, unsigned int sampleIndex) const
  {
    const DataPointCollection& concreteData = (DataPointCollection&)(data);
    return concreteData.GetDataPoint(sampleIndex)[axis_];
  }

  // ------------------------ //

  /// <returns>A new RandomHyperplaneFeatureResponse instance.</returns>
  RandomHyperplaneFeatureResponse RandomHyperplaneFeatureResponse::CreateRandom(Random& random, unsigned int dimensions)
  {
    return RandomHyperplaneFeatureResponse(random, dimensions);
  }

  float RandomHyperplaneFeatureResponse::GetResponse(const IDataPointCollection& data, unsigned int index) const
  {
    const DataPointCollection& concreteData = (const DataPointCollection&)(data);   
    
    float response = n[0] * concreteData.GetDataPoint(index)[0];
    for (unsigned int c = 1; c < dimensions; c++)
      response += n[c] * concreteData.GetDataPoint(index)[c];  

    return response;
  }


} } }
