% Compile files only if needed
function compile(cpp_file, out_file, sources, extra_arguments)
% Process
filename = mfilename('fullpath');
my_path = fileparts(filename);

mex_file_name = [my_path filesep '..' filesep out_file '.' mexext];
cpp_file_name = [my_path filesep cpp_file];

mex_file = dir(mex_file_name);
cpp_file = dir(cpp_file_name);

% Cpp file not found returning
if (isempty(cpp_file))
	error('C++ file not found');
	return;
end


% If modified or not existant compile
compile_file = false;
if ~exist(mex_file_name, 'file')
	compile_file = true;
end

% Uncomment to recompile each time the cpp file is modified.
% if length(mex_file) == 1
% 	mex_modified = mex_file.datenum;
% else
% 	mex_modified = 0;
% end
% 
% cpp_modified   = cpp_file.datenum;
% if mex_modified < cpp_modified
% 	compile_file = true;
% end

include_folders = {};

% Append current folder to sources
for i = 1 : length(sources);
	include_folders{i} = ['-I' my_path filesep fileparts(sources{i}) filesep];
	sources{i} = [my_path filesep sources{i}];
end

% Check all dependend files
for i = 1 : length(sources)
	cpp_file = dir(sources{i});
	
	if (isempty(cpp_file))
		error(sprintf('File not found: %s', sources{i}));
	end
	
% 	cpp_modified = cpp_file.datenum;
% 	if mex_modified < cpp_modified
% 		compile_file = true;
% 	end
end

if compile_file
	%mex(cpp_file_name,'-outdir',my_path, args{:});
		
	mex_argument = {'mex', ...
		cpp_file_name,  ...
		'-outdir', ...
		my_path, ...
		'-largeArrayDims', ...
		extra_arguments{:},...
		include_folders{:}, ...
		sources{:}};
	
	% Spaceing needed
	for i = 1:numel(mex_argument)
		mex_argument{i} = [mex_argument{i} ' '];
	end
	
	% Using eval to allow for arguments changing c and c++ flags
	eval([mex_argument{:}])
end
