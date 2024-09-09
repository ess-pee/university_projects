% Last Edited: 25/04/2023
% This is the main matlab script for the Y3 Machine Learning Assignment,
% which involves analysis of traffic data and air pollution data.

close all
clear
clc

%% Data Cleaning 
% In this process we will first refine out the un-needed data from the
% traffic sensors namely everything except the day, hour and ofcourse the
% traffic flow.

% Collecting useful statistics from 1YMD1, 1YMD2, 1YPD1, 1YPD3

YMD1 = readmatrix('T_FLOW_SCC_1YMD1.csv','Range','G2:Q8609');
SEN_1 = YMD1(:,[1 2 11]);

YMD2 = readmatrix('T_FLOW_SCC_1YMD2.csv','Range','G2:Q8609');
SEN_2 = YMD2(:,[1 2 11]);

YPD1 = readmatrix('T_FLOW_SCC_1YPD1.csv','Range','G2:Q8609');
SEN_3 = YPD1(:,[1 2 11]);

YPD3 = readmatrix('T_FLOW_SCC_1YPD3.csv','Range','G2:Q8609');
SEN_4 = YPD3(:,[1 2 11]);

clear YPD1 YPD3 YMD2 YMD1 % Housekeeping :)

% Since all of the sensors are on the same timescale we do not need to do
% anything to the first 2 columns of the sensor reading but we do need to
% average the traffic across the 4 sensors before processing further.

SEN_TRAFF = [SEN_1(:,3), SEN_2(:,3), SEN_3(:,3), SEN_4(:,3),];
TRAFF_AV = mean(SEN_TRAFF, 2);
SEN_AV = [SEN_1(:,1), SEN_1(:,2), TRAFF_AV];

clear TRAFF_AV SEN_TRAFF SEN_4  SEN_3 SEN_2 % More Housekeeping :))

% Now since we only require predictions of the model with the resolution of
% 1 hour, meaning average emissions/traffic across any given hour we can
% start by starting to shorten the 8608 data points by taking median
% traffic flows for an hour for a day. The choice for this approach is
% explained in the report.

% Here we are beginning to further clean the data into only 720 data
% points, this value is obtained by taking the number of unique days*number
% of unique hours i.e 30*24 and then taking the median traffic flow for
% each hour for each corresponding day

UNCUT_SEN_AV = CLEANER_MEDIAN(SEN_AV);
UNCUT_SEN_1 = CLEANER_MEDIAN(SEN_1);

clearvars -except UNCUT_SEN_AV UNCUT_SEN_1

% Now we will begin cleaning the Air Quality data 

AQ_PRE = readmatrix('AQ_S0008.csv','Range','G2:V6277');

NO = AQ_PRE(:,[1 2 14]);
CO = AQ_PRE(:,[1 2 16]);

% Mostly using the same steps as before

UNCUT_NO = CLEANER_MEAN(NO);
UNCUT_CO = CLEANER_MEAN(CO);

UNFIN_NOCO_DAT = [UNCUT_NO UNCUT_CO(:,3)];

% More housekeeping to re-use code :D

clearvars -except UNCUT_SEN_AV UNFIN_NOCO_DAT UNCUT_SEN_1;

% Our data is now almost clean but we notice that UNCUT_SEN_AV is twice as
% large as UNFIN_NOCO_DAT, with them being 720 and 360 respectively, so now
% our next step becomes to filter out the days where the data from the AQ
% sensor is not available from the traffic data. It is important to note
% the we are not wasting this data it will be useful later on when we use
% it as testing data for our model. For now we will make sure that the data
% corresponds to one another.

UN_AQ_SEN = UNFIN_NOCO_DAT;

col_idx = 3;

% Replace 0 values with NaN in column 3, NO
UN_AQ_SEN(UN_AQ_SEN(:,col_idx) == 0, col_idx) = NaN;

col_idx = 4;

% Replace 0 values with NaN in column 4, CO
UN_AQ_SEN(UN_AQ_SEN(:,col_idx) == 0, col_idx) = NaN;

% Now we remove all rows with NaN CO and NO data

% Find rows with NaN columns
nan_rows = any(isnan(UN_AQ_SEN), 2);

% Remove rows with NaN columns
UN_AQ_SEN = UN_AQ_SEN(~nan_rows, :);
UN_AQ_SEN1 = UN_AQ_SEN;

clearvars col_idx nan_rows;

UN_AQ_SEN(:,5) = SORTER(UN_AQ_SEN,UNCUT_SEN_AV);
UN_AQ_SEN1(:,5) = SORTER(UN_AQ_SEN,UNCUT_SEN_1);

% With this we are finally almost done with the data cleaning process all
% that remains is separating weekend data from weekday data since traffic
% in both these segments of days is very different. Our data set starts
% from day 244 which was a Thursday. Which tells us that the weekends, Sat
% and Sun, would be on days [246 247 253 254 260 261 267 268]

WKND = [246 247 253 254 260 261 267 268];

% We now use the ismember function to extract weekend data

% Find rows with desired day numbers
rows = ismember(UN_AQ_SEN(:,1), WKND);

% Extracting weekend data
WKND_DAT = UN_AQ_SEN(rows, :);
WKND_DAT1 = UN_AQ_SEN1(rows, :);

% Extracting weekday data

WKD_DAT = UN_AQ_SEN(~rows, :); % Extracting rows which are not weekends
WKD_DAT1 = UN_AQ_SEN1(~rows, :);

clearvars -except WKND_DAT WKD_DAT UNCUT_SEN_AV UNFIN_NOCO_DAT WKD_DAT1 WKND_DAT1; % Clearing more variables for cleanliness

% Since the data is now is sorted into Weekdays and Weekends we no longer
% need the day column within this data. 

WKD_DAT = WKD_DAT(:,[2 3 4 5]);
WKD_DAT(:,1) = WKD_DAT(:,1)+1;

WKND_DAT = WKND_DAT(:,[2 3 4 5]);
WKND_DAT(:,1) = WKND_DAT(:,1)+1;

WKD_DAT1 = WKD_DAT1(:,[2 3 4 5]);
WKD_DAT1(:,1) = WKD_DAT1(:,1)+1;

WKND_DAT1 = WKND_DAT1(:,[2 3 4 5]);
WKND_DAT1(:,1) = WKND_DAT1(:,1)+1;


% We can still refine the  data, since the data is now split into Weekend
% and Weekday we can shorten it to represent a typical hour within each
% dataset. To achieve this we reuse our code which we used to sort into days and hours.

WKD = ACCUMULATOR(WKD_DAT);
WKND = ACCUMULATOR(WKND_DAT);

WKD1 = ACCUMULATOR(WKD_DAT1);
WKND1 = ACCUMULATOR(WKND_DAT1);

clearvars -except WKD WKND WKD_DAT WKND_DAT WKD1 WKND1
%% Machine Learning Algorithms

MOF = WKD;
FNO = 1;
FNOR = 2;
LMMT(MOF(:,1), MOF(:,2), MOF(:,3),MOF(:,4), WKD1(:,4), 1, FNO, FNOR);
LMM(MOF(:,1), MOF(:,3), MOF(:,4),MOF(:,2), 2, FNO, FNOR);
LMM(MOF(:,1), MOF(:,2), MOF(:,4),MOF(:,3), 3, FNO, FNOR);

figure(FNO)
subplot(3,1,1);
title('Weekday Traffic');
legend({'SEN 1 Actual', 'Predicted Average'}, 'Location', 'northwest');
subplot(3,1,2);
title('Weekday NO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('Weekday CO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');

figure(FNOR)
subplot(3,1,1);
title('Weekday Traffic Regularised');
legend({'SEN 1 Actual', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,2);
title('Weekday NO Regularised');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('Weekday CO Regularised');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');

MOF = WKND;
FNO = 3;
FNOR = 4;

LMMT(MOF(:,1), MOF(:,2), MOF(:,3),MOF(:,4), WKND1(:,4), 1, FNO, FNOR);
LMM(MOF(:,1), MOF(:,3), MOF(:,4),MOF(:,2), 2, FNO, FNOR);
LMM(MOF(:,1), MOF(:,2), MOF(:,4),MOF(:,3), 3, FNO, FNOR);

figure(FNO)
subplot(3,1,1);
title('Weekend Traffic');
legend({'SEN 1 Actual', 'Predicted Average'}, 'Location', 'northwest');
subplot(3,1,2);
title('Weekend NO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('Weekend CO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');

figure(FNOR)
subplot(3,1,1);
title('Weekend Traffic Regularised');
legend({'SEN 1 Actual', 'Predicted Average'}, 'Location', 'northwest');
subplot(3,1,2);
title('Weekend NO Regularised');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('Weekend CO Regularised');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');

MOF = WKD;
FNO = 5;


CBMMT(MOF(:,1), MOF(:,2), MOF(:,3),MOF(:,4), WKD1(:,4), 1, FNO);
CBMM(MOF(:,1), MOF(:,3), MOF(:,4),MOF(:,2), 2, FNO);
CBMM(MOF(:,1), MOF(:,2), MOF(:,4),MOF(:,3), 3, FNO);

figure(FNO)
subplot(3,1,1);
title('8th Order Weekday Traffic');
legend({'SEN 1 Actual', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,2);
title('8th Order Weekday NO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('8th Order Weekday CO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');


MOF = WKND;
FNO = 6;

CBMMT(MOF(:,1), MOF(:,2), MOF(:,3),MOF(:,4), WKND1(:,4), 1, FNO);
CBMM(MOF(:,1), MOF(:,3), MOF(:,4),MOF(:,2), 2, FNO);
CBMM(MOF(:,1), MOF(:,2), MOF(:,4),MOF(:,3), 3, FNO);

figure(FNO)
subplot(3,1,1);
title('8th Order Weekend Traffic');
legend({'SEN 1 Actual', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,2);
title('8th Order Weekend NO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');
subplot(3,1,3);
title('8th Order Weekend CO');
legend({'Actual Data', 'Predicted'}, 'Location', 'northwest');



