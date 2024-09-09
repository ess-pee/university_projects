function OUTPUT = CLEANER_MEAN(INPUT);

unique_days = unique(INPUT(:,1)); unique_hours = unique(INPUT(:,2)); 
l_days = length(unique_days); l_hours = length(unique_hours);

mat_size = l_days*l_hours;

data_mat = zeros(mat_size,2);

for i = 1:(l_days)

    for j = 1:(l_hours)
        row_no = (i-1)*l_hours + j;

        data_mat(row_no,1) = unique_days(i);

        data_mat(row_no,2) = unique_hours(j);  

    end

end

% By here we have the hours and days properly sorted now we need to add the
% corresponding median traffic flows  to each hour.

clear i j;

unique_values = unique(data_mat, 'rows');

result = cell(size(unique_values, 1), 1);
for i = 1:size(unique_values, 1)
    rows = all(INPUT(:, 1:2) == unique_values(i,:), 2);
    result{i} = INPUT(rows, 3);
end

% By here we have sorted the data into a cell which contains a traffic flow
% reading for each indexed hour. But the data is inaccessible to us right
% now so we need to convert it into matrix form so that we can perform the
% median.


% We perform this operation because Matlab does not behave when you have
% discrepancy in cell sizes when converting to matrix
max_rows = max(cellfun(@(x) size(x,1), result));
max_cols = max(cellfun(@(x) size(x,2), result));

% Pad cells with zeros
PAD_CELL = cellfun(@(x) [x, zeros(size(x,1), max_cols-size(x,2)); zeros(max_rows-size(x,1), max_cols)], result, 'UniformOutput', false);

PAD_MAT = cell2mat(PAD_CELL);

LC = length(PAD_CELL);
WC = numel(PAD_CELL{1});

RESH_UNTRANS = reshape(PAD_MAT,[WC,LC]); % Matlab complexities somehow breaks reshaping to a 720x12 matrix

PRE_MED_TRAFF = transpose(RESH_UNTRANS); % Hence transposing the matrix

MEAN_TRAFF = mean(PRE_MED_TRAFF,2); 

data_mat(:,3) = MEAN_TRAFF; 

OUTPUT = data_mat; % Final pre-filtered traffic data 