#pragma once
#include "StatisticsAggregators.h"
#include "Node.h"

// Every FeatureRespone which allocates on the heap needs 
// a specialized serializer and deserializer.
namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{
  template<typename T>
  void  binary_write(std::ostream& o, const T& value)
  {
    o.write(reinterpret_cast<const char*>(&value), sizeof(T));
  }

  template<typename T>
  void binary_read(std::istream& i, T& value){
      i.read(reinterpret_cast<char*>(&value), sizeof(T));
  }

  template<> 
  void Serialize_(std::ostream& o, const RandomHyperplaneFeatureResponse& feature)
  {
    binary_write(o, feature.dimensions);  

    for (unsigned int i = 0; i < feature.dimensions; i++)
      binary_write(o, feature.n[i]) ;
  }

  template<> 
    void Deserialize_(std::istream& o, RandomHyperplaneFeatureResponse& feature)
    { 
      binary_read(o, feature.dimensions);
      feature.n.resize(feature.dimensions);

      for (unsigned int i = 0; i < feature.dimensions; i++)
        binary_read(o, feature.n[i]);
    }


  template<>
  void Serialize_(std::ostream& o, const HistogramAggregator& S)
  {
    binary_write(o, S.binCount_);  
    binary_write(o, S.sampleCount_);  ;  

    for (unsigned int i = 0; i < S.binCount_; i++)
      binary_write(o, S.bins_[i]) ;
  }

  template<> 
  void Deserialize_(std::istream& o, HistogramAggregator& S)
  { 
    binary_read(o, S.binCount_);
    binary_read(o, S.sampleCount_);
    S.bins_.resize(S.binCount_);

    for (unsigned int i = 0; i < S.binCount_; i++)
      binary_read(o, S.bins_[i]);
  }
}
}
}
