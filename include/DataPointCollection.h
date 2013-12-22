// Implementation of DataPoint for Sherwood
// for MATLAB matrices. No data is copied.
#pragma once

#include "mex.h"
#include "mexutils.h"
#include "cppmatrix.h" 

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Sherwood.h"

using namespace MicrosoftResearch::Cambridge::Sherwood;

/// <summary>
/// A collection of data points, each represented by a float[] and (optionally)
/// associated with a string class label and/or a float target value.
/// </summary>
class DataPointCollection: public IDataPointCollection
{
public:
  DataPointCollection(const matrix<float>& _features); 
  DataPointCollection(const matrix<float>& _features, const matrix<unsigned char>& _labels); 

  // features(feature_id, example_id)
  const matrix<float> features;
  const matrix<unsigned char> labels;
  unsigned int numPoints;
  unsigned int numLabels;
  unsigned int numFeatures;

  static const int UnknownClassLabel = -1;

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

  std::pair<float, float> GetRange(int dimension) const;
  std::pair<float, float> GetTargetRange() const;

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
};
