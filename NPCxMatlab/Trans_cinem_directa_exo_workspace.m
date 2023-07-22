%La transformada cinematica directa es la relacin matematica que nos indica
%cual es la posicion del extremo de un robot en función de la posición de
%cada eslabón móvil, es decir, de sus coordenadas articulares.

%Ejemplo para calcular la posición del extremo del robot en función de las
%coordenadas articulares
clc
clear all
close all
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Definimos el tiempo de simulacion y el numero de frames de la pelicula
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

t_sim = 10;
n_frames = 50; %<- numero de frames de la pelicula
t = linspace(0,t_sim,n_frames);

a2= 33.5;  %%33.5° angulo de corrección
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Definimos la trayectoria del eslabón
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Definición del giro del primer eslabón (angulo de flexión-extensión)
%rango 0 a 230°
q1i = -0*pi/180; qlf = 230*pi/180;
q1  = linspace(q1i,qlf,n_frames);
%Definición del giro del segundo eslabón (angulo de abducción)
%rango de 0° a 225°
q2i = -a2*pi/180; q2f = (-a2+225)*pi/180;
q2  = linspace(q2i,q2f,n_frames);
%Definición del giro del tercer eslabón (flexion extennsión del codo)
%rango 0 a 145°
q3i = 0*pi/180; q3f = 145*pi/180;
q3  = linspace(q3i,q3f,n_frames);

a = 30*pi/180;


figure;
for i = 1:n_frames
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Diseñamos el exoequeleto: calculamos los triedros y los puntos de los eslabones
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

T0 = [1 0 0 0
      0 1 0 0
      0 0 1 0
      0 0 0 1];
P0 = T0(1:3,4);
%Triedro B: Triedro del inicio de la base
L0 = 420/2;
TB = Traslaciony(T0,-L0);
PB = TB(1:3,4);
%Triedro 1: Triedro de final del primer eslabon
L1 = 0;
T1a = RotacionY(TB,a);
T1b = RotacionZ(T1a,a + q1(i));
T1c = Traslacionz(T1b,L1);
P1 = T1b(1:3,4);

%Triedro 2: Triedro de final del segundo eslabon
L2 = 360;
T2a = RotacionY(T1b,q2(i));
T2b = Traslacionz(T2a,L2);
P2 = T2b(1:3,4);

%Triedro 3: Triedro de final del tercer eslabon
L3 = 275;
T3a = RotacionY(T2b,-q3(i));
T3b = Traslacionz(T3a,L2);
P3 = T3b(1:3,4);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Diseñamos el exoesqueleto en 3D y sus proyecciones
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Representacion en 3D

subplot(3,3,[1 2 4 5 7 8]); hold on;
dibujaTriedro(T0);     dibujaTriedro(TB);
dibujaTriedro(T1b);    dibujaTriedro(T2b);     %dibujaTriedro(T3);
dibujaEslabon(P0,PB);  dibujaEslabon(PB,P1);
dibujaEslabon(P1,P2);  dibujaEslabon(P2,P3);

ws(i,:,:) = P3;
plot3(ws(:,1,:),ws(:,2,:),ws(:,3,:),'.');

marco = 700; % marco de la grafica representada
axis([-marco marco -marco marco -marco marco]);
box on; set(gca, 'View', [20 20]);
fsize = 16;

xlabel('Eje X ','FontName','Arial','FontSize', fsize);
ylabel('Eje Y','FontName','Arial','FontSize', fsize);
zlabel('Eje Z','FontName','Arial','FontSize', fsize);
s1 = '\fontsize{16} Escala (E 1:10 cm)';
s2 =['\fontsize{16}  tiempo de simulación =' num2str(t(i)) ' segundos'];
title( strcat(s1,s2));

%text(-6,-6,-6,[' Posicion del extremo: [ ' num2str(P2(1)) ',' num2str(P2(2)) ',' num2str(P2(3)) ']']);

% % Proyección en el eje x
% 
subplot(3,3,3); hold on;
dibujaEslabon(P0,PB); dibujaEslabon(PB,P1);  dibujaEslabon(P1,P2);
%dibujaEslabon(P2,P3);
%marco = 200; % marco de la grafica representada
axis([-marco marco -marco marco -marco marco]);
box on; set(gca, 'View', [0 0]); 

xlabel('X','FontName','Arial','FontSize', fsize);
ylabel('Y','FontName','Arial','FontSize', fsize);
zlabel('Z','FontName','Arial','FontSize', fsize);
title('\fontsize{16} Proyección eje X');

% Proyección en el eje y
% 
% subplot(3,3,6); hold on;
% dibujaEslabon(P0,PB); dibujaEslabon(PB,P1);  dibujaEslabon(P1,P2);
% dibujaEslabon(P2,P3);
% %marco = 8; % marco de la grafica representada
% axis([-marco marco -marco marco -marco marco]);
% box on; set(gca, 'View', [90 0]); 
% xlabel('X','FontName','Arial','FontSize', fsize);
% ylabel('Y','FontName','Arial','FontSize', fsize);
% zlabel('Z','FontName','Arial','FontSize', fsize);
% 
% title('\fontsize{16} Proyección eje Y');

% % Proyección en el eje z

% subplot(3,3,9); hold on;
% dibujaEslabon(P0,PB); dibujaEslabon(PB,P1);  dibujaEslabon(P1,P2);
% dibujaEslabon(P2,P3);
% %marco = 8; % marco de la grafica representada
% axis([-marco marco -marco marco -marco marco]);
% box on; set(gca, 'View', [90 90]); 
% xlabel('X','FontName','Arial','FontSize', fsize);
% ylabel('Y','FontName','Arial','FontSize', fsize);
% zlabel('Z','FontName','Arial','FontSize', fsize);
% title('\fontsize{16} Proyección eje Z');
% 
% %Pantalla completa ...
% tamano_pantalla = get(0, 'ScreenSize');
% set(gcf, 'Position', [0 0 tamano_pantalla(3) tamano_pantalla(4) ])

pause(0.01);
if (i~=n_frames)
    clf;
end
end
