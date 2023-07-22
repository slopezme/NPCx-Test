clc
clear all
close all

%%variables
syms q1 q2 q3
syms a1 a2 a3
syms d1 d2 d3
syms a01 a12 a23

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
                
T_03 = T_01*T_12*T_23;