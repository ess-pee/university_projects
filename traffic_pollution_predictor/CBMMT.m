function CBMMT(A, B, C, D, E, GRAPH_NO, FIGURE_NO)
X = [A, B, C];
X = [X X.^2 X.^3 X.^4 X.^5 X.^6 X.^7 X.^8];
N = size(X);
PSI = [ones(N(1,1),1), X];
[n, p] = size(PSI); 
Y = D;
THETA_HAT = inv(PSI'*PSI)*PSI'*Y;
Y_PREDICT = PSI*THETA_HAT;

SST = sum((E-mean(E)).^2);
SSE = sum((E-Y_PREDICT).^2);
R2 = 1 - SSE/SST;
SSR = SST - SSE;
F = (SSR / 3) / (SSE / (n - 3 - 1));

fprintf('\n\nFigure %d Graph %d: F statistic = %f, R squared value = %f.', FIGURE_NO, GRAPH_NO, F,R2);

figure(FIGURE_NO)
subplot(3,1,GRAPH_NO)
plot(Y,'o');
hold on 
plot(Y_PREDICT);
hold off