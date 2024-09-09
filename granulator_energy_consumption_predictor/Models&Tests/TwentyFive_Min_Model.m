clear 
clc
close all 
load('VARS.mat');

Y = T_10;
Y = Y(7:1348,:);
X = Elapsed_10;
X = X(7:1348,:);
X = log(X);
t = table(X, Y);

mdl = fitlm(t);
intC = table2array(mdl.Coefficients("(Intercept)",1));
sclr = table2array(mdl.Coefficients("X",1));

G = intC + sclr*X;

hold on
plot(Y);
plot(G);
hold off
 
legend('Actual Data', 'Log Function', 'Location', 'southeast');

text(840,0.2,'Gradient of Log(X) = 0.401', 'EdgeColor','black');
ylabel('TORQUEGRANULATOR');
xlabel('RUNTIME / seconds');

clear
