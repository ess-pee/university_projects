function LMM(A, B, C, D, GRAPH_NO, FIGURE_NO, FIGURE_NO_R)
X = [A, B, C];
X = [X X.^2 X.^3 X.^4];
N = size(X);
lambda = 0.1;
PSI = [ones(N(1,1),1), X];
[n, p] = size(PSI); 
Y = D;
THETA_HAT = inv(PSI'*PSI)*PSI'*Y;
THETA_HATR = inv(PSI'*PSI + lambda*eye(p)) * PSI' * Y;
Y_PREDICT = PSI*THETA_HAT;
Y_PREDICTR = PSI*THETA_HATR;

SST = sum((Y-mean(Y)).^2);
SSE = sum((Y-Y_PREDICT).^2);
R2 = 1 - SSE/SST;
SSR = SST - SSE;
F = (SSR / 3) / (SSE / (n - 3 - 1));

fprintf('\n\nFigure %d Graph %d: F statistic = %f, R squared value = %f.', FIGURE_NO, GRAPH_NO, F,R2);

SST = sum((Y-mean(Y)).^2);
SSE = sum((Y-Y_PREDICTR).^2);
R2 = 1 - SSE/SST;
SSR = SST - SSE;
F = (SSR / 3) / (SSE / (n - 3 - 1));

fprintf('\n\nFigure %d Graph %d: F statistic = %f, R squared value = %f.', FIGURE_NO_R, GRAPH_NO, F,R2);

figure(FIGURE_NO)
subplot(3,1,GRAPH_NO)
plot(Y,'o');
hold on 
plot(Y_PREDICT);
hold off

figure(FIGURE_NO_R)
subplot(3,1,GRAPH_NO)
plot(Y,'o');
hold on 
plot(Y_PREDICTR);
hold off
end