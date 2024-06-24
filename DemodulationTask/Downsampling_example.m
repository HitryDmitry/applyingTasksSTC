% % Downsampling
% n = 0:0.05:10;
% x = sin(2*pi*n);
% y = x(1:2:length(x));
% subplot(2,1,1); stem(n, x); xlabel('time'); ylabel('amplitude');
% title('input signal');
% subplot(2,1,2); stem(y); xlabel('time'); ylabel('amplitude');
% title('input signal');

Fs = 44.1e3;
t = 0:1/Fs:1-1/Fs;
x = cos(2*pi*2000*t) + 1/2*sin(2*pi*4000*(t-pi/4)) + ...
    1/4*cos(2*pi*8000*t);


[P,Q] = rat(48e3/Fs);
% abs(P/Q*Fs-48000)

xnew = resample(x,P,Q);

% sound(x,44100);
% sound(xnew,48000);