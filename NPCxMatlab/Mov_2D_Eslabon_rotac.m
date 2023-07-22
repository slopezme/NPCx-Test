clc
clear all
close all
%Representacion de movimientos 2D
%Eslabon Rotacional
L=1;

%configuracion de la velocidad de movimiento

t_sim =10;
n_frames = 10;
t= linspace(0,t_sim,n_frames);

%Eslabon estacional
L_eslabon =2;
angulo_inicial = 30*pi/180;
angulo_final = 150 *pi/180;
angulo = linspace (angulo_inicial,angulo_final,n_frames);
posicion = [0 0];

%Posicion de punto inicial
x_inicial = posicion(1);
y_inicial = posicion(2);

figure;
ws = zeros(length(t),2);
%comienza el movimiento
for i=1:length(t)
    hold on;
    plot([0 L],[0 0],'r');plot([L],[0],'>r');
    plot([0 0],[0 L],'g');plot([0],[L],'>g');
    
    x_final = x_inicial+L_eslabon*cos(angulo(i));
    y_final = y_inicial+L_eslabon*sin(angulo(i));
    plot([x_inicial x_final],[y_inicial y_final],'b','LineWidth',3);
    plot([x_inicial x_final],[y_inicial y_final],'or');
    ws(i,:) = [x_final,y_final];
    plot(ws(:,1),ws(:,2),'.');
    marco = 5;
    axis([-marco marco -marco marco]);
    box on;
    title(['te' num2str(t(i))]); 
    pause(0.01);
    if (i~=length(t))
        clf
    end
end
    