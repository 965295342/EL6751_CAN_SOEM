function outputArg1 = merge_num(input_num)
%UNTITLED 此处显示有关此函数的摘要
%   此处显示详细说明
    outputArg1 = {''};
    blank ={' '};
    for i = 1:length(input_num)
        outputArg1(1,1) = strcat(outputArg1(1,1),input_num(i));
%         outputArg1(1,1) = strcat(outputArg1(1,1), ' 0');
        if (i<length(input_num))
            outputArg1(1,1) = strcat(outputArg1(1,1), blank);
        end
    end   
end

