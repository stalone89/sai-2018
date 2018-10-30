%Lab1 Teste

%Usando apenas para teste percurso Lisboa -> Paris
global EARTH_RADIUS
EARTH_RADIUS = 6378000;
T_iter=1;

position1=[38.7812995911, -9.13591957092, 374];
position2=[49.0127983093, 2.54999995232, 392];

i=0;


%Vm=TAS; %Por agora
R=position1(3)+EARTH_RADIUS;
TAS=200*60;

threshold=TAS*T_iter;
[heading_dep, heading_appr, radian_dist, length]=orthodrome(position1,position2);
position_log=position1;

while length>= threshold
    TAS=200*60*(1+0.01*sin(2*pi*(i/20)));
    theta=0;
    V_north=TAS*cos(theta)*cosd(heading_dep);
    V_east=TAS*cos(theta)*sind(heading_dep);
    
    dNorth=V_north*T_iter;
    dEast=V_east*T_iter;
    
    dlat=dNorth/R;
    dlon=dEast/(R*cosd(position1(1)));
    
    lat_new=position1(1) + dlat;
    lon_new=position1(2) + dlon;
    
    position1=[lat_new, lon_new, position1(3)];
    [heading_dep, heading_appr, radian_dist, length]=orthodrome(position1,position2);
    TAS_log(i+1)=TAS;
    position_log=[position_log; position1];
    i=i+1;
end
