clc;
clear all;
syms q1 q2 q3;

A = [1 0 0 0
     0 1 0 0
     0 0 1 0
     0 0 0 1];

A01= RotacionZ(A,q1)*RotacionZ(A,pi/2)*RotacionX(A,pi/2);
