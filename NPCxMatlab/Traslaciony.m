
%funcion para realizar la traslacion de un triedro en el eje y

function [B]=traslacionY(A,d)

trasY = [1 0 0 0
         0 1 0 d
         0 0 1 0
         0 0 0 1];
B = A*trasY;

return;