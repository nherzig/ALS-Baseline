clear all; close all; clc;
%% Generate new data
T= 10;     % Time (in Seconds)
N=100.*T;                              % Number of Impulses/change in track
h=sqrt(T/N);
np=1    % Number of Particles

% Initialization of the position of particles..i.e origin

x = zeros(1,np);

% Iteration to store positions of particles

for j=1:np
    for i=1:N
        x(i+1,j)=x(i,j)+h*randn();
    end
end
% Plotting of the particles. 
figure; plot(x);
%% Write input
FID = fopen('input','w');
fwrite(FID,length(x),'int32',0,'ieee-le');
for ii = 1:length(x)
    fwrite(FID,x(ii),'float64',0,'ieee-le');
end
fclose(FID);
%% Read output
FID = fopen('output','r');
len = fread(FID,1,'int32',0,'ieee-le');
data = zeros(len,1);
for ii = 1:len
    data(ii) = fread(FID,1,'float64',0,'ieee-le');
end
fclose(FID);
figure; plot(data);