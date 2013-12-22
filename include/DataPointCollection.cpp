#include "DataPointCollection.h"
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

DataPointCollection::DataPointCollection(const matrix<float>& _features) 
  : features(_features)
{
  // features(feature_id, example_id)
  numFeatures = features.M;
  numPoints   = features.N;

  numLabels = 0;
}; 


DataPointCollection::DataPointCollection(const matrix<float>& _features, const matrix<unsigned char>& _labels) 
  : features(_features), labels(_labels)
{
  // features(feature_id, example_id)
  numFeatures = features.M;
  numPoints   = features.N;

  std::set<unsigned int> la(&labels.data[0], &labels.data[numPoints]);
  numLabels = la.size();
}; 

/// <returns>A tuple containing min and max over the specified dimension of the data</returns>
std::pair<float, float> DataPointCollection::GetRange(int dimension) const
{
  if (Count() < 0)
    throw std::runtime_error("Insufficient features to compute range.");

  if (dimension < 0 || dimension> numFeatures)
    throw std::runtime_error("Insufficient features to compute range.");

  float min = features(dimension,0);
  float max = features(dimension,0);

  for (unsigned int i =1; i < numPoints; i++)
  {
    if (features(dimension,i)  < min)
      min = features(dimension,i);
    else if (features(dimension,i) > max)
      max = features(dimension,i);
  }

  return std::pair<float, float>(min, max);
}

/// <summary>
/// Get the range of target values (or raise an exception if these data
/// do not have associated target values).
/// </summary>
/// <returns>A tuple containing the min and max target value for the data</returns>
std::pair<float, float> DataPointCollection::GetTargetRange() const
{

  throw std::runtime_error("Target not implemented.");
  /*
  if (!HasTargetValues())
    throw std::runtime_error("Data points do not have target values.");
  if (Count() < 0)
    throw std::runtime_error("Insufficient data to compute range.");
  */
}