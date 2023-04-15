clc;
clear;


formatSpec = '%f';
A = readmatrix('large_u', 'Delimiter', " ");

figure(1)

B = A(:,2)';

histogram(B, unique(B));

title("Uniform Dist")

xlabel("Address");
ylabel("Count");
