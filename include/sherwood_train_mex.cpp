#include "sherwood_mex.h"

#if USE_OPENMP == 1
#include <omp.h>
#endif

using namespace MicrosoftResearch::Cambridge::Sherwood;

// F: Feature Response
// S: StatisticsAggregator
template<typename F, typename S>
void sherwood_train(int nlhs, 		    /* number of expected outputs */
        mxArray        *plhs[],	    /* mxArray output pointer array */
        int            nrhs, 		/* number of inputs */
        const mxArray  *prhs[],		/* mxArray input pointer array */
				Options options)
{
	unsigned int curarg = 0;

	// Features along rows
	// Examples along columns
	const matrix<float> features = prhs[curarg++];
	const matrix<unsigned char> labels 	= prhs[curarg++];

  // Supervised classification
  TrainingParameters trainingParameters;
  trainingParameters.MaxDecisionLevels = options.MaxDecisionLevels;
  trainingParameters.NumberOfCandidateFeatures = options.NumberOfCandidateFeatures;
  trainingParameters.NumberOfCandidateThresholdsPerFeature = options.NumberOfCandidateThresholdsPerFeature;
  trainingParameters.NumberOfTrees = options.NumberOfTrees;
  trainingParameters.Verbose = false;

	// Point class
	DataPointCollection trainingData(features,labels);  

	if (options.Verbose) {
		mexPrintf("Training data has: %d features %d classes and %d examples.\n",
              trainingData.Dimensions(), trainingData.CountClasses(), trainingData.Count());
  }

  Random random;

  LinearFeatureFactory<F> featureFactory(trainingData.Dimensions());

	ClassificationTrainingContext<F> 
		classificationContext(trainingData.CountClasses(), &featureFactory);

  // Without OPENMP no multi threading.
  #if USE_OPENMP == 0
    if (options.MaxThreads > 1) {
      mexPrintf("Compiled without OpenMP flags, falling back to single thread code.\n");
    }

    options.MaxThreads = 1;
  #endif

  std::auto_ptr<Forest<F, S> > forest ;

	// Create forest
  if (options.MaxThreads == 1)
  {
    ProgressStream progressStream(std::cout, Silent);

    mexPrintf("Using 1 thread.\n");
  
    forest = ForestTrainer<F, S>::TrainForest 
    (random, trainingParameters, classificationContext, trainingData, &progressStream );
  }

  // Parallel
  // ParallelForestTrainer.h segfaults using gcc.
  else 
  {
    #if USE_OPENMP == 1
      omp_set_num_threads(options.MaxThreads);

      unsigned int current_num_threads = 0;
      if (options.Verbose)
      {
        int current_num_threads;

        #pragma omp parallel
          current_num_threads = omp_get_num_threads();

        mexPrintf("Using OpenMP with %d threads (maximum %d) \n ",current_num_threads,omp_get_max_threads());
      }

      forest = std::auto_ptr<Forest<F,S> >(new Forest<F,S>());
      
      omp_lock_t writelock;
      omp_init_lock(&writelock);

      #pragma omp parallel for
      for (int t = 0; t < trainingParameters.NumberOfTrees; t++)
      {
        std::auto_ptr<Tree<F,S> > tree = TreeTrainer<F,S>::TrainTree(random, 
            classificationContext, trainingParameters, trainingData);

        omp_set_lock(&writelock);
        forest->AddTree(tree);
        omp_unset_lock(&writelock);
      }

      omp_destroy_lock(&writelock);

    #endif
  }

  // Saving the forest
  std::ofstream o(options.ForestName.c_str(), std::ios_base::binary);
	forest->Serialize(o);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray  *prhs[])
{
	MexParams params(1, prhs+2);
	Options options(params);

  if (!options.WeakLearner.compare("axis-aligned-hyperplane"))
    sherwood_train<AxisAlignedFeatureResponse, HistogramAggregator>(nlhs, plhs, nrhs, prhs, options);
  else if (!options.WeakLearner.compare("random-hyperplane"))
    sherwood_train<RandomHyperplaneFeatureResponse, HistogramAggregator>(nlhs, plhs, nrhs, prhs, options);
  else
   mexErrMsgTxt("Unknown weak learner. Supported are: axis-aligned-hyperplane and random-hyperplane");
}