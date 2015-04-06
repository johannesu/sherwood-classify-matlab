#include "sherwood_mex.h"

using namespace MicrosoftResearch::Cambridge::Sherwood;

// F: Feature Response
// S: StatisticsAggregator
template<typename F, typename S>
void sherwood_classify(int nlhs, 		    /* number of expected outputs */
        mxArray        *plhs[],	    /* mxArray output pointer array */
        int            nrhs, 		/* number of inputs */
        const mxArray  *prhs[],		/* mxArray input pointer array */
        Options options)
{
	unsigned int curarg = 0;
	const matrix<float> features = prhs[curarg++];

  if (options.Verbose) {
    mexPrintf("Loading tree at: %s\n", options.ForestName.c_str());
  }
 
	// Point class
	DataPointCollection testData(features);  

	// Create the tree.
	std::auto_ptr<Forest<F, S> > forest;

	// Load the tree from file 
  std::ifstream istream(options.ForestName.c_str(), std::ios_base::binary);
  forest = forest->Deserialize(istream);

  unsigned int num_classes = forest->GetTree(0).GetNode(0).TrainingDataStatistics.BinCount();

  if (options.Verbose) 
  {
    mexPrintf("Number of classes: %d\n", num_classes);
    mexPrintf("Number of test data: %d\n", testData.Count());
  }

  // Output ordered as (class, index)
  matrix<double> output(num_classes,testData.Count());

  for (int i = 0; i < output.numel(); i++) {
    output(i) = 0;
  }
  
  // Perform classification
  // forest::apply is wasting memory, bypassing it.
  //
  // Note: leafNodeIndices should unsigned int, modify tree.h and the line below.
  //
  std::vector<int> leafNodeIndices;

  // Forest.h (Apply)
  for (unsigned int t = 0; t < forest->TreeCount(); t++)
  {
    Tree<F,S>& tree = forest->GetTree(t);

    // Tree.h
    tree.Apply(testData, leafNodeIndices);

    // Aggregate histograms
    if (options.TreeAggregator == Histogram) {

      for (unsigned int i = 0; i < testData.Count(); i++)
      { 
        S aggregator = tree.GetNode(leafNodeIndices[i]).TrainingDataStatistics;

        for (unsigned int c = 0; c < num_classes; c++) {
          output(c,i) += aggregator.bins_[c];
        }
      }
 
    // Aggregate probabilities
    } else if (options.TreeAggregator ==  Probability) {
      for (unsigned int i = 0; i < testData.Count(); i++)
      { 
        S aggregator = tree.GetNode(leafNodeIndices[i]).TrainingDataStatistics;

        for (unsigned int c = 0; c < num_classes; c++) {
          output(c,i) += aggregator.GetProbability(c);
        }
      }

    } else {
      mexErrMsgTxt("Unkown aggregator.");
    }

    leafNodeIndices.clear();
  }

  plhs[0] = output;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray  *prhs[])
{
  MexParams params(1, prhs+1);
  Options options(params);

  if (!options.WeakLearner.compare("axis-aligned-hyperplane"))
    sherwood_classify<AxisAlignedFeatureResponse, HistogramAggregator>(nlhs, plhs, nrhs, prhs, options);
  else if (!options.WeakLearner.compare("random-hyperplane"))
    sherwood_classify<RandomHyperplaneFeatureResponse, HistogramAggregator>(nlhs, plhs, nrhs, prhs, options);
  else 
    mexErrMsgTxt("Unknown weak learner. Supported are: axis-aligned-hyperplane and random-hyperplane");
  
}