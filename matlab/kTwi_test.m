
clc, clear, close all;

s = kSerial(115200, 'clear');
s.setRecordBufferSize(1024 * 64);
s.open();

i2c = kTwi(s);

slaveAddress = 0x0C;

deviceaddr = i2c.scandevice('printon');
devicereg = i2c.scanregister(deviceaddr(1), 'printon');

% {
% init
reg = [
    0x32, 0x01;
    0x30, 0x00;
    0x34, 0x00;
    0x41, 0x24;
    0x35, 0x02;
    0x31, 0x0A;
];
fprintf('[W]');
i2c.write(slaveAddress, reg(1, 1), reg(1, 2));
fprintf(' %02X:%02X', reg(1, 1), reg(1, 2));
i2c.delay(0.5);
for i = 2 : size(reg, 1)
    i2c.write(slaveAddress, reg(i, 1), reg(i, 2));
    fprintf('  %02X:%02X', reg(i, 1), reg(i, 2));
end
fprintf('\n\n');

% read id
deviceid = i2c.read(slaveAddress, 0, 1);
fprintf('device id = 0x%02X\n', deviceid);
fprintf('\n');
s.delay(0.1);
%}

% {
sv = struct;
sv.raw = [];

for i = 1 : 1000
    res = i2c.read(slaveAddress, 0x10, 7);
    if ~isempty(res) && (size(res, 1) == 7)
        DRDY = mod(res(1), 2);
        lsb = typecast(uint8(res(2:end)), 'int16');
        fprintf('[RAW %d] %6d, %6d, %6d\n', DRDY, lsb);

        sv.raw = [sv.raw, lsb];
    end
end

s.close();



fig = figure(1);
hold on; grid on; axis equal;
plot3(sv.raw(1, :), sv.raw(2, :), sv.raw(3, :), 'r.');
xlabel('x-axis(LSB)');
ylabel('y-axis(LSB)');
zlabel('z-axis(LSB)');
