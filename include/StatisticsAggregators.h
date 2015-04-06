#pragma once
#include <math.h>
#include <limits>
#include "sherwood_mex.h"

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  struct HistogramAggregator
  {
  public:
    std::vector<unsigned int> bins_;
    unsigned int binCount_;
    unsigned int sampleCount_;

  public:

    double Entropy() const
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

    HistogramAggregator()
    {
      binCount_ = 0;
      sampleCount_ = 0;
    }

    HistogramAggregator(unsigned int nClasses) 
    {
      binCount_ = nClasses;
      
      bins_.resize(nClasses); 
      for(unsigned int b=0; b<binCount_; b++)
        bins_[b] = 0;

      sampleCount_ = 0;
    }

    float GetProbability(unsigned int classIndex) const
    {
      return (float)(bins_[classIndex]) / sampleCount_;
    }

    unsigned int BinCount() const {
      return binCount_; 
    }
    unsigned int SampleCount() const { 
      return sampleCount_; 
    }

    unsigned int FindTallestBinIndex() const
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
    void Clear()
    {
      for (unsigned int b = 0; b < BinCount(); b++)
        bins_[b] = 0;

      sampleCount_ = 0;
    }

    void Aggregate(const IDataPointCollection& data, unsigned int index)
    {
      const DataPointCollection& concreteData = (const DataPointCollection&)(data);

      bins_[concreteData.GetIntegerLabel(index)]++;
      sampleCount_ += 1;
    }

    void Aggregate(const HistogramAggregator& aggregator)
    {
      assert(aggregator.BinCount() == BinCount());
   
      for (unsigned int b = 0; b < BinCount(); b++)
        bins_[b] += aggregator.bins_[b];

      sampleCount_ += aggregator.sampleCount_;
    }

    HistogramAggregator DeepClone() const
    {
      HistogramAggregator result(BinCount());

      for (unsigned int b = 0; b < BinCount(); b++)
        result.bins_[b] = bins_[b];

      result.sampleCount_ = sampleCount_;

      return result;
    }
  };
} } }