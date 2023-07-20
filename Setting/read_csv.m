function read = read_csv(filename, dataLines)
%IMPORTFILE 从文本文件中导入数据
%  READ = IMPORTFILE(FILENAME)读取文本文件 FILENAME 中默认选定范围的数据。  以元胞数组形式返回数据。
%
%  READ = IMPORTFILE(FILE, DATALINES)按指定行间隔读取文本文件 FILENAME
%  中的数据。对于不连续的行间隔，请将 DATALINES 指定为正整数标量或 N×2 正整数标量数组。
%
%  示例:
%  read = importfile("D:\mat\mVeh_fun_tbm_20221221\read.csv", [2, Inf]);
%
%  另请参阅 READTABLE。
%
% 由 MATLAB 于 2023-03-06 11:41:26 自动生成

%% 输入处理

% 如果不指定 dataLines，请定义默认范围
if nargin < 2
    dataLines = [2, Inf];
end

%% 设置导入选项并导入数据
opts = delimitedTextImportOptions("NumVariables", 5);

% 指定范围和分隔符
opts.DataLines = dataLines;
opts.Delimiter = ",";

% 指定列名称和类型
opts.VariableNames = ["Transition", "Protocol", "Index", "Data", "Comment"];
opts.VariableTypes = ["char", "char", "char", "char", "char"];

% 指定文件级属性
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% 指定变量属性
opts = setvaropts(opts, ["Transition", "Protocol", "Index", "Data", "Comment"], "WhitespaceRule", "preserve");
opts = setvaropts(opts, ["Transition", "Protocol", "Index", "Data", "Comment"], "EmptyFieldRule", "auto");

% 导入数据
read = readtable(filename, opts);

%% 转换为输出类型
read = table2cell(read);
numIdx = cellfun(@(x) ~isnan(str2double(x)), read);
read(numIdx) = cellfun(@(x) {str2double(x)}, read(numIdx));
end
