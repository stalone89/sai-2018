function [heading_dep, heading_appr, radian_dist, length] = orthodrome(LL_p1,LL_p2)
%ORTHODROME 
global EARTH_RADIUS

lat1=deg2rad(LL_p1(:,1));
lon1=deg2rad(LL_p1(:,2));
r=LL_p1(:,3);

R = EARTH_RADIUS + r;

lat2=deg2rad(LL_p2(:,1));
lon2=deg2rad(LL_p2(:,2));

radian_dist=acos(cos(lat2).*cos(lon1-lon2).*cos(lat1)+sin(lat2).*sin(lat1));

heading_dep=atan2(-cos(lat2).*sin(lon1-lon2),-cos(lat2).*cos(lon1-lon2).*sin(lat1)+sin(lat2).*cos(lat1));

heading_appr=atan2(-sin(lon1-lon2).*cos(lat1),sin(lat2).*cos(lon1-lon2).*cos(lat1)-cos(lat2).*sin(lat1));

length=R.*radian_dist;

radian_dist=rad2deg(radian_dist);
heading_dep=rad2deg(heading_dep);
heading_appr=rad2deg(heading_appr);


end

