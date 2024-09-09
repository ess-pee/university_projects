% Author: Shaurya Pathak
% Last Edited: 26-04-2023

%%
clear
clc
close all
%% Reading the Data 

OPTIONS = detectImportOptions('280320182_280320182_09May2018_143443- 3 hr cont. GEA.xlsx');
OPTIONS.SelectedVariableNames = [1, 3:49];
OPTIONS.DataRange = 'A5';
OPTIONS.VariableNamesRange = 'A2';

T = readtable('280320182_280320182_09May2018_143443- 3 hr cont. GEA.xlsx', OPTIONS);

T = renamevars(T,"Description","TIME");

DAT_T = T(:, ["TIME" "TORQUEGRANULATOR" "MASSFLOWPOWDERDOSING1" "SPEEDGRANULATOR"]);

TT = table2timetable(DAT_T);

RT_TT = retime(TT,'secondly', 'mean');

RT_TT = fillmissing(RT_TT, 'next');

RT_TT = STDCLEANER(RT_TT,RT_TT.MASSFLOWPOWDERDOSING1);


figure(1)
plot(RT_TT,"TIME","TORQUEGRANULATOR");









