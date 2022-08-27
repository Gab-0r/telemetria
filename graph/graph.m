clear
clc
filename = 'acelData.txt';
delimiterIn = ',';
acel_data = importdata(filename,delimiterIn);
acel_data_size = size(acel_data);
acel_data_rows = acel_data_size(1);

x_acel = acel_data(:, 1);
y_acel = acel_data(:, 2);
z_acel = acel_data(:, 3);

t = [0:1:acel_data_rows-1];

subplot(3,1,1);
plot(t,x_acel, 'b', LineWidth=1);
title('Eje X');
grid on;

subplot(3,1,2); 
plot(t, y_acel, 'r', LineWidth=1);
title('Eje Y');
grid on;

subplot(3,1,3)
plot(t, z_acel, 'g', LineWidth=1);
title('Eje Z');
grid on;

