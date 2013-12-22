#include "StatisticsAggregators.h"
#include <iostream>


namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  double HistogramAggregator::Entropy() const
  {
    if (sampleCount_ == 0)
      return 0.0;

    double result = 0.0;
    for (unsigned int b = 0; b < BinCount(); b++)
    {
      double p = (double)bins_[b] / (double)sampleCount_;
      result -= p == 0.0 ? 0.0 : p * log(p)/log(2.0);
    }

    return result;
  }

  HistogramAggregator::HistogramAggregator()
  {
    binCount_ = 0;
    sampleCount_ = 0;
  }

  HistogramAggregator::HistogramAggregator(unsigned int nClasses) 
  {
    binCount_ = nClasses;
    
    bins_.resize(nClasses); 
    for(unsigned int b=0; b<binCount_; b++)
      bins_[b] = 0;

    sampleCount_ = 0;
  }

  float HistogramAggregator::GetProbability(unsigned int classIndex) const
  {
    return (float)(bins_[classIndex]) / sampleCount_;
  }

  unsigned int HistogramAggregator::FindTallestBinIndex() const
  {
    unsigned int maxCount = bins_[0];
    unsigned int tallestBinIndex = 0;

    for (unsigned int i = 1; i < BinCount(); i++)
    {
      if (bins_[i] > maxCount)
      {
        maxCount = bins_[i];
        tallestBinIndex = i;
      }
    }

    return tallestBinIndex;
  }

  // IStatisticsAggregator implementation
  void HistogramAggregator::Clear()
  {
    for (unsigned int b = 0; b < BinCount(); b++)
      bins_[b] = 0;

    sampleCount_ = 0;
  }

  void HistogramAggregator::Aggregate(const IDataPointCollection& data, unsigned int index)
  {
    const DataPointCollection& concreteData = (const DataPointCollection&)(data);

    bins_[concreteData.GetIntegerLabel(index)]++;
    sampleCount_ += 1;
  }

  void HistogramAggregator::Aggregate(const HistogramAggregator& aggregator)
  {
    assert(aggregator.BinCount() == BinCount());
 
    for (unsigned int b = 0; b < BinCount(); b++)
      bins_[b] += aggregator.bins_[b];

    sampleCount_ += aggregator.sampleCount_;
  }

  HistogramAggregator HistogramAggregator::DeepClone() const
  {
    HistogramAggregator result(BinCount());

    for (unsigned int b = 0; b < BinCount(); b++)
      result.bins_[b] = bins_[b];

    result.sampleCount_ = sampleCount_;

    return result;
  }
}}}