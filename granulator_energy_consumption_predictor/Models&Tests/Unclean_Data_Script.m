% Author: Shaurya Pathak
% Last Edited: 26-04-2023

%%
clear
clc
close all
%% Reading the Data 

TORG = FIMPORTF('050319A_050319A_06Mar2019_090754.xlsx');
T = FIMPORTF('300323A_300323A_30Mar2023_153459.xlsx');

DAT_T = T(:, ["TIME" "TORQUEGRANULATOR" "MASSFLOWPOWDERDOSING1" "SPEEDGRANULATOR"]);
DAT_TORG = TORG(:, ["TIME" "TORQUEGRANULATOR" "MASSFLOWPOWDERDOSING1" "SPEEDGRANULATOR"]);

DAT_T = table2timetable(DAT_T);
DAT_TORG = table2timetable(DAT_TORG);

%% Beginning Treatment


ROWPFR7 = timerange('30-Mar-2023 15:15:01','30-Mar-2023 15:22:07');
ROWPFR13 = timerange('30-Mar-2023 15:24:21','30-Mar-2023 15:31:09');
ROWPFR10 = timerange('05-Mar-2019 09:43:17','05-Mar-2019 10:07:12');

PFR7 = DAT_T(ROWPFR7,:);
PFR10 = DAT_TORG(ROWPFR10,:);
PFR13 = DAT_T(ROWPFR13,:);

PFR7 = TREATMENT(PFR7);
PFR10 = TREATMENT(PFR10);
PFR13 = TREATMENT(PFR13);

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

RT_TT.TIME = RT_TT.TIME - RT_TT.TIME(1);

RT_TT = RT_TT(2184:10815,:);

RT_TT.TIME = RT_TT.TIME - RT_TT.TIME(1);
RT_TT = RT_TT(1:6907,:);

PFR10.TIME = PFR10.TIME - PFR10.TIME(1);
PFR13.TIME = PFR13.TIME - PFR13.TIME(1);

figure(1)
plot(RT_TT,"TIME","TORQUEGRANULATOR");
hold on
plot(PFR10,"TIME","TORQUEGRANULATOR");
plot(PFR13,"TIME","TORQUEGRANULATOR");
legend('3 Hr Data','25 Min Data', '5 Min Data', 'Location','northeast');
title('Figure 1');








