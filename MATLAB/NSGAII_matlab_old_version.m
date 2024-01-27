function [objs_of_Ptp1, R, current_front_index] = NSGAII_matlab_final(pop_size,obj_n,do_the_plot )
% objs_of_Ptp1 = genObjs(pop_size,obj_n,do_the_plot )
% 
% This function generates random objective function values
% for a population of pop_size individuals and obj_n objectives.
% The additional (optional) parameter do_the_plot
% allows to plot the generated values.
% 
% Examples
%
% objs_of_Ptp1 = genObjs(1000,2,true);

if nargin < 3
    do_the_plot = true;
    if nargin < 2
       obj_n = 2;
       if nargin < 1
          pop_size = 1000; 
       end
    end
end
if obj_n > 2
    %error('Caso non ancora trattato');
    do_the_plot = false;
end
X = randn(pop_size,obj_n);
if do_the_plot 
   subplot(1,2,1);
   plot(X(:,1),X(:,2),'.');
end
[~,is]=sort(X(:,1)); % is contiene gli indici degli elementi una volta 
                     % ordinati in base alla prima colonna
X = X(is,:);
Z = linspace(2,-2,pop_size);
Y = X;
Y(:,2) = Y(:,2) + Z';

if do_the_plot 
   subplot(1,2,2);
   plot(Y(:,1),Y(:,2),'.');
end
objs_of_Ptp1 = Y;


P = objs_of_Ptp1;
% Salvataggio della matrice in un file CSV chiamato 'output.csv'
%writematrix(P, 'output_16obj.csv');

R = zeros(1,pop_size); % vettore contenente il rank del fronte per ogni p
F = [];
allF=[];    % Mantiene gli indici che sono stati già analizzati e salvati
current_front_index = 1; % indice del fronte da considerare 
                         % nell'iterazione del while

while size(allF)<pop_size
    for p = 1:size(P,1) % p è un indice relativo alla matrice P
        if ismember(p, allF)
            continue;
        end
        n_p = 0;            % domination counter of p 
                            % (contatore di punti da cui è dominato)
        for q = 1:size(P,1) 
            if ismember(q, allF)
                continue;
            end
            if and(all(P(q,:) <= P(p,:)),any(P(q,:) < P(p,:)))
                n_p = n_p +1;   % p è dominato da q
            end 
        end
    
        if n_p == 0
            R(p)=current_front_index;
            F = [F, p]; % aggiungo p alla lista dei punti del fronte corrente
        end
    end
    allF=[allF, F]; % aggiungo la lista dei punti appartenenti al fronte F 
                    % alla lista di tutti i fronti
    F=[];
    current_front_index = current_front_index +1;
end

disp("Fronti trovati: ");
disp(current_front_index-1);



% Verifica che il numero di righe in P e il numero di elementi in R siano gli stessi
if size(P, 1) ~= numel(R)
    disp('Il numero di righe in P non corrisponde al numero di elementi in R.');
    return;
end

% Creazione di una nuova matrice combinando P con gli elementi di R
newMatrix = [P, R'];
% Salvataggio della matrice in un file CSV chiamato 'output.csv'
%writematrix(newMatrix, 'point_rank_16obj.csv');

if do_the_plot
    i=1;
    colore= 0.0;
    % Imposta tutti i punti di colore giallo
    colors = ones(1, pop_size) * 1;
    
    while i < current_front_index
        intero = i;  % Intero da cercare
    
        indici = find(R == intero);
        colore= colore + 0.1;
        colors(indici) = colore;
        i = i +1;
    end
    
    
    % Plotta i punti con colori diversi
    scatter(Y(:,1),Y(:,2), 10, colors,'filled');
end
