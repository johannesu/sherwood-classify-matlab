% MATLAB wrapper for the c++ wrapper.
function sherwood_train(features,labels, settings)

% Set to true to allow OpenMP support.
% --
% Windows note: MatLabs recommended compiler:
% (Microsoft Software Development Kit (SDK) 7.1)
% Does not have support for OpenMP, see
% http://openmp.org/wp/openmp-compilers/.
%
% For Matlab 2013a:
% Visual Studio Pro 2012  has OpenMP support:
% Note visual studio version support depends on your Matlab version. 
% Some work around exist see e.g
% http://www.mathworks.com/matlabcentral/fileexchange/44408-matlab-mex-support-for-visual-studio-2013-and-mbuild
% --
use_openmp = true;

my_path = fileparts(mfilename('fullpath'));
addpath([my_path filesep 'include']);

% Check input
if (any(diff(unique(labels))-1))
	error(sprintf('Labels index should be 1,2,...,n \nAnd all label index should have atleast one example.'));
end

if (min(labels(:)) ~=1)
	error('Labels ids must start at 1');
end

if (size(features,2) ~= numel(labels))
	error('Number of columns in feature vector (number of exampels) must be same as length of labels')
end

% Convert
int_entries = {'MaxDecisionLevels','NumberOfCandidateFeatures',  ...
							 'NumberOfCandidateThresholdsPerFeature', 'NumberOfTrees', 'MaxThreads'};
for i = 1:numel(int_entries)
	if (isfield(settings,int_entries{i}))
			val = int32(getfield(settings,  int_entries{i}));
			settings = setfield(settings, int_entries{i},val);
	end
end

if ~isa(features,'single')
	fprintf('Sherwood features uses single precision (floats), converting features matrix \n');
	features = single(features);
end

if ~isa(labels,'uint8')
	fprintf('Sherwood labels uses unsigned ints (uint8), converting labels matrix \n');
	labels = uint8(labels);
end
true_entires = {'verbose'};
for i = 1:numel(true_entires)
	if (isfield(settings,true_entires{i}))
			val = logical(getfield(settings,  true_entires{i}));
			settings = setfield(settings, true_entires{i},val);
	end
end

my_name = mfilename('fullpath');
my_path = fileparts(my_name);

cpp_file = 'sherwood_train_mex.cpp';
[~,out_file] = fileparts(cpp_file);
out_file = ['include' filesep out_file];

% Includes etc
extra_arguments = {};
extra_arguments{end+1} = ['-I' my_path];
extra_arguments{end+1} = ['-I' my_path filesep 'include'];
extra_arguments{end+1} = ['-I' my_path filesep 'Sherwood' filesep 'cpp' filesep 'lib'];


if (use_openmp)
	if ~ispc
		extra_arguments{end+1} = '-lgomp';
		extra_arguments{end+1} = 'CXXFLAGS="\$CXXFLAGS -fopenmp"';
	else
		extra_arguments{end+1} = 'COMPFLAGS="$COMPFLAGS /openmp"';	
	end
	
	extra_arguments{end+1} = '-DUSE_OPENMP=1';
else
	extra_arguments{end+1} = '-DUSE_OPENMP=0';
end

% Additional files to be compiled.
sources = {'DataPointCollection.cpp', ...
						'FeatureResponseFunctions.cpp', ...
						'StatisticsAggregators.cpp'};


% Only compile if files have changed
compile(cpp_file, out_file, sources, extra_arguments);

% Labels from 0 in c++ code.
sherwood_train_mex(features,labels-1, settings);
