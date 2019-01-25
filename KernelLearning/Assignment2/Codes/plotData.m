function plotData(X, y)
figure; hold on;

c1 = find(y==1);
c2 = find(y==2);
c3 = find(y==3);
% Plot Examples
plot(X(c1, 1), X(c1, 2), 'r');
plot(X(c2, 1), X(c2, 2), 'g');
plot(X(c3, 1), X(c3, 2), 'b');
xlabel('feature 1')
ylabel('feature 2')

% Specified in plot order
legend('Class 1', 'Class 2', 'Class 3')
hold off;

end