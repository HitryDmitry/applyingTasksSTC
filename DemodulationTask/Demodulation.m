% clear all;
close all;
clc

%% Демодуляция АМ сигнала
% Количество отсчетов
nsamps = (1024*1024);
% Частота дискретизации
Fs_am = 32000;
 
% номер позиции (символа), с которого начинаем чтение
nstart = 1;
% сохраняем id файла в переменную fid
fid = fopen('am_sound.dat');
% устанавливаем позицию, с которой будет реализовано чтение (равна 1)
fseek(fid, nstart, 'bof');
% Считываем действительные и мнимые части и записываем их в массив размером
% 2xnsamps. В верхней строке массива находятся действительные части
y = fread(fid, [2, nsamps], 'short');
% Закрываем файл
fclose(fid);
% Из полученных значений для вещественной и мнимой частей получаем
% комплексные числа
x = complex(y(1,:),y(2,:));

% Из графика действительной части сигнала получаем, что нормированная
% частота равна 360/180 (градусов/отсчет) или pi/90 (рад/отсчет)
% С помощью ФНЧ выделяем низкочастотную составляющую
filtered_signal = lowpass(x, pi/90);

% Получаем модулирующий сигнал
modulating_signal = 2*real(filtered_signal);

% Добавим фильтр скользящего среднего
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
average_filtered_signal = filter(b,a,real(modulating_signal));


% sound(average_filtered_signal(1:200000), Fs_am);  % Перед проигрыванием уменьшить
% звук компьютера!!
% figure
% plot(average_filtered_signal(1:10000));
% figure
% plot(real(fft(x(1:10000))));

%% Демодуляция FM сигнала

% Частота дискретизации FM сигнала
% Fs_fm = 500*1e6;
% Так в функции sound частота дискретизации должна лежать в пределах от
% 1000 до 384 000, то используем 192000
Fs_fm = 192000;

% Получение комплексных отсчетов аналогично АМ
nsamps = (1024*1024);
nstart = 1;
file_FM = fopen('file1EuropaPlus.bin');
fseek(file_FM, nstart, 'bof');
realAndImag = fread(file_FM, [2, nsamps], 'short');
RealParts = realAndImag(1,:);
ImagParts = realAndImag(2,:);
fclose(file_FM);
ComplexSamples = complex(RealParts, ImagParts);

figure
plot(abs(fft(ComplexSamples)));

% Прореживание
DownsampledSignal = downsample(ComplexSamples, 2.605e+03);

% До прореживания необходимо пропустить сигнал через ФНЧ
filtered_signal = lowpass(ComplexSamples, 96000, Fs_fm);

% С помощью ФНЧ выделяем низкочастотную составляющую


% вычисляем зависимость чатоты от времени. w(t) = w_д*s_m(t), w_д -
% девиация частоты. s_m(t) - модулирующая функция
s_m = atan2(imag(filtered_signal), real(filtered_signal));
s_m = gradient(s_m);

% Добавим фильтр скользящего среднего
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
aver_filtered_signal = filter(b,a,s_m);

figure
plot(abs(fft(s_m)));

% figure
% plot(s_m(1:100000));
% sound(aver_filtered_signal(1:500000), Fs_fm);