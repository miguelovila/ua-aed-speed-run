% Load Values
valores = load("solution_5_times.txt");
n1 = valores(1:end, 1);
t = valores(1:end, 2);

% Graph
figure(5);
plot(n1, t, '.-');
ylabel("Tempo (s)");
xlabel("Valores de N");
title("Solução 5");