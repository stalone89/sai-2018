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

clf;
surf([-45, 25, 125], pressure, [out_minus45 out_25 out_125]);
surf([-45, 25, 125], pressure, [polyval(p_minus45, pressure, 2) polyval(p_25, pressure, 2) polyval(p_125, pressure, 2)]);

%clf;
%hold on;
%i = 1;
%for i = 1:1:15
%	plot([1 2 3], [out_minus45(i) out_25(i) out_125(i)]);
%	plot([1 2 3], [polyval(p_minus45, pressure, 2) polyval(p_25, pressure, 2) polyval(p_125, pressure, 2)]);
%	i = i + 1;
%end;

sf = fit([[out_minus45;out_25;out_125] [-45*ones(size(out_25));25*ones(size(out_25));125*ones(size(out_25))]], [pressure;pressure;pressure], 'poly21');
plot(sf,[[out_minus45;out_25;out_125] [-45*ones(size(out_25));25*ones(size(out_25));125*ones(size(out_25))]], [pressure;pressure;pressure]);

hold on;
error_3dfit = [pressure;pressure;pressure] - feval(sf,[out_minus45;out_25;out_125],[-45*ones(size(out_25));25*ones(size(out_25));125*ones(size(out_25))]);
%scatter3([out_minus45;out_25;out_125], [-45*ones(size(out_25));25*ones(size(out_25));125*ones(size(out_25))], error_3dfit); % -- Scatter version of error plots

% -- Non-sorted 3D plot
%error_3dfit_plot = sortrows([[out_minus45;out_25;out_125] [-45*ones(size(out_25));25*ones(size(out_25));125*ones(size(out_25))] error_3dfit], 1); 
%plot3([out_minus45], [-45*ones(size(out_25))], error_3dfit(1:219));
%plot3([out_25], [25*ones(size(out_25))], error_3dfit(220:438));
%plot3([out_125], [125*ones(size(out_25))], error_3dfit(439:657));

% -- Sorted 3D plot
plotthis = sortrows([[out_minus45] [-45*ones(size(out_25))] error_3dfit(1:219)], 1);
plot3(plotthis(:,1), plotthis(:,2), plotthis(:,3));
plotthis = sortrows([[out_25] [25*ones(size(out_25))] error_3dfit(220:438)], 1);
plot3(plotthis(:,1), plotthis(:,2), plotthis(:,3));
plotthis = sortrows([[out_125] [125*ones(size(out_25))] error_3dfit(439:657)], 1);
plot3(plotthis(:,1), plotthis(:,2), plotthis(:,3));

% -- Error surface using only voltage of minus45
%hold on;
%error_3dfit = [error_3dfit(1:219), error_3dfit(220:438), error_3dfit(439:657)];
%surf([out_minus45], [-45 25 125], error_3dfit');
