% MATLAB wrapper for the c++ wrapper.
function [P, bins] = sherwood_classify(features, settings)

my_path = fileparts(mfilename('fullpath'));
addpath([my_path filesep 'include']);

% Convert
int_entries = {'MaxDecisionLevels','NumberOfCandidateFeatures',  ...
							 'NumberOfCandidateThresholdsPerFeature', 'NumberOfTrees', 'MaxThreads'};
for i = 1:numel(int_entries)
	if (isfield(settings,int_entries{i}))
			val = int32(getfield(settings,  int_entries{i}));
			settings = setfield(settings, int_entries{i},val);
	end
end

features = single(features);

true_entires = {'verbose'};
for i = 1:numel(true_entires)
	if (isfield(settings,true_entires{i}))
			val = logical(getfield(settings,  true_entires{i}));
			settings = setfield(settings, true_entires{i},val);
	end
end

if ~isfield(settings,'verbose')
	settings.verbose = false;
end

if ~isfield(settings,'MaxThreads')
	settings.MaxThreads = int32(1);
end

my_name = mfilename('fullpath');
my_path = fileparts(my_name);
cpp_file = 'sherwood_classify_mex.cpp';
[~,out_file] = fileparts(cpp_file);
out_file = ['include' filesep out_file];

% Includes etc
extra_arguments = {};
extra_arguments{end+1} = ['-I' my_path];
extra_arguments{end+1} = ['-I' my_path filesep 'include'];
extra_arguments{end+1} = ['-I' my_path filesep 'Sherwood' filesep 'cpp' filesep 'lib'];

% Additional files to be compiled.
sources = {	'DataPointCollection.cpp', ...
						'FeatureResponseFunctions.cpp', ...
						'StatisticsAggregators.cpp'};


% Only compile if files have changed
compile(cpp_file, out_file, sources, extra_arguments);

% Parallel
poolSize = matlabpool('size');
isOpen = poolSize > 0;

if ((settings.MaxThreads) > 1)
	if (isOpen)
		if (poolSize ~= settings.MaxThreads)
			warning(sprintf('Number of open matlabpool(s) %d while MaxThreads %d, running code with %d threads  \n', poolSize, settings.MaxThreads, poolSize));
		end
	else
		matlabpool(settings.MaxThreads);
	end
	
	sub_bins = cell(settings.MaxThreads,1);
	index_cut = round(linspace(0,size(features,2), settings.MaxThreads+1));
	indices = [index_cut(1:end-1)+1; index_cut(2:end)]';
	 
	% Matlab cannot assign to bins inside the parfor
	parfor thread = 1:settings.MaxThreads
		index = indices(thread,1):indices(thread,2);
		sub_bins{thread} = sherwood_classify_mex(features(:,index), settings);
	end
	
	if nargout == 1
		
		% Memory efficency.
		P = zeros( size(sub_bins{1},1),0,'single');	
		for thread = 1:settings.MaxThreads
			index = indices(thread,1):indices(thread,2);
			P(:,index) = sub_bins{thread};
			sub_bins{thread} = {};
		end
	
		clear sub_bins;

	else
		bins = [sub_bins{:}];
		clear sub_bins;
		P = single(bins);
	end
		
	
% Single thread
else
	if nargout == 1
		P = single(sherwood_classify_mex(features, settings));
	else
		bins = sherwood_classify_mex(features, settings);
		P = single(bins);
	end
	
end

denom = sum(P,1);
for i = 1:size(P,1)
	P(i,:) = single(P(i,:))./denom;
end

