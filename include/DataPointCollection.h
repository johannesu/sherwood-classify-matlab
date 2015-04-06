// Implementation of DataPoint for Sherwood
// for MATLAB matrices. No data is copied.
#pragma once

#include "sherwood_mex.h"
#include <set>


using namespace MicrosoftResearch::Cambridge::Sherwood;

/// <summary>
/// A collection of data points, each represented by a float[] and (optionally)
/// associated with a string class label and/or a float target value.
/// </summary>
class DataPointCollection: public IDataPointCollection
{
public:

  DataPointCollection(const matrix<float>& features) : features(features)
  {
    // features(feature_id, example_id)
    numFeatures = features.M;
    numPoints   = features.N;

    numLabels = 0;
  }; 

  DataPointCollection(const matrix<float>& features, const matrix<unsigned char>& labels) 
  : features(features), labels(labels)
  {
    // features(feature_id, example_id)
    numFeatures = features.M;
    numPoints   = features.N;

    std::set<unsigned int> la(&labels.data[0], &labels.data[numPoints]);
    numLabels = la.size();
  }; 

  bool HasLabels() const
  {
    return (numLabels != 0);
  }

  unsigned int CountClasses() const
  {
    if (!HasLabels())
      throw std::runtime_error("Unlabelled data.");
    return numLabels;
  }

  bool HasTargetValues() const
  {
    return false;
  }

  unsigned int Count() const
  {
    return numPoints;
  }

  /// <returns>A tuple containing min and max over the specified dimension of the data</returns>
  std::pair<float, float> GetRange(int dimension) const
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
  std::pair<float, float> GetTargetRange() const
  {
    throw std::runtime_error("Target not implemented.");
  }

  unsigned int Dimensions() const
  {
    return numFeatures;
  }

  /// Returns Pointer to the first element of the data point.
  const float* GetDataPoint(unsigned int i) const
  {   
    return &features.data[i*numFeatures];
    //return &data_[i*dimension_];
  }

  unsigned int GetIntegerLabel(unsigned int i) const
  {
    return labels(i);
  }

  float GetTarget(int i) const
  {
    throw std::runtime_error("Data have no associated target values.");
  }

  // features(feature_id, example_id)
  const matrix<float> features;
  const matrix<unsigned char> labels;
  unsigned int numPoints;
  unsigned int numLabels;
  unsigned int numFeatures;
  static const int UnknownClassLabel = -1;
};