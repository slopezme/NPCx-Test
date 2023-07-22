%funcion que dibuja un eslabon como una linea recta entre 2 triedros
%desplazados entre si

%p_i: vector (xi,yi,zi) del punto del comienzo del eslabon
%p_f: vector (xf,yf,zf) del punto del final del eslabon
function dibujaEslabon(p_i,p_f)
plot3([p_i(1) p_f(1)], [p_i(2) p_f(2)], [p_i(3) p_f(3)], 'k', 'LineWidth',4);

return;