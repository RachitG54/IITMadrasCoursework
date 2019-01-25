key = {'Leopards' ,   'bonsai' ,   'brain' ,   'buddha' ,   'butterfly' ,   'car side' ,   'chandelier' ,   'ewer' ,   'grand piano' ,   'hawksbill' , 'helicopter' ,   'ibis' ,   'kangaroo' ,   'ketch' ,   'menorah' ,   'revolver' ,   'scorpion' ,   'starfish' ,   'sunflower' ,   'trilobite' ,  'watch'};
gp7data = [40,48,56,76,93];
X = [];
y = [];
for i = 1:2233
	for j = 1:5
        if data.labels(i) == gp7data(j)
            X = [X;data.data(i,:)];
            y = [y;data.labels(i)];
        end
	end  
end
in = [X,y];
in = transpose(in);
[train,val,test] = dividerand(in,0.6,0.2,0.2);
train = transpose(train);
val = transpose(val);
test = transpose(test);