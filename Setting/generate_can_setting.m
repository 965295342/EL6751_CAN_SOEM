function   generate_can_setting()
%GENERATE_CAN_SETTING 此处显示有关此函数的摘要
%   此处显示详细说明clear;
clear;
clc;
[dbcFileName,dbcFilePath] = uigetfile('*.dbc', 'select a .dbc file');       % user 选择dbc文件
if isequal(dbcFileName,0)
    disp('User selected Cancel');
else
    disp(['User selected ', fullfile(dbcFilePath,dbcFileName)]);
end
dbcHandle = canDatabase(fullfile(dbcFilePath,dbcFileName));                                 %读取dbc文件, Estella found this from CSDN (Chinese github)
msgInfo = dbcHandle.messageInfo;
temp_dbcMsgs = dbcHandle.Messages;
%读取此数据库中定义的所有报文的名称，cell数组变量，可用c.Messages{i}索引第i个报文名称

[send_msgs_seltd,send_temp_tf_] = listdlg('PromptString', 'Select message to send', 'ListString',temp_dbcMsgs);  %user select message(s), tf: t: made selection, false made no. ending _ meaning variable not used;
[read_msgs_seltd,read_temp_tf_] = listdlg('PromptString', 'Select message to read', 'ListString',temp_dbcMsgs);  %user select message(s), tf: t: made selection, false made no. ending _ meaning variable not used;
sendMsg = struct('id',0,'COB_ID',0,'Txnode',{},'cycle_time',0,'DLC',0);
readMsg = struct('id',0,'COB_ID',0,'Txnode',{},'DLC',0,'uniqueRxNodes',{},'isUse',{});
sendMsgs = repmat(sendMsg,[1,length(send_msgs_seltd)]);%提前分配内存
readMsgs = repmat(readMsg,[1,length(read_msgs_seltd)]);
sendTxNode = cell(0);

for i = 1 : length(send_msgs_seltd)
    id = send_msgs_seltd(i);
    sendMsgs(i).id = id;
    sendMsgs(i).COB_ID = msgInfo(id).ID; % 找到对应信号的ID
    sendMsgs(i).Txnode = msgInfo(id).TxNodes; % 找到message对应的node
    sendMsgs(i).DLC = msgInfo(id).DLC;%data length
    sendTxNode(i) = msgInfo(id).TxNodes;
    attri_info = msgInfo(id).AttributeInfo; % 包括cycletime等一系列属性
    for j = 1 : length(attri_info)
        if (strcmp(attri_info(j).Name, 'GenMsgCycleTime'))
            sendMsgs(i).cycle_time = attri_info(j).Value;
            break;
        end
    end
    %sendMsgs(i).cycle_time = attri_info(7).Value;
end

send_TxNode_Unique_Num = length(unique(sendTxNode));
sendTxNode = unique(sendTxNode);
for i = 1 : length(read_msgs_seltd)
    id = read_msgs_seltd(i);
    readMsgs(i).id = id;
    readMsgs(i).COB_ID = msgInfo(id).ID; % 找到对应信号的ID
    readMsgs(i).Txnode = msgInfo(id).TxNodes; % 找到message对应的node
    readMsgs(i).DLC = msgInfo(id).DLC;%data length
    %        readMsgs(i).signalInfo = {msgInfo(id).SignalInfo.RxNodes{:}};
    %        readMsgs(i).signalInfo = unique(readMsgs(i).signalInfo);
    rxNodes = {};
    for j = 1:numel(msgInfo(id).SignalInfo)
        rxNodes = [rxNodes;msgInfo(id).SignalInfo(j).RxNodes(:)];
    end
    readMsgs(i).uniqueRxNodes = unique([rxNodes]);
end

[csvFileName,csvFilePath] = uigetfile('*.csv', 'select a .csv file');       % user 选择csv文件
if isequal(csvFileName,0)
    disp('User selected Cancel');
else
    disp(['User selected ', fullfile(csvFilePath,csvFileName)]);
end
csv_handle = read_csv(fullfile(csvFilePath,csvFileName));%读入原始CSV文件
reg_0x80n0 = '';%0x8000的内容

for i = 1:length(csv_handle)
    if(strncmp(csv_handle(i,3),'0x8000',6))%获得0x8000的内容,csv文件至少有0x8000的内容
        reg_0x80n0 = csv_handle(i,4);
        break;
    end
end

reg_0x80n0 = split(reg_0x80n0);

sendHashMap = containers.Map();%统计sendMsgs各node有几个message
readHashMap = containers.Map();%统计readMsgs各node有几个message
indexHashMap = containers.Map();%统计各node在什么位置

for i = 1:length(sendTxNode)%map初始化
    sendHashMap(string(sendTxNode(i))) = 0;
    readHashMap(string(sendTxNode(i))) = 0;
end

for i = 1:length(sendMsgs)
    sendHashMap(string(sendMsgs(i).Txnode)) = sendHashMap(string(sendMsgs(i).Txnode)) + 1;
    for j = 1:length(readMsgs)
        RxNodes = readMsgs(j).uniqueRxNodes;
        checkExist = sum(ismember(RxNodes,sendMsgs(i).Txnode));
        
        %readHashMap(string(readMsgs(i).node)) = readHashMap(string(readMsgs(i).COB_ID)) + checkExist;
        if checkExist>0 && length(readMsgs(j).isUse)<=0
            readHashMap(string(sendMsgs(i).Txnode)) = readHashMap(string(sendMsgs(i).Txnode)) + 1;
            readMsgs(j).isUse = sendMsgs(i).Txnode;
        end
    end
end

readNodeEmpty = 0;
for i = 1:length(readMsgs)
    if isempty(readMsgs(i).isUse)
        readNodeEmpty = readNodeEmpty + 1;
    end
end

read_Node_Unique_Num = send_TxNode_Unique_Num + 0;

sendTxNode_num = length(sendTxNode);

for i = 1:length(sendTxNode)%先完成0x8000，0x8000
    reg_0x80n0(3,1)= {dec2hex(i,2)};
    csv_handle(i*4,1) = {'<PS>'};
    csv_handle(i*4,2) = {'CoE'};
    csv_handle(i*4,3) = {strcat(strcat('0x',dec2hex(0x8000 + 16*(i-1))),' C 0')};
    csv_handle(i*4,4) = merge_num(reg_0x80n0);%配置好0x8000+16*n的数据
    csv_handle(i*4,5) = {'EL67xx CoE Init Cmd '};
    
    reg_0x80n8 = cell(2 + 12 * sendHashMap(string(sendTxNode(i))),1);
    reg_0x80n8(:) = {'00'};
    readHashMap(string(sendTxNode(i)))
    reg_0x80n6 = cell(2 + 12 * readHashMap(string(sendTxNode(i))),1);
    reg_0x80n6(:) = {'00'};
    
    if length(reg_0x80n6) ~= 2 %配置0x80n6
        reg_0x80n6(1,1) = {dec2hex(readHashMap(string(sendTxNode(i))),2)};
        subindex = 3;
        for j = 1:length(readMsgs)
            if string(readMsgs(j).isUse) == string(sendTxNode(i))
                COB_ID = readMsgs(j).COB_ID;
                reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
                COB_ID = floor(double(COB_ID) / 256);
                subindex = subindex + 1;
                reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
                COB_ID = floor(double(COB_ID) / 256);
                subindex = subindex + 1;
                reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
                COB_ID = floor(double(COB_ID) / 256);
                subindex = subindex + 1;
                reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
                subindex = subindex + 1;
                reg_0x80n6(subindex) = {'FF'};
                subindex = subindex + 1;
                reg_0x80n6(subindex) = {dec2hex(readMsgs(j).DLC,2)};
                subindex = subindex + 7;
            end
        end
    end
    csv_handle(1+i*4,1) = {'<PS>'};
    csv_handle(1+i*4,2) = {'CoE'};
    csv_handle(1+i*4,3) = {strcat(strcat('0x',dec2hex(0x8006 + 16*(i-1))),' C 0')};
    csv_handle(1+i*4,4) = merge_num(reg_0x80n6);
    csv_handle(1+i*4,5) = {'EL67xx CoE Init Cmd '};
    
    
    if length(reg_0x80n8) ~= 2 %配置0x80n8
        reg_0x80n8(1,1) = {dec2hex(sendHashMap(string(sendTxNode(i))),2)};
        subindex = 3;
        indexHashMap(string(sendTxNode(i))) = i;
        for j = 1:length(sendMsgs)
            if string(sendMsgs(j).Txnode) == string(sendTxNode(i))
                COB_ID = sendMsgs(j).COB_ID;
                reg_0x80n8(subindex) = {dec2hex(mod(COB_ID,256),2)};
                subindex = subindex + 1;
                COB_ID = floor(double(COB_ID) / 256);
                reg_0x80n8(subindex) = {dec2hex(mod(COB_ID,256),2)};
                subindex = subindex + 1;
                COB_ID = floor(double(COB_ID) / 256);
                reg_0x80n8(subindex) = {dec2hex(mod(COB_ID,256),2)};
                subindex = subindex + 1;
                COB_ID = floor(double(COB_ID) / 256);
                reg_0x80n8(subindex) = {dec2hex(mod(COB_ID,256),2)};
                subindex = subindex + 1;
                reg_0x80n8(subindex) = {'FF'};
                subindex = subindex + 1;
                reg_0x80n8(subindex) = {dec2hex(sendMsgs(j).DLC,2)};
                subindex = subindex + 3;
                reg_0x80n8(subindex) = {dec2hex(sendMsgs(j).cycle_time,2)};
                subindex = subindex + 4;
            end
        end
    end
    csv_handle(2+i*4,1) = {'<PS>'};
    csv_handle(2+i*4,2) = {'CoE'};
    csv_handle(2+i*4,3) = {strcat(strcat('0x',dec2hex(0x8008 + 16*(i-1))),' C 0')};
    csv_handle(2+i*4,4) = merge_num(reg_0x80n8);
    csv_handle(2+i*4,5) = {'EL67xx CoE Init Cmd '};
    
    csv_handle(3+i*4,1) = {'<PS>'};
    csv_handle(3+i*4,2) = {'CoE'};%0x80n3意义不明，目前置0
    csv_handle(3+i*4,3) = {strcat(strcat('0x',dec2hex(0x8003 + 16*(i-1))),' C 0')};
    csv_handle(3+i*4,4) = {'00 00'};
    csv_handle(3+i*4,5) = {'EL67xx CoE Init Cmd '};
end

offset = length(sendTxNode);

for i = 1:length(readMsgs)
    if isempty(readMsgs(i).isUse)
        node = {};
        for j = 1:length(readMsgs(i).uniqueRxNodes)
            if isKey(indexHashMap,string(readMsgs(i).uniqueRxNodes(j)))
                node = string(readMsgs(i).uniqueRxNodes(j));
                break;
            end
        end
        if ~isempty(node)
            readMsgs(i).isUse = node;
            index = indexHashMap(string(node));%indexHashMap
            reg_0x80n6 = cell(2 + 12 * readHashMap(string(node)),1);
            reg_0x80n6(:) = {'00'};
            reg_0x80n6(1,1) = {dec2hex(readHashMap(string(node)),2)};
            subindex = 3;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            reg_0x80n6(subindex) = {'FF'};
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {dec2hex(readMsgs(i).DLC,2)};
            subindex = subindex + 7;
            csv_handle(1+index*4,1) = {'<PS>'};
            csv_handle(1+index*4,2) = {'CoE'};
            csv_handle(1+index*4,3) = {strcat(strcat('0x',dec2hex(0x8006 + 16*(index-1))),' C 0')};
            csv_handle(1+index*4,4) = merge_num(reg_0x80n6);
            csv_handle(1+index*4,5) = {'EL67xx CoE Init Cmd '};
        end
    end
end

for i = 1 : length(readMsgs)
    if isempty(readMsgs(i).isUse)
       if length(readMsgs(1).uniqueRxNodes)>1
           node = readMsgs(i).uniqueRxNodes(1);%默认使用第一个，fix，可能为空造成报错
       else
              node = {'A'};
       end
        %read_Node_Unique_Num = read_Node_Unique_Num + 1;
        if isKey(readHashMap,string(node))
            readHashMap(string(node)) = readHashMap(string(node)) + 1;
            index = indexHashMap(string(node));
            reg_0x80n6 = csv_handle(1+index * 4,4);
            reg_0x80n6 = split(reg_0x80n6);
            reg_0x80n6(1,1) = {dec2hex(readHashMap(string(node)),2)};
            subindex =  length(split(reg_0x80n6)) + 1;
            COB_ID = readMsgs(i).COB_ID;
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {'FF'};
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {dec2hex(readMsgs(i).DLC,2)};
            subindex = subindex;
            reg_0x80n6(subindex+1) = {'00'};
            reg_0x80n6(subindex+2) = {'00'};
            reg_0x80n6(subindex+3) = {'00'};
            reg_0x80n6(subindex+4) = {'00'};
            reg_0x80n6(subindex+5) = {'00'};
            reg_0x80n6(subindex+6) = {'00'};
            reg_0x80n6 = merge_num(reg_0x80n6);
            csv_handle(1+index*4,1) = {'<PS>'};
            csv_handle(1+index*4,2) = {'CoE'};
            csv_handle(1+index*4,3) = {strcat(strcat('0x',dec2hex(0x8006 + 16*(index-1))),' C 0')};
            csv_handle(1+index*4,4) = merge_num(reg_0x80n6);
            csv_handle(1+index*4,5) = {'EL67xx CoE Init Cmd '};
        else
            read_Node_Unique_Num = read_Node_Unique_Num + 1;
            readHashMap(string(node)) = 1;
            offset = offset + 1;
            indexHashMap(string(node)) = offset;
            index = offset;
            
            reg_0x80n0(3,1)= {dec2hex(index,2)};
            csv_handle(index*4,1) = {'<PS>'};
            csv_handle(index*4,2) = {'CoE'};
            csv_handle(index*4,3) = {strcat(strcat('0x',dec2hex(0x8000 + 16*(index-1))),' C 0')};
            csv_handle(index*4,4) = merge_num(reg_0x80n0);%配置好0x8000+16*n的数据
            csv_handle(index*4,5) = {'EL67xx CoE Init Cmd '};
            
            reg_0x80n8 = cell(2 ,1);
            reg_0x80n8(:) = {'00'};
            csv_handle(2+index*4,1) = {'<PS>'};
            csv_handle(2+index*4,2) = {'CoE'};
            csv_handle(2+index*4,3) = {strcat(strcat('0x',dec2hex(0x8008 + 16*(index-1))),' C 0')};
            csv_handle(2+index*4,4) = merge_num(reg_0x80n8);
            csv_handle(2+index*4,5) = {'EL67xx CoE Init Cmd '};
            
            csv_handle(3+index*4,1) = {'<PS>'};
            csv_handle(3+index*4,2) = {'CoE'};%0x80n3意义不明，目前置0
            csv_handle(3+index*4,3) = {strcat(strcat('0x',dec2hex(0x8003 + 16*(index-1))),' C 0')};
            csv_handle(3+index*4,4) = {'00 00'};
            csv_handle(3+index*4,5) = {'EL67xx CoE Init Cmd '};
            
            reg_0x80n6 = cell(2 + 12 * readHashMap(string(node)),1);
            reg_0x80n6(:) = {'00'};
            reg_0x80n6(1,1) = {dec2hex(readHashMap(string(node)),2)};
            subindex = 3;
            COB_ID = readMsgs(i).COB_ID;
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            COB_ID = floor(double(COB_ID) / 256);
            reg_0x80n6(subindex) = {dec2hex(mod(COB_ID,256),2)};
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {'FF'};
            subindex = subindex + 1;
            reg_0x80n6(subindex) = {dec2hex(readMsgs(i).DLC,2)};
            subindex = subindex + 7;
            csv_handle(1+index*4,1) = {'<PS>'};
            csv_handle(1+index*4,2) = {'CoE'};
            csv_handle(1+index*4,3) = {strcat(strcat('0x',dec2hex(0x8006 + 16*(index-1))),' C 0')};
            csv_handle(1+index*4,4) = merge_num(reg_0x80n6);
            csv_handle(1+index*4,5) = {'EL67xx CoE Init Cmd '};
        end
        readMsgs(i).isUse = node;
    end
end
%0x1C12 and 0x1C13
reg_0x1C12 = cell(send_TxNode_Unique_Num * 2 + 2,1);
reg_0x1C12(:) = {'00'};
if send_TxNode_Unique_Num>0
    reg_0x1C12(1,1) = {dec2hex(send_TxNode_Unique_Num,2)};
    for i = 1:send_TxNode_Unique_Num
        
        reg_0x1C12(i*2+1,1) = {dec2hex(i-1,2)};
        reg_0x1C12(i*2+2,1) = {'16'};
    end
end
csv_handle(4 + 4 * offset,1) = {'<PS>'};%0x1C12
csv_handle(4 + 4 * offset,2) = {'CoE'};
csv_handle(4 + 4 * offset,3) = {'0x1C12 C 0'};
csv_handle(4 + 4 * offset,4) = merge_num(reg_0x1C12);
csv_handle(4 + 4 * offset,5) = {'download pdo 0x1C12 index'};


reg_0x1C13 = cell(read_Node_Unique_Num * 2 + 8,1);%0x1C13
reg_0x1C13(:) = {'00'};
if read_Node_Unique_Num > 0
    reg_0x1C13(1,1) = {dec2hex(read_Node_Unique_Num + 3,2)};
    k = 3;
    for i = 1:read_Node_Unique_Num
        reg_0x1C13(k,1) = {dec2hex(i-1,2)};
        k = k + 1;
        reg_0x1C13(k,1) = {'1A'};
        k = k + 1;
    end
    reg_0x1C13(k,1) = {'82'};
    reg_0x1C13(k+1,1) = {'1A'};
    reg_0x1C13(k+2,1) = {'83'};
    reg_0x1C13(k+3,1) = {'1A'};
    reg_0x1C13(k+4,1) = {'84'};
    reg_0x1C13(k+5,1) = {'1A'};
end
csv_handle(5 + 4 * offset,1) = {'<PS>'};%0x1C13
csv_handle(5 + 4 * offset,2) = {'CoE'};
csv_handle(5 + 4 * offset,3) = {'0x1C13 C 0'};
csv_handle(5 + 4 * offset,4) = merge_num(reg_0x1C13);
csv_handle(5 + 4 * offset,5) = {'download pdo 0x1C13 index'};
csv_handle(6 + 4 * offset :length(csv_handle),:) = [];%删除多余内容

cell2csv('test.csv',csv_handle);
end

