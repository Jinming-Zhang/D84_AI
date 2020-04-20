%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Artificial Intelligence - Winter 2020
%
% This function will perform a graph search (UCS) on the
% given input graph.
%
% The graph is passed in as an adjacency list as defined in
% MARS_pathfinding.m (see that script for a description)
%
% You have to implement the search process. You're free to
% do this however you like (i.e. use recursion, or not,
% it's up to you).
%
% Remember:
% For UCS you need to keep track of the current lowest cost
%  to nodes in the graph (initially this is unknown for all
%  nodes except for s, with cost zero)
% UCS expands next the UNEXPANDED node with lowest cost
%  NO LOOPS ARE ALLOWED - you should NEVER expand a node twice
% You need to keep track of the predecessor for each node
%  so you can build the path at the end.
%
% You can use arrays for the costs and predecessors, or you can use
% whatever other data structure you find helps you do your work.
%
% THE SEARCH FUNCTION MUST RETURN A PATH AS A SEQUENCE OF NODES
% TO VISIT - the first entry in the path is the start node's index
% and the last entry is the goal node's index.
%
% You can prepend entries to the Path variable below like so:
% Path=[new_index Path];    <- To append, change the order!
%
% Input parameters:
%
% - A graph 'G' of size Nx4 where N is the number of nodes in the graph
% and edges are real valued (the function should not assume the
% edges are positive, the absence of an edge between two nodes is
% indicated by the value 1e9).
%
% - An array Gid that gives the indexes of the neighbours for each
% node in the graph, as explained in MARS_pathfinding.m
%
% - The index of the start node 's', and the goal node 'g' (we are
% using this function to find a path between two nodes). UCS will
% return the lowest-cost path between 's' and 'g' if edge weights
% are positive. If there are negative edges, the caveats we
% have discussed apply.
%
% Return values:
%
% The PATH that the function has found, it can be empty if there is no
% path between the nodes.
%
% Starter code: F. Estrada, Jan 2020.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Path]=graph_search(G,Gid,s,g,REG_SIZE, mpp)
    visit_his = []; %if the node has been visited
    dis_from_s = 1e9*ones(REG_SIZE * REG_SIZE); %distance from s to each nodes
    predesessor = -1*ones(REG_SIZE * REG_SIZE); %predesessor for each node
    Path = [];

    visit_his = [visit_his s];
    priQ = [s]; %index of nodes ordered by its distance from s
    dis_from_s(s) = 0;


    [Path] = helper(G, Gid, priQ, visit_his, dis_from_s, predesessor,s, g, 1, mpp);

    return;
end;
function [Path]=helper(G, Gid, priQ, visit_his, dis_from_s, predesessor,s, g, count, mpp)
  % no path from s to g
  while(length(priQ)>0)
      % extract next node
      node = priQ(1);
      min_index = 1;
      % >-------- ucs queue -------<
      % for i = 1:length(priQ)
      %     if(dis_from_s(node) > dis_from_s(priQ(i)))
      %         node = priQ(i);
      %         min_index = i;
      %     end;
      % end;

      % >-------- heuristic queue -------<
      for i = 1:length(priQ)
        heuristic = mpp(g) - mpp(node);
        if(heuristic > 0)
          heuristic = heuristic^2;
        else
          heuristic = heuristic;
        end;
          if((dis_from_s(node) + heuristic > (dis_from_s(priQ(i)) + heuristic)))
              node = priQ(i);
              min_index = i;
          end;
      end;

      visit_his = [visit_his node];
      priQ = [priQ(1:min_index-1) priQ(min_index+1:end)];
      % length(priQ)
      % reached goal
      if(node == g)%{
          % return path
          predesessor_index = g;

          Path = [g];
          while(predesessor(predesessor_index) >= 0)
              Path = [predesessor(predesessor_index) Path];
              predesessor_index = predesessor(predesessor_index);
          end;
          % since in the list, predesessor[s] = -1, we add s at the beginning of path
          Path = [s Path];
          return;
      else
        top_neighbor_wt = G(node, 1);
        bottom_neighbor_wt = G(node, 2);
        left_neighbor_wt = G(node, 3);
        right_neighbor_wt = G(node, 4);

        top_neighbor_idx = Gid(node, 1);
        bottom_neighbor_idx = Gid(node, 2);
        left_neighbor_idx = Gid(node, 3);
        right_neighbor_idx = Gid(node, 4);
        % if the neighbor is not visited
        % if(visit_his(top_neighbor_idx) <= 0)
        if(ismember([top_neighbor_idx], visit_his) <= 0 && top_neighbor_wt < 1e9)
          % check and update it's distance from s
          if(dis_from_s(top_neighbor_idx) > dis_from_s(node) + top_neighbor_wt)
            % dis_from_s(node)
            % top_neighbor_wt
            dis_from_s(top_neighbor_idx) = dis_from_s(node) + top_neighbor_wt;
            priQ = [priQ top_neighbor_idx];
          end;
        end;
        % if the neighbor is not visited
        % if(visit_his(bottom_neighbor_idx) <= 0)
        if(ismember([bottom_neighbor_idx], visit_his) <= 0 && bottom_neighbor_wt < 1e9)
          % check and update it's distance from s
          if(dis_from_s(bottom_neighbor_idx) > dis_from_s(node) + bottom_neighbor_wt)
            dis_from_s(bottom_neighbor_idx) = dis_from_s(node) + bottom_neighbor_wt;
            priQ = [priQ bottom_neighbor_idx];
          end;
        end;
        % if the neighbor is not visited
        % if(visit_his(left_neighbor_idx) <= 0)
        if(ismember([left_neighbor_idx], visit_his) <= 0 && left_neighbor_wt < 1e9)
          % check and update it's distance from s
          if(dis_from_s(left_neighbor_idx) > dis_from_s(node) + left_neighbor_wt)
            dis_from_s(left_neighbor_idx) = dis_from_s(node) + left_neighbor_wt;
            priQ = [priQ left_neighbor_idx];
          end;
        end;

        % if the neighbor is not visited
        % if(visit_his(right_neighbor_idx) <= 0)
        if(ismember([right_neighbor_idx], visit_his) <= 0 && right_neighbor_wt < 1e9)
          % check and update it's distance from s
          if(dis_from_s(right_neighbor_idx) > dis_from_s(node) + right_neighbor_wt)
            dis_from_s(right_neighbor_idx) = dis_from_s(node) + right_neighbor_wt;
            priQ = [priQ right_neighbor_idx];
          end;
        end;

        % recursively find the path
        % helper(G, Gid, priQ, visit_his, dis_from_s, predesessor,s, g, count)
        % end;
      end;

  end; % end while




  Path = [];
  return;
end; % end function

function [heuristic] = heuristic_function(node, g, mpp)
  heuristic = mpp(g) - mpp(node);
  if(heuristic > 0)
    heuristic = heuristic^2;
  else
    heuristic = heuristic;
  end;
  % heuristic = 0;

end;





% function [heuristics] = calc_heuristic(G,Gid,s,g,REG_SIZE, mpp)
%   heuristics = ones(REG_SIZE * REG_SIZE);
%   % calculate heuristic value for each node in the graph
%   map = mpp';
%   for j=1:size(map,1)
%     for i=1:size(map,2)
%       id1=i+((j-1)*REG_SIZE);
%       % heuristics(id1) = heuristic_function(id1, g, mpp);
%       heuristics(id1) = mpp(id1) - mpp(g);
%     end;
%   end;
% end;% end function













































%priQ: index of nodes, in the order of cost from node s
% function [Path]=helper(G, Gid, priQ, visit_his, dis_from_s, predesessor,s, g, count)
%   % no path from s to g
%     if(length(priQ) <= 0)
%       Path = [];
%       return;
%     else
%       %%%%%%%%%%% extract next node from q %%%%%%%%%%%%%
%       % for each node in q, check it's distance from s
%       % extract the node with mininum distance from s
%       node = priQ(1);
%       min_index = 1;
%       for i = 1:length(priQ)
%           if(dis_from_s(node) > dis_from_s(priQ(i)))
%               node = priQ(i);
%               min_index = i;
%           end;
%       end;
%       % dis_from_s
%       % [mindist mindistindx] = min(dis_from_s);
%       % fprintf("minimun distance node %i at index of %i\n", mindist, mindistindx);
%       fprintf("extracted node index %i with distance %i\n", node, dis_from_s(node));
%       % mark this node and visited
%       visit_his = [visit_his node];
%       % (node) = 1;
%       % update q
%       priQ = [priQ(1:min_index-1) priQ(min_index+1:end)];
%       fprintf("expanding node: %i, depth: %i\n", node, count);
%       % fprintf("expanding node: %i\n", node);
%       count = count + 1;
%       %%%%%%%%%%%%% check if the node is goal %%%%%%%%%%%%%
%       if(node == g)%{
%           % return path
%           predesessor_index = g;

%           Path = [Path g];
%           while(predesessor(predesessor_index) >= 0)
%               Path = [predesessor(predesessor_index) Path];
%               predesessor_index = predesessor(predesessor_index);
%           end;
%           % since in the list, predesessor[s] = -1, we add s at the beginning of path
%           Path = [s Path];
%         %}
%       % if it's not the goal, expand it's neighbor to the queue
%       else
%         top_neighbor_wt = G(node, 1);
%         bottom_neighbor_wt = G(node, 2);
%         left_neighbor_wt = G(node, 3);
%         right_neighbor_wt = G(node, 4);

%         top_neighbor_idx = Gid(node, 1);
%         bottom_neighbor_idx = Gid(node, 2);
%         left_neighbor_idx = Gid(node, 3);
%         right_neighbor_idx = Gid(node, 4);
%         % if the neighbor is not visited
%         % if(visit_his(top_neighbor_idx) <= 0)
%         if(ismember([top_neighbor_idx], visit_his) <= 0)
%           % check and update it's distance from s
%           if(dis_from_s(top_neighbor_idx) > dis_from_s(node) + top_neighbor_wt)
%             dis_from_s(top_neighbor_idx) = dis_from_s(node) + top_neighbor_wt;
%             priQ = [priQ top_neighbor_idx];
%           end;
%         end;
%         % if the neighbor is not visited
%         % if(visit_his(bottom_neighbor_idx) <= 0)
%         if(ismember([bottom_neighbor_idx], visit_his) <= 0)
%           % check and update it's distance from s
%           if(dis_from_s(bottom_neighbor_idx) > dis_from_s(node) + bottom_neighbor_wt)
%             dis_from_s(bottom_neighbor_idx) = dis_from_s(node) + bottom_neighbor_wt;
%             priQ = [priQ bottom_neighbor_idx];
%           end;
%         end;
%         % if the neighbor is not visited
%         % if(visit_his(left_neighbor_idx) <= 0)
%         if(ismember([left_neighbor_idx], visit_his) <= 0)
%           % check and update it's distance from s
%           if(dis_from_s(left_neighbor_idx) > dis_from_s(node) + left_neighbor_wt)
%             dis_from_s(left_neighbor_idx) = dis_from_s(node) + left_neighbor_wt;
%             priQ = [priQ left_neighbor_idx];
%           end;
%         end;

%         % if the neighbor is not visited
%         % if(visit_his(right_neighbor_idx) <= 0)
%         if(ismember([right_neighbor_idx], visit_his) <= 0)
%           % check and update it's distance from s
%           if(dis_from_s(right_neighbor_idx) > dis_from_s(node) + right_neighbor_wt)
%             dis_from_s(right_neighbor_idx) = dis_from_s(node) + right_neighbor_wt;
%             priQ = [priQ right_neighbor_idx];
%           end;
%         end;

%         % recursively find the path
%         helper(G, Gid, priQ, visit_his, dis_from_s, predesessor,s, g, count)
%       end;
%     end;

% end;