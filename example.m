clear all;
close all;

%% Generate random 2D problem instance
rng(2); % seed
num_classes = 3;
num_example_per_class = 100;
sigma = 1e-2*eye(2);

train_features = single([]);
labels = uint8([]);
for c = 1:num_classes
	mu = rand(2,1);
	train_features = [train_features mvnrnd(mu,sigma,num_example_per_class)']; %#ok<AGROW>
						
	labels = [labels;
					 c*ones(num_example_per_class,1)]; %#ok<AGROW>
end

% Rescale features 
max_feature = 500;
train_features = train_features - min(train_features(:));
train_features = train_features/(max(train_features(:)));
train_features = train_features*max_feature;

% Test points densly
[x,y] = meshgrid(1:max_feature,1:max_feature);
test_features = single([x(:)';y(:)']);

%% Settings for training and testing
settings = SherwoodSettings();

% Each tree stores the results as histograms
% histogram (default): add all histogram over all trees and then calculate probability.
% probability: calculate probability in each tree and then average over the trees.
settings.TreeAggregator = 'histogram';
        
% Maximum depth of each tree
% Default: 5.
settings.MaxDecisionLevels = 5;

% Number of candidate feature response functions per split node
% Default: 10.
settings.NumberOfCandidateFeatures = 30;

% Optimal entropy split is determined by thresholding on 
% NumberOfCandidateThresholdsPerFeature equidistant points
% Default 10.
settings.NumberOfCandidateThresholdsPerFeature = 20;

% Number of trees in the forest
% Default: 30.
settings.NumberOfTrees = 100;

% Default: false.
settings.Verbose = false;

% Determine which weak learner to be used as a split function.
% Options {random-hyperplane, axis-aligned-hyperplane}
% Default: axis-aligned-hyperplane
settings.WeakLearner = 'random-hyperplane'; 

% Thread(s) used when training and testing.
% Default: 1
settings.MaxThreads =  feature('numThreads');

% The serialized forest will be saved and loaded from this filename
% Default: forest.bin
settings.ForestName = 'forest.bin';

% Automatic scaling; it is faster to normalize the features prior
% to using sherwood and settings this setting to false.
% Default: true
settings.FeatureScaling = true;

%% Training
t_t = tic;
% Size of the features matrix is
% number features x number of sampled points 
sherwood_train(train_features, labels, settings);
fprintf('Training time %g. \n', toc(t_t));

%% Classify (each thread will write it's own verbose message)
t_c = tic;
probabilities = sherwood_classify(test_features, settings);
fprintf('Classification time %g. \n', toc(t_c));

%% Display results
probability_im = zeros(max_feature,max_feature, num_classes);

colors = [1 0 0;
					0 1 0;
					0 0 1];
				
for c = 1:num_classes
	probability_im(:,:,c) = reshape(probabilities(c,:), [max_feature max_feature]);
end

figure(1); hold on;
imagesc(probability_im);
				
for c = 1:num_classes
	ind = labels == c;
	
	 plot(train_features(1,ind),train_features(2,ind),'o', ...
		 'MarkerFaceColor', colors(c,:), ...
		 'MarkerEdgeColor','k')
end
axis off;