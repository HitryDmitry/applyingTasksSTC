% clear all;
close all;
clc

%% Демодуляция АМ сигнала
% % Частота дискретизации
% Fs_am = 32000;
% 
% % сохраняем id файла в переменную fid
% fid = fopen('am_sound.dat');
% % Извлекаем данные (считываем в однострочный массив)
% data = fread(fid, [1, Inf], 'int32');
% % Извлекаем вещественные и мнимые части
% complexData = complex(data(1:2:end), data(2:2:end));
% % Закрываем файл
% fclose(fid);
% 
% % Из графика действительной части сигнала получаем, что нормированная
% % частота равна 360/180 (градусов/отсчет) или pi/90 (рад/отсчет)
% % С помощью ФНЧ выделяем низкочастотную составляющую
% filtered_signal = lowpass(complexData, pi/90);
% 
% % Умножаем на 1e-5, чтобы уменьшить амплитуду, избавляемся от постоянной 
% % составляющей (-3.2) и получаем модулирующий сигнал
% modulating_signal = abs(filtered_signal)*1e-5-3.2;
% % plot(modulating_signal);
% % Добавим фильтр скользящего среднего
% windowSize = 10; 
% b = (1/windowSize)*ones(1,windowSize);
% a = 1;
% average_filtered_signal = filter(b,a,real(modulating_signal));
% 
% % sound(average_filtered_signal, Fs_am);  % Перед проигрыванием уменьшить
% % звук компьютера!!

%% Демодуляция FM сигнала

% Частота дискретизации FM сигнала
Fs_fm_original = 500*1e6;
% Так в функции sound частота дискретизации должна лежать в пределах от
% 1000 до 384 000, то используем 44100
Fs_fm = 44100;

% Получение комплексных отсчетов аналогично АМ
FM_file_id = fopen('file1EuropaPlus.bin');
data = fread(FM_file_id, [1, Inf], 'float32');
ComplexSamples = complex(data(1:2:end), data(2:2:end));
% Закрываем файл
fclose(FM_file_id);

% % Графики для построения спектров исходного сигнала (от нормированной
% % частоты fx)
% NFFT = 2^18; % FFT size, this may not be all the data and that's ok
% fx = [0:NFFT-1]/NFFT - 0.5; % Normalized frequency (if NFFT is an even number)
% ComplexSamplesAmpSpec = abs(fftshift(fft(ComplexSamples,NFFT))); % Compute the spectrum
% subplot(411); plot(fx,20*log10(ComplexSamplesAmpSpec)); % Plot in normalized frequency

%% До прореживания необходимо пропустить сигнал через ФНЧ
% Пропускаем через ФНЧ с частотой среза, совпадающещей с новой частотой
% Найквиста
filtered_signal = lowpass(ComplexSamples, Fs_fm/2, Fs_fm_original);

% Проверка спектра
filtered_signal_amp_spec = abs(fftshift(fft(filtered_signal,NFFT))); % Compute the spectrum
subplot(412); plot(fx,20*log10(filtered_signal_amp_spec)); % Plot in normalized frequency

% Прореживание (из графиков спектра можно видеть, что прореживание без 
% появления ложных частотможно сделать только на 5
D = 5; % Decimation factor
Downsampled_signal = filtered_signal(1:D:end);
Fs_fm_original = Fs_fm_original/D;
% Проверка спектра
ComplexSamplesAmpSpec = abs(fftshift(fft(Downsampled_signal,NFFT))); % Compute the spectrum
subplot(413); plot(fx,20*log10(ComplexSamplesAmpSpec));% Plot in normalized frequency

%% Попытки построить спектрограмму
% Nx = length(Downsampled_signal);
% nsc = floor(Nx/100);
% nov = floor(nsc/2);
% nff = max(256,2^nextpow2(nsc));
% 
% [s, f, t] = spectrogram(Downsampled_signal,hamming(nsc),nov,nff);
% figure
% waterplot(s, f, t);

% s = spectrogram(filtered_signal);
% figure
% spectrogram(filtered_signal,'yaxis')

% M = 8;
% L = 3;
% g = bartlett(M);
% Ndft = 16;
% 
% [s,f,t] = spectrogram(filtered_signal,g,L,Ndft,Fs_fm_original);
% figure
% waterplot(s, f, t);

%% Демодуляция
s_m = unwrap(atan2(imag(Downsampled_signal), real(Downsampled_signal)));

% Создаем дифференцирующий фильтр
L = 19; % Filter delay
n = [-L:L].'; % Time vector
deriv = (-1).^n ./ n; % Derivative filter impulse response
deriv(L+1) = 0; % Fix the zero in the center
deriv = deriv.*hamming(2*L+1); % Include the Hamming window

% Demodulate FM 
s_m = conv(s_m,deriv);

% Проверка спектра
demodulated_amp_spec = abs(fftshift(fft(s_m,NFFT))); % Compute the spectrum
subplot(414); plot(fx,20*log10(demodulated_amp_spec));% Plot in normalized frequency

% Отфильтруем сигнал еще раз и построим спектр
filtered_s_m = lowpass(s_m, 100, Fs_fm_original);
filtered_s_m_amp_spec = abs(fftshift(fft(filtered_s_m,NFFT))); % Compute the spectrum
% Продолжаем построение спектров на новом графике
figure
subplot(411); plot(fx,20*log10(filtered_s_m_amp_spec)); % Plot in normalized frequency

% Еще раз проводим downsampling
D = 5; % Decimation factor
Downsampled_s_m = filtered_s_m(1:D:end);
Fs_fm_original = Fs_fm_original/D;

% Проверка спектра
Downsampled_s_m_amp_spec = abs(fftshift(fft(Downsampled_s_m,NFFT))); % Compute the spectrum
subplot(412); plot(fx,20*log10(Downsampled_s_m_amp_spec));% Plot in normalized frequency


% Добавим фильтр скользящего среднего
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
aver_filtered_signal = filter(b,a,Downsampled_s_m);


sound(Downsampled_s_m, Fs_fm);