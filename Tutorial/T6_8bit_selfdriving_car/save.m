function [] = save(Q_table, Policy, V_star)
fid = fopen('Q_table.txt','wt');
for ii = 1:size(Q_table,1)
    fprintf(fid,'%g\t',Q_table(ii,:));
    fprintf(fid,'\n');
end;
fclose(fid)

fid = fopen('Policy.txt','wt');
for ii = 1:size(Policy,1)
    fprintf(fid,'%g\t',Policy(ii,:));
    fprintf(fid,'\n');
end;
fclose(fid)

fid = fopen('V_star.txt','wt');
for ii = 1:size(V_star,1)
    fprintf(fid,'%g\t',V_star(ii,:));
    fprintf(fid,'\n');
end;
fclose(fid)
end;