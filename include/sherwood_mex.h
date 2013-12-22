#pragma once
#include "mex.h"
#include "mexutils.h"
#include "cppmatrix.h" 
#include "Sherwood.h"
#include "ProgressStream.h"
#include "DataPointCollection.h"
#include "StatisticsAggregators.h"
#include "ClassificationContext.h"
#include "FeatureResponseFunctions.h"
#include "Serialize.h"
#include <iostream>

struct Options
{
  Options(MexParams params) 
  {
    MaxDecisionLevels = params.get<int>("MaxDecisionLevels", 5) - 1;
    NumberOfCandidateFeatures = params.get<int>("NumberOfCandidateFeatures", 10);
    NumberOfCandidateThresholdsPerFeature = params.get<int>("NumberOfCandidateThresholdsPerFeature", 1);
    NumberOfTrees = params.get<int>("NumberOfTrees", 30);

    verbose = params.get<bool>("verbose", false);
    forestName = params.get<string>("forestName", "forest.bin");  
    WeakLearner = params.get<string>("WeakLearner", "axis-aligned-hyperplane"); 
    MaxThreads = params.get<int>("MaxThreads", 1);
  };

  int MaxDecisionLevels;
  int NumberOfCandidateFeatures;
  int NumberOfCandidateThresholdsPerFeature;
  int NumberOfTrees;
  int MaxThreads;

  bool verbose;
  string forestName;
  string WeakLearner;
};
  

std::ostream& operator<<(std::ostream &out, const Options& o)
{
    out << " Training parameters:" <<std::endl;
    out << " WeakLearner: (Default: axis-aligned-hyperplane): " << o.WeakLearner << std::endl;
    out << " MaxDecisionLevels (Max Tree depth, default: 5): " 
              << o.MaxDecisionLevels +1 << std::endl;
    out << " NumberOfTrees: (Default: 30): " 
              << o.NumberOfTrees << std::endl;
    out  << " NumberOfCandidateFeatures (No. of candidate feature response functions per split node, default: 10): " 
      <<   o.NumberOfCandidateFeatures << std::endl;
    out << " NumberOfCandidateThresholdsPerFeature (No. of candidate thresholds per feature response function default: 1): " 
    <<  o.NumberOfCandidateThresholdsPerFeature << std::endl;
    out << " MaxThreads (Default: 1): " << o.MaxThreads << std::endl;
    return out;
}
