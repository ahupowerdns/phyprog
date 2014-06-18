clear all
fileName = 'simplest-test-file-10-10-1-20';

file = fopen(fileName);
line = fgetl(file);
dimVectorCell = regexp(line,' ','split')' % In newer matlab version: strsplit(line,' ')
for i = 1:size(dimVectorCell,1)
    dimensions(i) = str2num(cell2mat(dimVectorCell(i)))
end

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