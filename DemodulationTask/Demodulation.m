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

% В файле записан комплексный сигнал на нулевой частоте. Т.е. по сути в
% файле записана комплексная огибающая сигнала. Поэтому достаточно взять
% действительную часть abs(x).*cos(angle(x)) или real(x)

% sound(abs(x).*cos(angle(x)), Fs_am);  # Перед проигрыванием уменьшить
% звук компьютера!!
% figure
% plot(real(x(1:1000)));

%% Демодуляция FM сигнала

% Частота дискретизации FM сигнала
% Fs_fm = 500*1e6;
% Так как матлаб в функции sound не позволяет использовать число > 1e6,
% используем 1e6
Fs_fm = 100000;

% Получение комплексных отсчетов аналогично АМ
nsamps = (1024*1024*2);
nstart = 1;
file_FM = fopen('file1EuropaPlus.bin');
fseek(file_FM, nstart, 'bof');
realAndImag = fread(file_FM, [2, nsamps], 'short');
RealParts = realAndImag(1,:);
ImagParts = realAndImag(2,:);
fclose(file_FM);
ComplexSamples = complex(RealParts, ImagParts);

% вычисляем зависимость чатоты от времени. w(t) = w_д*s_m(t), w_д -
% девиация частоты. s_m(t) - модулирующая функция

% s_m = (gradient(ImagParts).*RealParts-gradient(RealParts).*ImagParts)./(RealParts.^2+ImagParts.^2);
s_m = atan2(ImagParts, RealParts);
s_m = gradient(s_m);

% figure
% plot(s_m(1:100000));
% sound(s_m, Fs_fm);