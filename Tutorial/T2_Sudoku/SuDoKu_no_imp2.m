%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  CSC D84 - Artificial Intelligence - UTSC
%
%  We will practice solving a CSP using backtracking
%  search. The basic search process will be implemented
%  recursively, and the function will keep track of how
%  many times it's called until a solution is found.
%
%  We will then implement the various variable selection
%  strategies discussed in lecture, and check how much
%  of a reduction in search we get from those.
%
%  The task at hand is sudoku. The input is a partially
%  completed sudoku puzzle (which is just a 9x9 array
%  with numbers 1-9. Unassigned squares are marked with 0)
%
%  Your task is to implement backtracking search, then
%  to enhance it to implement variable selection
%  methods as discussed in lecture.
%
%  [sol]=SuDoKu(input)
%
%  input - a 9x9 array containing the initial puzzle
%  sol - a 9x9 array with the completed puzzle
%
%  For input puzzles, have a look here:
%   https://www.sudoku-puzzles-online.com/index.php
%
%  Starter code: F. Estrada, Jan 2020.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [sol]=SuDoKu(input)

    persistent call_count;

    if isempty(call_count)
        call_count=0;
    end;
    call_count=call_count+1;

    % This is a recursive solver for a sudoku.
    % The simplest version of it chooses a variable,
    % then tries all values left to assign to it
    % that DO NOT BREAK any constraints.

    % It assigns that variable each of these values,
    % one at a time, and then tries to recursively
    % solve the smaller resulting sudoku

    % FIRST: Understand the vanilla solver below.
    % Run it and see how long it takes.

    idx=find(input(:)==0);      % Check for remaining unassigned cells
    if (isempty(idx))
        printf('Solved! SuDoKu() calls = %d\n',call_count);
        sol=input;
        return;
    end;

    available_assignments = [1 2 3 4 5 6 7 8 9];
    node = idx(1);
    % for each unassigned variable, find its number of possible values left
    for k=1:length(idx)
        id=idx(k);                          % Get index of the first unassigned cell
        [i,j]=ind2sub(size(input),id);      % Get location [i,j] of this cell in the input array
        ii=floor((i-1)/3);                  % Figure out which 3x3 sub-grid it's in
        jj=floor((j-1)/3);
        % Find the set of remaining values for this cell
        gr=input(1+(ii*3):3+(ii*3),1+(jj*3):3+(jj*3));      % get the subgrid
        row=input(i,:);                                     % get the row
        col=input(:,j);                                     % get the column

        % make a list of values that are already assigned in the row, column, or sub-grid
        taken=unique(union(union(row(find(row>0)),col(find(col>0))),gr(find(gr>0))))';

        % And then determine which values are left that we have to try
        left=unique(setdiff([1 2 3 4 5 6 7 8 9],taken));
        if(length(left)<length(available_assignments))
            available_assignments = left;
            node=idx(k);
        end;
    end;

    % for each possible assignment, find the one that limits the least number of values left for its neighbor
    for l=1:length(available_assignments)
        ass = available_assignments(l);

        [i,j]=ind2sub(size(input),id);
        % top neighbor

    end;
    % Try each of the remaining values, one by one, and attempt to recursively
    % solve the smaller sudoku puzzle. If we run out of values to try without
    % finding a solution, return [] (empty)

    sol=[];                         % No solution just yet
    for i=1:length(available_assignments)
        input(node)=available_assignments(i);
        sol=SuDoKu(input);
        if (~isempty(sol))          % If the recursive call solved it, return!
            return;
        end;
    end;

    %% TO DO:
    %% Modify the solver provided to implement the variable ordering criteria
    %% we discussed in lecture:
    %% 1 - Choose first the variable with fewest remaining values
    %% 2 - If (1) has a tie, choose variable that intervenes in more active constraints
    %% 3 - Once you have chosen a variable, select the least constraining value to try first!
    %%
    %% You have to decide how to keep track of
    %%   # of valid values left for each unassigned cell
    %%   # of active constraints for each unassigned cell
    %%
    %% And you need to think about how to check which values are least constraining.
    %% since there may be many, you'll need to sort them from least constraining to
    %% most constraining!
    %%
    %% BUT! the goal is not to make the most efficient data structure for these things!
    %% (that's not what Matlab's for, you can do that in C if you like). Here, we only
    %% care about *reduction in the number of calls to SuDoKu()* which tells us something
    %% important about the size of the search tree!
    %%
    %% It is assumed you can then make your data structures fast in a language designed
    %% to do so, and the improved algorithm (which uses the variable selection process
    %% we just described) will beat the 'vanilla' version because of the reduction in
    %% the number of nodes that need to be expanded to reach the full solution.
    %%
    %% So - don't spend time writing clever data structures, just get this to work, and
    %% see what you can tell about how much it saves in terms of calls to SuDoKu().

return;


% function [domain_values] = available_assignments(input, idx)
%     for k=1:length(idx)
%         id=idx(k);                          % Get index of the first unassigned cell
%         [i,j]=ind2sub(size(input),id);      % Get location [i,j] of this cell in the input array
%         ii=floor((i-1)/3);                  % Figure out which 3x3 sub-grid it's in
%         jj=floor((j-1)/3);
%         % Find the set of remaining values for this cell
%         gr=input(1+(ii*3):3+(ii*3),1+(jj*3):3+(jj*3));      % get the subgrid
%         row=input(i,:);                                     % get the row
%         col=input(:,j);                                     % get the column

%         % make a list of values that are already assigned in the row, column, or sub-grid
%         taken=unique(union(union(row(find(row>0)),col(find(col>0))),gr(find(gr>0))))';

%         % And then determine which values are left that we have to try
%         left=unique(setdiff([1 2 3 4 5 6 7 8 9],taken));
%         if(length(left)<length(available_assignments))
%             available_assignments = left;
%             node=idx(k);
%         end;
%     end;
% end;