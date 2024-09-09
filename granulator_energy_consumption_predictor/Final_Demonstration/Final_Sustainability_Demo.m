% This work of genius is brought to you by: Shaurya Pathak
% Last Edited: 01-05-2023

clear
clc
close all 

syms IntInit Int25

X25 = seconds(34:1348); X25 = seconds(X25); X25 = transpose(X25);

X3H = seconds(34:6907); X3H = seconds(X3H); X3H = transpose(X3H);


%% Select your variables. 
% IntInit is the initial steady state torque. 
% SCRSPD is the Screw Speed 
% PFR is the Powder Feed Rate

IntInit = 5;
SCRSPD = RPM2RPS(500);
PFR = 13;

%% Need not worry about anything else, everything is taken care of.

Y25 = IntInit-1.79 + 0.401*(log(X25));

Int25 = Y25(end,1);

Y3H = Int25-2.52 + 0.431*(log(X3H));

Y3H = Y3H(310:end,1);

S1 = length(X25); S2 = length(Y3H); SF = S1 + S2;

XFULL = seconds(1:SF); XFULL = seconds(XFULL); XFULL = transpose(XFULL);
TORQUE = zeros(size(XFULL)); TORQUE(1:S1,1) = Y25; TORQUE(S1+1:end,1) = Y3H; 

figure(1)
plot(TORQUE,'LineWidth',1);
xlabel('Elapsed Time / seconds');
ylabel('TORQUEGRANULATOR');
title(strcat("Predicted Torque Response for Steady State Torque: ",num2str(IntInit)," Nm"));
legend('Torque Response','Location','southeast');



ENERGY = (SCRSPD .* TORQUE)./PFR;
TE = trapz(ENERGY);
TE = TE/1000;
KWH = TE/3600;

figure(2)
area(ENERGY,'LineWidth',1,'EdgeColor','black','FaceColor','#FFAE42');
xlabel('Elapsed Time / seconds');
ylabel('ENERGY / joules');
title(strcat("Predicted Energy Consumption for Steady State Torque: ",num2str(IntInit)," Nm"));
legend(strcat("Total Energy = ",num2str(KWH)," kWh"),'Location','northwest');

