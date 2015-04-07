#pragma once

struct Stats {
  float mean;
  float stdev;

  Stats() {
    mean = 0;
    stdev = 0;    
  }

  Stats(float mean, float stdev) 
  : mean(mean), stdev(stdev)
  {}

};

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

namespace MicrosoftResearch { namespace Cambridge { namespace Sherwood
{

enum WeakLearnType {AxisAligned, RandomHyperplane};
enum TreeAggregatorType {Histogram, Probability};

struct Options
{
  Options(MexParams params) 
  {
    MaxDecisionLevels = params.get<int>("MaxDecisionLevels", 5) - 1;
    NumberOfCandidateFeatures = params.get<int>("NumberOfCandidateFeatures", 10);
    NumberOfCandidateThresholdsPerFeature = params.get<int>("NumberOfCandidateThresholdsPerFeature", 1);
    MaxThreads = params.get<int>("MaxThreads", 1);
    NumberOfTrees = params.get<int>("NumberOfTrees", 30);

    FeatureScaling = params.get<bool>("FeatureScaling", true);
    Verbose = params.get<bool>("Verbose", false);

    ForestName = params.get<string>("ForestName", "forest.bin");  

    WeakLearnerStr = params.get<string>("WeakLearner", "axis-aligned-hyperplane"); 
    TreeAggregatorStr = params.get<string>("TreeAggregator", "histogram");

    if (WeakLearnerStr == "axis-aligned-hyperplane") {
      WeakLearner = AxisAligned;
    } else if (WeakLearnerStr == "random-hyperplane") {
      WeakLearner = RandomHyperplane;
    } else {
      mexErrMsgTxt("Unkown WeakLearner");
    }

    if (TreeAggregatorStr == "histogram") {
      TreeAggregator = Histogram;
    } else if (TreeAggregatorStr == "probability") {
      TreeAggregator = Probability;
    } else {
      mexErrMsgTxt("Unkown TreeAggregator");
    }

    if (WeakLearner == AxisAligned) {
      FeatureScaling = false;

      if (Verbose) {
        mexPrintf("Turning of feature scaling since it make no difference for axis-aligned-hyperplane weak learner.");
      }
    }
  };

  int MaxDecisionLevels;
  int NumberOfCandidateFeatures;
  int NumberOfCandidateThresholdsPerFeature;
  int NumberOfTrees;
  int MaxThreads;

  bool FeatureScaling;
  bool Verbose;
  string ForestName;

  TreeAggregatorType TreeAggregator;
  WeakLearnType WeakLearner;

  // Used for Verbose output
  string TreeAggregatorStr;
  string WeakLearnerStr;
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
    if (o.TreeAggregator == Histogram) {
      out << " TreeAggregator: Histogram" << std::endl; 
    } else {
      out << " TreeAggregator: Probability" << std::endl;
    }

    return out;
}

}}}