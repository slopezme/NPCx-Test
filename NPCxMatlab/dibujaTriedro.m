function dibujaTriedro(M)
%figure; hold on;
%saca de la matriz los vectores n,o,a y la posicion P
n = M(1:3,1); o = M(1:3,2); a = M(1:3,3);
P = M(1:3,4); x = P(1); y = P(2); z = P(3);
%dibuja el vector n
plot3([x x+n(1)],[y y+n(2)],[z z+n(3)],'r' ,'LineWidth',[2]);
plot3([ x+n(1) ],[y+n(2)  ],[ z+n(3) ],'>r','LineWidth',[2]);
%dibuja el vector o
plot3([x x+o(1)],[y y+o(2)],[z z+o(3)],'g' ,'LineWidth',[2]);
plot3([ x+o(1) ],[y+o(2)  ],[ z+o(3) ],'>g','LineWidth',[2]);
%dibuja el vector a
plot3([x x+a(1)],[y y+a(2)],[z z+a(3)],'b' ,'LineWidth',[2]);
plot3([ x+a(1) ],[y+a(2)  ],[ z+a(3) ],'>b','LineWidth',[2]);
%dibuja el origen del triedro
plot3([ x      ],[  y     ],[   z    ],'ok','LineWidth',[2]);

% marco = 5; %marco de la grafica representada
% axis([-marco marco -marco marco -marco marco]);
% box on; set(gca,'View',[35 17]);
% xlabel('x'); ylabel('y'); zlabel('z');
return;