
train1 = importdata('./1.Bivariate/overlapping_data/group7/class1_train.txt');
train2 = importdata('./1.Bivariate/overlapping_data/group7/class2_train.txt');
train3 = importdata('./1.Bivariate/overlapping_data/group7/class3_train.txt');

Y_train1 = ones(size(train1,1),1);
Y_train2 = ones(size(train2,1),1)*2;
Y_train3 = ones(size(train3,1),1)*3;

test1 = importdata('./1.Bivariate/overlapping_data/group7/class1_test.txt');
test2 = importdata('./1.Bivariate/overlapping_data/group7/class2_test.txt');
test3 = importdata('./1.Bivariate/overlapping_data/group7/class3_test.txt');

Y_test1 = ones(size(test1,1),1);
Y_test2 = ones(size(test2,1),1)*2;
Y_test3 = ones(size(test3,1),1)*3;

val1 = importdata('./1.Bivariate/overlapping_data/group7/class1_val.txt');
val2 = importdata('./1.Bivariate/overlapping_data/group7/class2_val.txt');
val3 = importdata('./1.Bivariate/overlapping_data/group7/class3_val.txt');

Y_val1 = ones(size(val1,1),1);
Y_val2 = ones(size(val2,1),1)*2;
Y_val3 = ones(size(val3,1),1)*3;

X_train = train1;
X_train = [X_train;train2];
X_train = [X_train;train3];

X_test = test1;
X_test = [X_test;test2];
X_test = [X_test;test3];

X_val = val1;
X_val = [X_val;val2];
X_val = [X_val;val3];

Y_train = Y_train1;
Y_train = [Y_train;Y_train2];
Y_train = [Y_train;Y_train3];

Y_test = Y_test1;
Y_test = [Y_test;Y_test2];
Y_test = [Y_test;Y_test3];

Y_val = Y_val1;
Y_val = [Y_val;Y_val2];
Y_val = [Y_val;Y_val3];
train = [X_train,Y_train];
