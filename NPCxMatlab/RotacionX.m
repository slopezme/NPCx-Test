function [B]=rotacionX(A,theta)

rotX = [1   0                0          0
        0   cos(theta)  -sin(theta)     0
        0   sin(theta)   cos(theta)     0
        0       0            0          1];
B = A*rotX;

return;