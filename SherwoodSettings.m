classdef SherwoodSettings
	
	% Default values
	properties (SetAccess = public)
		% Maximum depth of each tree
		MaxDecisionLevels = int32(5);

		% Number of candidate feature response functions per split node
		NumberOfCandidateFeatures = int32(10);

		% Optimal entropy split is determined by thresholding on 
		% NumberOfCandidateThresholdsPerFeature equidistant points
		NumberOfCandidateThresholdsPerFeature = int32(10);

		% Number of trees in the forest
		NumberOfTrees = int32(30);

		% Thread(s) used when training and testing.
		MaxThreads = int32(1);

		% The serialized forest will be saved and loaded from this filename	
		ForestName = 'forest.bin';

		% Verbose output during progress of the algorithm
		Verbose = false;

		% Determine which weak learner to be used as a split function.
		% Options {random-hyperplane, axis-aligned-hyperplane}
		WeakLearner = 'random-hyperplane';

		% Each tree stores the results as histograms
		% histogram (default): add all histogram over all trees and then calculate probability.
		% probability: calculate probability in each tree and then average over the trees.
		TreeAggregator = 'histogram';

		% Automatic scaling; it is faster to normalize the features prior
		% to using sherwood and settings this setting to false.
		FeatureScaling = true;
	end
		
	methods (Hidden)
		function settings = generate_struct(self)
			settings.MaxDecisionLevels = self.MaxDecisionLevels;
			settings.NumberOfCandidateFeatures = self.NumberOfCandidateFeatures;
			settings.NumberOfCandidateThresholdsPerFeature = self.NumberOfCandidateThresholdsPerFeature;
			settings.NumberOfTrees = self.NumberOfTrees;
			settings.MaxThreads = self.MaxThreads;
			settings.ForestName = self.ForestName;
			settings.WeakLearner = self.WeakLearner;
			settings.Verbose = self.Verbose;
			settings.FeatureScaling = self.FeatureScaling;
			settings.TreeAggregator = self.TreeAggregator;
		end
	end

	methods
		function self = SherwoodSettings()
			self.MaxThreads = feature('NumThreads');
        end

         % Training on either settings file will give the same result (excluding the
        % random seed).
        function equvialent = training_equvialent_settings(self, other)
            equvialent = true;
            
            if (self.MaxDecisionLevels ~= other.MaxDecisionLevels)
                equvialent = false;
                return
            end
            
            if (self.NumberOfCandidateFeatures ~= other.NumberOfCandidateFeatures)
                equvialent = false;
                return
            end

            if (self.NumberOfCandidateThresholdsPerFeature ~= other.NumberOfCandidateThresholdsPerFeature)
                equvialent = false;
               return
            end
  
            if (self.NumberOfTrees ~= other.NumberOfTrees)
                equvialent = false;
                return
            end
            
            if (~strcmp(self.WeakLearner, other.WeakLearner))
                equvialent = false;
                return
			end

        end
        
		% Set functions
		function self = set.WeakLearner(self, WeakLearner)
			switch(WeakLearner)
				case 'axis-aligned-hyperplane'
					self.WeakLearner = 'axis-aligned-hyperplane';
				case 'random-hyperplane'
					self.WeakLearner = 'random-hyperplane';
				otherwise	
					error('WeakLearner supported: axis-aligned-hyperplane, random-hyperplane');
			end
		end

		function self = set.TreeAggregator(self, TreeAggregator)
			switch(TreeAggregator)
				case 'histogram'
					self.TreeAggregator = 'histogram';
				case 'probability'
					self.TreeAggregator = 'probability';
				otherwise	
					error('TreeAggregator available: histogram, probability');
			end
		end

		function self = set.MaxDecisionLevels(self, MaxDecisionLevels)
			MaxDecisionLevels = int32(MaxDecisionLevels);

			if (MaxDecisionLevels < 2)
				error('MaxDecisionLevels must be >= 2')
			end

			self.MaxDecisionLevels = MaxDecisionLevels;
		end

		function self = set.NumberOfCandidateFeatures(self, NumberOfCandidateFeatures)
			NumberOfCandidateFeatures = int32(NumberOfCandidateFeatures);

			if (NumberOfCandidateFeatures < 1)
				error('NumberOfCandidateFeatures must be >= 1')
			end

			self.NumberOfCandidateFeatures = NumberOfCandidateFeatures;
		end

		function self = set.NumberOfCandidateThresholdsPerFeature(self, NumberOfCandidateThresholdsPerFeature)
			NumberOfCandidateThresholdsPerFeature = int32(NumberOfCandidateThresholdsPerFeature);

			if (NumberOfCandidateThresholdsPerFeature < 1)
				error('NumberOfCandidateThresholdsPerFeature must be >= 1')
			end

			self.NumberOfCandidateThresholdsPerFeature = NumberOfCandidateThresholdsPerFeature;
		end		

		function self = set.NumberOfTrees(self, NumberOfTrees)
			NumberOfTrees = int32(NumberOfTrees);

			if (NumberOfTrees < 1)
				error('NumberOfTrees must be >= 1')
			end

			self.NumberOfTrees = NumberOfTrees;
		end		


		function self = set.MaxThreads(self, MaxThreads)
			MaxThreads = int32(MaxThreads);

			if (MaxThreads < 1)
				error('MaxThreads must be >= 1')
			end

			self.MaxThreads = MaxThreads;
		end	

		function self = set.Verbose(self, Verbose)
			self.Verbose = logical(Verbose);
		end	

		function self = set.FeatureScaling(self, FeatureScaling)
			self.FeatureScaling = logical(FeatureScaling);
		end
	end
end
