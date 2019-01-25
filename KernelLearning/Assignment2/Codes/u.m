%{
% X = [ 0.  0.  0.  0.  0.  0.  0.  0.  0.  0.  0.  0.  0.  0.  2.  0.  1.  1. 1.  1.  1.  1.  1.  1.  1.  1.  2.  1.  3.  1.  1.  2.  1.  1.  1.  1. 1.  1.  2.  2.  4.  2.  2.  2.  2.  2.  2.  2.  2.  2.  2.  2.  2.  2. 2.  2.  2.  2.  2.  2.  4.  2.  2.  2.  2.  3.  3.  3.  3.  3.  3.  3. 3.  3.  3.  3.  3.  3.  3.  3.  3.  4.  4.  4.  4.  4.  4.  4.  4.  0. 4.  4.  4.  4.  4.  4.  4.  4.  4.  4.  4.  4.]
Y = X+1
n_classes = 5
y_one_hot = zeros( size( Y, 1 ), n_classes);

% assuming class labels start from one
for i = 1:n_classes
    rows = Y == i;
    y_one_hot( rows, i ) = 1;
end
Yd = transpose(y_one_hot);
%}
