function outputArg1 = merge_num(input_num)
%UNTITLED �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
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

