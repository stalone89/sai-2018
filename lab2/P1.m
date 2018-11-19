%Least Squares Lab2 SAI

input=importdata('inputdata.csv',",",1);

pressure=input.data(:,1);
out_minus45=input.data(:,2);
out_25=input.data(:,3);
out_125=input.data(:,4);

n=size(input,1);

T_minus45=ones(n,1)*(-45);
T_25=ones(n,1)*25;
T_125=ones(n,1)*125;

plot(pressure,out_minus45);
hold on
plot(pressure,out_25);
plot(pressure,out_125);

p_minus45=polyfit(pressure,out_minus45,2);
p_25=polyfit(pressure,out_25,2);
p_125=polyfit(pressure,out_125,2);

plot(pressure,polyval(p_minus45,pressure,2));
plot(pressure,polyval(p_25,pressure,2));
plot(pressure,polyval(p_125,pressure,2));

%H=ones(n,1);

%H_minus45=[H pressure T_minus45];