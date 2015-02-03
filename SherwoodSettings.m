classdef SherwoodSettings
	
	% Default values
	properties (SetAccess = public)
		MaxDecisionLevels = 5;
		NumberOfCandidateFeatures = 10;
		NumberOfCandidateThresholdsPerFeature = 10;
		NumberOfTrees = 100;
		MaxThreads = 1;
		ForestName = 'forest.bin';
		Verbose = false;
		WeakLearner = 'axis-aligned-hyperplane';
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
		end
	end

	methods
		function self = SherwoodSettings()
			self.MaxThreads = feature('NumThreads');
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

		function self = set.MaxDecisionLevels(self, MaxDecisionLevels)
			MaxDecisionLevels = int32(MaxDecisionLevels);

			if (MaxDecisionLevels <= 1)
				error('MaxDecisionLevels must be > 1')
			end

			self.MaxDecisionLevels = MaxDecisionLevels;
		end

		function self = set.NumberOfCandidateFeatures(self, NumberOfCandidateFeatures)
			NumberOfCandidateFeatures = int32(NumberOfCandidateFeatures);

			if (NumberOfCandidateFeatures <= 1)
				error('NumberOfCandidateFeatures must be > 1')
			end

			self.NumberOfCandidateFeatures = NumberOfCandidateFeatures;
		end

		function self = set.NumberOfCandidateThresholdsPerFeature(self, NumberOfCandidateThresholdsPerFeature)
			NumberOfCandidateThresholdsPerFeature = int32(NumberOfCandidateThresholdsPerFeature);

			if (NumberOfCandidateThresholdsPerFeature <= 1)
				error('NumberOfCandidateThresholdsPerFeature must be > 1')
			end

			self.NumberOfCandidateThresholdsPerFeature = NumberOfCandidateThresholdsPerFeature;
		end		

		function self = set.NumberOfTrees(self, NumberOfTrees)
			NumberOfTrees = int32(NumberOfTrees);

			if (NumberOfTrees <= 1)
				error('NumberOfTrees must be > 1')
			end

			self.NumberOfTrees = NumberOfTrees;
		end		


		function self = set.MaxThreads(self, MaxThreads)
			MaxThreads = int32(MaxThreads);

			if (MaxThreads <= 1)
				error('MaxThreads must be > 1')
			end

			self.MaxThreads = MaxThreads;
		end	

		function self = set.Verbose(self, Verbose)
			self.Verbose = logical(Verbose);
		end	
	end
end