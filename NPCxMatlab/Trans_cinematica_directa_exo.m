clc
clear all
close all

%%variables
syms q1 q2 q3

d1 = 0;
d2 = 0;
d3 = 0;


a01 = 0;
a12 = 360;
a23 = 275;

a1 = 0;
a2 = -pi/2;
a3 = 0;

%%the coefficients are calculated according to the transformation described by the D-H method
T_01 = [ cos(q1)                 -sin(q1)                       0                    a01
         sin(q1)*cos(a1)      cos(q1)*cos(a1)      -sin(a1)   -d1*sin(a1)
         sin(q1)*sin(a1)      cos(q1)*sin(a1)       cos(a1)    d1*cos(a1)
                    0                          0                        0                   1           ];
                
T_12 = [ cos(q2)                 -sin(q2)                       0                     a12
         sin(q2)*cos(a2)      cos(q2)*cos(a2)      -sin(a2)   -d2*sin(a2)
         sin(q2)*sin(a2)      cos(q2)*sin(a2)       cos(a2)    d2*cos(a2)
                    0                          0                        0                   1           ];

T_23 = [ cos(q3)                 -sin(q3)                       0                    a23
         sin(q3)*cos(a3)      cos(q3)*cos(a3)      -sin(a3)   -d3*sin(a3)
         sin(q3)*sin(a3)      cos(q3)*sin(a3)       cos(a3)    d3*cos(a3)
                    0                          0                        0                   1           ];
%%transformation matrix among first coordinate system and final efector is computed

T_12 = [ cos(q2)    -1.0*sin(q2)            0         366.0
            0             0                 1            0
        -1.0*sin(q2),    -1.0*cos(q2)       0         0
            0               0               0                1.0];


T_03 = T_01*T_12*T_23;
T_01
T_12
T_23