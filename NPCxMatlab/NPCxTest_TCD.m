%% calculo de la cinematica directa para 2 grados de libertal 
syms q0 q1;
syms h1 h2;

a = 90*pi/180;


% Matriz inicial

A0 = [ 1 0 0 0
       0 1 0 0
       0 0 1 0
       0 0 0 1 ];

% Transformacion del sistema 0 al 1
A01R = RotacionZ(A0, q0); 
A01 = Traslacionx(A01R,h1);


% Transformacion del sistema 1 al 2

A12R = RotacionZ(A0, q1); 
A12 = Traslacionx(A12R,h2);


% 1. Matriz de transformacion Homogenea

H = simplify(A01 * A12 );



%% 2. vector de posición de j0 a j1
p1 = H(1:3, 4);



%% 3. Vector local de posición p2 (que va de j1 a j2 en el marco de referencia local de j1)
syms x2 y2 z2
p2_local = [x2; y2; z2];

% Aplicar la transformación al vector p2 al sistema global (sistema 0)
p2_global = simplify(A01 * A12 * [p2_local; 1]);

% Extraer las coordenadas x, y, z del vector p2_global
x2_global = p2_global(1);
y2_global = p2_global(2);
z2_global = p2_global(3);


%% Calcular las distancias h1 y h2 en base a los markers obtenidos


% coordenadas de j0
x0 = 0;
y0 = 0;
z0 = 0;

% coordenadas de j1

x1 = h1 * cos(q0);
y1 = h1 * sin(q0);
z1 = 0;

% Coordenadas de j2

x2 = x1 + h2 * cos(q0 + q1);
y2 = y1 + h2 * sin(q0 + q1);
z2 = 0;


%%distancia  H1
h1 = sqrt((x1 - x0)^2 + (y1 - y0)^2 + (z1 - z0)^2);

%%distancia H2

h2 = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2);