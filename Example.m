fileName = 'test-file-binary-10-10-10-100-0';

file = fopen(fileName);
line = fgetl(file);
dimVector = regexp(line,' ','split')';  % In newer matlab version: strsplit(line,' ')
dimensions = str2num(cell2mat(dimVector))

data = fread(file);
for z = 1:dimensions(3)
    for y = 1:dimensions(2)
        for x = 1:dimensions(1)
            dataPoint = (z-1) * (dimensions(3) * dimensions(2)) + (y-1) * dimensions(2) + x;
            curData = data(dataPoint);
            fprintf('z = %d, y = %d, z = %d, data = %d\n',z,y,x,curData);
        end
    end
end