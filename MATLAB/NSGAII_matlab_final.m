function [objs_of_Ptp1, R, current_front_index] = NSGAII_matlab_final(pop_size,obj_n)

% Population Generation
% Set default values for pop_size and obj_n if not provided
if nargin < 2
   obj_n = 2;
   if nargin < 1
      pop_size = 1000; 
   end
end

% Determine whether to create plots based on the number of objectives
do_the_plot = true;
if obj_n > 2
    do_the_plot = false;
end

% Generate random points in a 2D space
X = randn(pop_size, obj_n);

% Plot the points in the first subplot if plotting is enabled
if do_the_plot 
   subplot(1, 2, 1);
   plot(X(:, 1), X(:, 2), '.');
end

% Sort the points based on the values in the first column
[~, is] = sort(X(:, 1)); % 'is' contains the indices of the elements once
                          % sorted based on the first column
X = X(is, :);

% Create a new set of points with a third dimension (Z) added
Z = linspace(2, -2, pop_size);
Y = X;
Y(:, 2) = Y(:, 2) + Z';

% Plot the new set of points in the second subplot if plotting is enabled
if do_the_plot 
   subplot(1, 2, 2);
   plot(Y(:, 1), Y(:, 2), '.');
end

P = Y;
% Save the matrix to a CSV file named 'output.csv'
%writematrix(P, 'output_16obj.csv');

R = zeros(1, pop_size); % Vector containing the rank of the front for each point
current_front_index = 1; % Index of the front to consider in the while loop
F = false(pop_size, 1);
already_assigned_to_a_front = false(pop_size, 1);

while sum(already_assigned_to_a_front) < pop_size - 1
    for p = 1:size(P, 1) % p is an index relative to the matrix
        if already_assigned_to_a_front(p)
            continue;
        end

        n_p = 0; % Domination counter of point p
        
        for q = 1:size(P, 1)
            if already_assigned_to_a_front(q)
                continue;
            end
            if and(all(P(q, :) <= P(p, :)), any(P(q, :) < P(p, :)))
                n_p = n_p + 1; % p is dominated by q
            end 
        end
    
        if n_p == 0
            R(p) = current_front_index;
            F(p) = true;
        end
    end
    already_assigned_to_a_front(F) = true;
    current_front_index = current_front_index + 1;
end

disp("Fronts found: ");
disp(current_front_index - 1);

% Check that the number of rows in P matches the number of elements in R
if size(P, 1) ~= numel(R)
    disp('The number of rows in P does not match the number of elements in R.');
    return;
end

% Create a new matrix by combining P with the elements of R
newMatrix = [P, R'];

% Save the matrix to a CSV file named 'output.csv'
%writematrix(newMatrix, 'point_rank_16obj.csv');

if do_the_plot
    i = 1;
    color = 0.0;
    % Set all points to yellow color
    colors = ones(1, pop_size) * 1;
    
    while i < current_front_index
        integer = i;  % Integer to search for
        
        % indices = find(R == integer);
         color = color + 0.1;
        colors(R == integer) = color;
        i = i + 1;
    end
    
    % Plot the points with different colors
    scatter(Y(:, 1), Y(:, 2), 10, colors, 'filled');
end
