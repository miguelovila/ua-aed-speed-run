% Load Values
valores3 = load("solution_3_times.txt");
valores4 = load("solution_4_times.txt");
valores5 = load("solution_5_times.txt");
n = valores3(1:end, 1);
t1 = valores3(1:end, 2);
t2 = valores4(1:end, 2);
t3 = valores5(1:end, 2);

% Graph
figure(5);
plot(n, t1, '.-');
hold on;
plot(n, t2, '*-');
hold on;
plot(n, t3, 'o-');
hold on;
legend('Solução 3', 'Solução 4', 'Solução 5')
ylabel("Tempo (s)");
xlabel("Valores de N");
title("Comparação das soluções 3, 4 e 5");