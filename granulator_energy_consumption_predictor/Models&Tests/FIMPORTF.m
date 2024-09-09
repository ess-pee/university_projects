function OUTPUT = FIMPORTF(INPUT)

OPTIONS = detectImportOptions(INPUT);
OPTIONS.SelectedVariableNames = [1:48];
OPTIONS.DataRange = 'A5';
OPTIONS.VariableNamesRange = 'A2';

OUTPUT = readtable(INPUT, OPTIONS);
OUTPUT = renamevars(OUTPUT,"Description","TIME");
end