% clear all;
close all;
clc

%% Демодуляция АМ сигнала
% Частота дискретизации
Fs_am = 32000;

% сохраняем id файла в переменную fid
fid = fopen('am_sound.dat');
% Извлекаем данные (считываем в однострочный массив)
data = fread(fid, [1, Inf], 'int32');
% Извлекаем вещественные и мнимые части
complexData = complex(data(1:2:end), data(2:2:end));
% Закрываем файл
fclose(fid);

% Из графика действительной части сигнала получаем, что нормированная
% частота равна 360/180 (градусов/отсчет) или pi/90 (рад/отсчет)
% С помощью ФНЧ выделяем низкочастотную составляющую
filtered_signal = lowpass(complexData, pi/90);

% Умножаем на 1e-5, чтобы уменьшить амплитуду, избавляемся от постоянной 
% составляющей (-3.2) и получаем модулирующий сигнал
modulating_signal = abs(filtered_signal)*1e-5-3.2;
% plot(modulating_signal);
% Добавим фильтр скользящего среднего
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
average_filtered_signal = filter(b,a,real(modulating_signal));

% sound(average_filtered_signal, Fs_am);  % Перед проигрыванием уменьшить
% звук компьютера!!

%% Демодуляция FM сигнала

% Частота дискретизации FM сигнала
Fs_fm_original = 500*1e6;
Sample_time = 1/Fs_fm_original;
% Так в функции sound частота дискретизации должна лежать в пределах от
% 1000 до 384 000, то используем 192000
Fs_fm = 44100;

% Получение комплексных отсчетов аналогично АМ
FM_file_id = fopen('file1EuropaPlus.bin');
data = fread(FM_file_id, [1, 1000000], 'int32');
ComplexSamples = complex(data(1:2:end), data(2:2:end));
% Закрываем файл
fclose(FM_file_id);

% figure
% plot(abs(fft(ComplexSamples)));

% % С помощью ФНЧ выделяем низкочастотную составляющую
filtered_signal = ComplexSamples;

% вычисляем зависимость чатоты от времени. w(t) = w_д*s_m(t), w_д -
% девиация частоты. s_m(t) - модулирующая функция
sample_indexes = 2:length(ComplexSamples);
% s_m = (angle(ComplexSamples(sample_indexes))-angle(ComplexSamples(sample_indexes-1)))/Sample_time;
s_m = atan2(imag(filtered_signal), real(filtered_signal));
s_m = s_m(sample_indexes) - s_m(sample_indexes-1);
s_m = s_m/Sample_time;

% Добавим фильтр скользящего среднего
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
aver_filtered_signal = filter(b,a,s_m);

% До прореживания необходимо пропустить сигнал через ФНЧ
filtered_signal = lowpass(aver_filtered_signal, 0.5);

% Прореживание
% [P,Q] = rat(Fs_fm_original/Fs_fm);
% DownsampledSignal = resample(ComplexSamples, P, Q);
DownsampledSignal = downsample(filtered_signal, round(Fs_fm_original/Fs_fm));

figure
plot(abs(fft(DownsampledSignal)));

figure
plot(abs(fft(s_m)));

sound(aver_filtered_signal, Fs_fm);