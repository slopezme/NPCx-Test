%funcion para realizar la traslacion de un triedro en el eje x
function [B]=traslacionX(A,d)

trasX = [1 0 0 d
         0 1 0 0
         0 0 1 0
         0 0 0 1];
B = A*trasX;

return;

