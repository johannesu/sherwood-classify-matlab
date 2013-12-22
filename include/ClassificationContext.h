template<class F>
class IFeatureResponseFactory
{
public:
  virtual F CreateRandom(Random& random)=0;
};

template<class F>
class ClassificationTrainingContext : public ITrainingContext<F,HistogramAggregator> // where F:IFeatureResponse
{
private:
  unsigned int nClasses_;

  IFeatureResponseFactory<F>* featureFactory_;

public:
  ClassificationTrainingContext(unsigned int nClasses, IFeatureResponseFactory<F>* featureFactory)
  {
    nClasses_ = nClasses;
    featureFactory_ = featureFactory;
  }

private:
  // Implementation of ITrainingContext
  F GetRandomFeature(Random& random)
  {
    return featureFactory_->CreateRandom(random);
  }

  HistogramAggregator GetStatisticsAggregator()
  {
    return HistogramAggregator(nClasses_);
  }

  double ComputeInformationGain(const HistogramAggregator& allStatistics, const HistogramAggregator& leftStatistics, const HistogramAggregator& rightStatistics)
  {
    double entropyBefore = allStatistics.Entropy();

    unsigned int nTotalSamples = leftStatistics.SampleCount() + rightStatistics.SampleCount();

    if (nTotalSamples <= 1)
      return 0.0;

    double entropyAfter = (leftStatistics.SampleCount() * leftStatistics.Entropy() + rightStatistics.SampleCount() * rightStatistics.Entropy()) / nTotalSamples;

    return entropyBefore - entropyAfter;
  }

  bool ShouldTerminate(const HistogramAggregator& parent, const HistogramAggregator& leftChild, const HistogramAggregator& rightChild, double gain)
  {
    return gain < 0.01;
  }
};