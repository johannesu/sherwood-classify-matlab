#pragma once
#include <math.h>
#include <limits>
#include <vector>
#include "Sherwood.h"
#include "DataPointCollection.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  struct HistogramAggregator
  {
  public:
    std::vector<unsigned int> bins_;
    unsigned int binCount_;
    unsigned int sampleCount_;

  public:
    double Entropy() const;

    HistogramAggregator();

    HistogramAggregator(unsigned int nClasses);
    float GetProbability(unsigned int classIndex) const;

    unsigned int BinCount() const {return binCount_; }
    unsigned int SampleCount() const { return sampleCount_; }
    unsigned int FindTallestBinIndex() const;

    // IStatisticsAggregator implementation
    void Clear();

    void Aggregate(const IDataPointCollection& data, unsigned int index);
    void Aggregate(const HistogramAggregator& aggregator);

    HistogramAggregator DeepClone() const;
  };
} } }