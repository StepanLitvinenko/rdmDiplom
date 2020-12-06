clear 
data=fopen('coordinatesPostForBsWithSn314.xy', 'rb');
parcedData = fread(data, [2 inf], 'int32');


plot(parcedData(1,:),parcedData(2,:))