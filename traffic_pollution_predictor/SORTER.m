function OUTPUT = SORTER(I1, I2)

UN_AQ = unique(I1,'rows');

LOOKUP_AQ = UN_AQ(:,[1 2]);

result = cell(size(LOOKUP_AQ, 1), 1);
for i = 1:size(LOOKUP_AQ, 1)
    rows = all(I2(:, 1:2) == LOOKUP_AQ(i,:), 2);
    result{i} = I2(rows, 3);
end

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

OUTPUT = transpose(RESH_UNTRANS); % Hence transposing the matrix

end