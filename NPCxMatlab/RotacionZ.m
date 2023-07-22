function [B]=RotacionZ(A,theta)

rotZ = [cos(theta)  -sin(theta)     0   0  
        sin(theta)   cos(theta)     0   0
        0               0           1   0
        0               0           0   1];
B = A*rotZ;

return;