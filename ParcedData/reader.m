clear 
data=fopen('peakPositionLte672.pos', 'rb');
parcedData = fread(data, [1 inf], 'float');


plot(parcedData(1,:))