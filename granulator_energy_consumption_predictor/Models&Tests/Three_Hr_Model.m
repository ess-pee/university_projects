clear 
clc
close all 
load('VARS.mat');

Y = T_3H;
Y = Y(34:6874,:);
X = Elapsed_3H;
X = X(34:6874,:);
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

text(4200,1.2,'Gradient of Log(X) = 0.431', 'EdgeColor','black');
ylabel('TORQUEGRANULATOR');
xlabel('RUNTIME / seconds');

clear


