% Clear Command Window
clc

% Load Values
valores = load("solution_1_times.txt");
n1 = valores(1:end, 1);
t = valores(1:end, 2);

% Calculate Linear Regression, getting the log10 for 
P = polyfit(n1, log10(t), 1);
m = P(1);
b = P(2);
disp(['Linear Regression equation: y = ' num2str(m) 'x + ' num2str(b) ])

% Calculate y with x = 800, and then do 10^y, to get the time
y = m*800 + b;
final = 10^y;
disp(['Tempo final = ' num2str(final, 5) ' segundos'])