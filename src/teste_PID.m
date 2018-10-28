clear;
clc;
mass=100000;
scale_factor=1/mass;

kp=10000*scale_factor;
kd=10000*scale_factor;
ki=1*scale_factor;

plant_gain=1;

ref=210;
int=0;
curr_speed(1)=370;
last_err=0;

for i=1:1:50
    err=0;
    last_err=err;
    err=ref-curr_speed(i);
    
    prop=err;
    int=int+err;
    der=err-last_err;
    
    csignal(i)=kp*prop+ki*int+kd*der;
    
    curr_speed(i+1)=curr_speed(i)+csignal(i)*plant_gain;
    
end
    
plot(curr_speed);
%plot(csignal);