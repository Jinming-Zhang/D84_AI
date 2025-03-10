%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Artificial Intelligence, Winter 2020
% 
% Unit 1 - Heuristic search
%
%  You have managed to land the next-gen robot exporer
% on the surface of MARS. It comes equipped with the
% latest topography data, and a very clever, heuristic
% search algorithm that it can use to find the optimal
% path between two points it needs to travel.
%
% Optimal here means: Using the least possible energy
%
% The robot spends energy when going uphill (from lower
% to higher elevation) at a rate that is quadratic
% with the difference in height.
%
% The robot can recharge its battery when going downhill
% at a rate that is (sadly) only linear with the difference
% in height.
%
% Your task is to:
%
% a) Build a graph of the selected section of MARS
% b) Create a search algorithm (based on UCS)
%    to find a path between two
%    points that results in the robot spending the
%    least amount of energy
% c) Figure out what is the best way to define the edge
%    weights (different definitions will give you
%    different paths).
% d) Experiment and see what happens when you change the
%    energy cost for the robot to go up or down hill
%
% Starter code: F. Estrada, Jan 2020
%
% [G, Path,cost]=MARS_pathfinding(REG_SIZE,Xc, Yc, Xs, Ys, Xg, Yg)
%
% - Input REG_SIZE is the size of the sub-region (in pixels)
%   where we will run path finding
%   Optional parameters Xc, Yc, Xs, Ys, Xg, Yg give the location of 
%   the sub-region to explore, and the start and goal pixels within
%   that subregion
% - Returns the graph G, the path found, and the cost for this path. 
%
% The elevation map is provided by NASA (Public Domain)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [G, Path, cost]=MARS_pathfinding(REG_SIZE, Xc, Yc, Xs, Ys, Xg, Yg)

close all;

% Read the elevation map and convert it to a grayscale
% image where brightness = elevation
map_big=double(imread('mars_elevation.jpg'));       % in [0, 255]
map_big=mean(map_big,3);

% Allow the user to select a section of the map. The 
% user clicks on a point, and we will grab a square
% region of size REGION_SIZExREGION_SIZE centered at the point
if (~exist('Xc')||~exist('Yc'))
 figure(1);clf;imagesc(map_big);axis image;colormap(jet);
 [Xc,Yc]=ginput(1);
 Xc=round(Xc);
 Yc=round(Yc);
end;

if (Xc<REG_SIZE/2||Xc>size(map_big,2)-(REG_SIZE/2)||Yc<REG_SIZE/2||Yc>size(map_big,1)-(REG_SIZE/2))
    fprintf(2,'Selected point is too close to the border\n');
    return;
end;

map=map_big(Yc-REG_SIZE/2:Yc+REG_SIZE/2,Xc-REG_SIZE/2:Xc+REG_SIZE/2);
figure(1);clf;imagesc(map);axis image;colormap(jet);title('MARS region selected for exploration');hold on;

%% UPDATE REG_SIZE because we grabbed a square region CENTERED at the pixel clicked
% on by the user - so we end up with an extra row and column :)
REG_SIZE=size(map,1);

% Allow the user to select two points in this region to use for path finding
if (~exist('Xs')||~exist('Ys')||~exist('Xg')||~exist('Yg'))
 % Starting node is at [Xs,Ys] with INDEX = Xs+((Ys-1)*REG_SIZE)
 [Xs,Ys]=ginput(1);
 Xs=min(size(map,2),max(1,round(Xs)));
 Ys=min(size(map,1),max(1,round(Ys)));

 % Destination node is at [Xg,Yg] with INDEX = Xg+((Yg-1)*REG_SIZE)
 [Xg,Yg]=ginput(1);
 Xg=min(size(map,2),max(1,round(Xg)));
 Yg=min(size(map,1),max(1,round(Yg))); 
end;

% Indexes for the start and goal nodes
IDXs=Xs+((Ys-1)*REG_SIZE);
IDXg=Xg+((Yg-1)*REG_SIZE);

fprintf(2,'Selected coordinates [Xc,Yc,Xs,Ys,Xg,Yg]=[%d %d %d %d %d %d]\n',Xc,Yc,Xs,Ys,Xg,Yg);

% Plot both points on the map for visualization
plot(Xs,Ys,'k.','markersize',17);
plot(Xg,Yg,'k.','markersize',17);
plot(Xs,Ys,'y.','markersize',15);
plot(Xg,Yg,'y.','markersize',15);

% TO DO:
% Add code here to complete the graph building step.
%
% We will use an adjacency list of size (REG_SIZE*REG_SIZE * 4). 
%
% * Each pixel in the region is a node, so you have REG_SIZE^2 nodes
%   for a given location x,y, the corresponding index is x+((y-1)*REG_SIZE)
% * Each pixel is connected by edges to its 4 neighbours TOP, BOTTOM, LEFT, RIGHT
%   in that specific order, so in the adjacency list G:
%   G(idx,1) should give the weight of the edge from the pixel at index 'idx' to
%    its TOP neighbour, G(idx,2) is the weight to the BOTTOM neighbour, and so on.
%
% BE CAREFUL - The indexing above corresponds to a ROW-MAJOR order, pixels are
% stored in images in rows from left to right. That's the standard convention.
% But Matlab arrays are indexed as (row, column), so to get the value of pixel
% at (x,y), you have to go to map(y,x)!
% Make sure you're indexing your map() array in the right order or the graph 
% will be wrong, and the path will be wrong, and it will be hard for you to 
% figure out where the problem is!
%
% EDGE WEIGHTS:
% You have to figure out what the edge weights should be (consider the problem you
% are solving and what the path you return should be). They need to depend in some
% way on the difference in elevation between two neighbouring pixels, which is
% equivalent to the difference in brightness between these pixels (a scale factor
% won't change your optimal path).

G=1e9*ones(REG_SIZE*REG_SIZE,4);        % Initially all weights are set to 1e9.
                                        % After you build the graph, any entry
                                        % whose value is still 1e9 indicates 
                                        % there is NO connection between the
                                        % corresponding pair of nodes.
                                        % Note that 0 is a valid edge weight!
                                        % (there may be two pixels for which
                                        % the travel cost is actually zero).

Gid=-1*ones(REG_SIZE*REG_SIZE,4);       % For each node in G, it contains the
        % indexes of the neighbours in the same
        % order as above: TOP, BOTTOM, LEFT, RIGHT
        % e.g.
        % G(150,1) gives you the weight of the
        %          edge linking node 150 to its
        %          TOP neighbour
        % Gid(150,1) gives you the INDEX of 
        %            node 150's TOP neighbour

fprintf(2,'Number of nodes in G=%d\n',size(G,1));

%% TO DO:
%% Complete the part of the code below that sets the weights for each of the neighbours
%% You're free to define the weight in a way you think makes sense for the problem,
%% this will change what path your code will find. The point is to figure out what
%% is a reasonable and good way to set the weights.

mpp=map';
for j=1:size(map,1)
 for i=1:size(map,2)
   id1=i+((j-1)*REG_SIZE);
   if (j>1)  % TOP neighbour
       id2=i+((j-1-1)*REG_SIZE);
       Gid(id1,1)=id2;
       if (id1>REG_SIZE*REG_SIZE||id2>REG_SIZE*REG_SIZE) fprintf(2,'id1=%d, id2=%d, i=%d, j=%d, TOP\n',id1,id2,i,j); end;
       v1=mpp(id1);
       v2=mpp(id2);
       delta=v2-v1;
%       G(id1,1)=1e9;                %% <--- Change this to something that makes sense for this edege!
       if (delta > 0) % going uphill from node to it's top neighbor
        G(id1, 1) = costFN_up(delta);
       else
          G(id1, 1) = costFN_dn(delta);
       end; 
   end;
   if (j<REG_SIZE)  % BOTTOM neighbour
       id2=i+(j*REG_SIZE);
       Gid(id1,2)=id2;
       if (id1>REG_SIZE*REG_SIZE||id2>REG_SIZE*REG_SIZE) fprintf(2,'id1=%d, id2=%d, i=%d, j=%d, BOTTOM\n',id1,id2,i,j); end;
       v1=mpp(id1);
       v2=mpp(id2);
       delta=v2-v1;
%       G(id1,2)=1e9;                %% <--- Change this to something that makes sense for this edege!
      if (delta > 0) % going uphill from node to it's Bottom neighbor
        G(id1, 2) = costFN_up(delta);
       else
          G(id1, 2) = costFN_dn(delta);
       end; 
   end;
   if (i>1)  % LEFT neighbour
       id2=(i-1)+((j-1)*REG_SIZE);
       Gid(id1,3)=id2;
       if (id1>REG_SIZE*REG_SIZE||id2>REG_SIZE*REG_SIZE) fprintf(2,'id1=%d, id2=%d, i=%d, j=%d, RIGHT\n',id1,id2,i,j); end;
       v1=mpp(id1);
       v2=mpp(id2);
       delta=v2-v1;
%       G(id1,3)=1e9;                %% <--- Change this to something that makes sense for this edege!
       if (delta > 0) % going uphill from node to it's Left neighbor
        G(id1, 3) = costFN_up(delta);
       else
          G(id1, 3) = costFN_dn(delta);
       end; 
   end;
   if (i<REG_SIZE)  % RIGHT neighbour
       id2=(i+1)+((j-1)*REG_SIZE);
       Gid(id1,4)=id2;
       if (id1>REG_SIZE*REG_SIZE||id2>REG_SIZE*REG_SIZE) fprintf(2,'id1=%d, id2=%d, i=%d, j=%d, LEFT\n',id1,id2,i,j); end;
       v1=mpp(id1);
       v2=mpp(id2);
       delta=v2-v1;
%       G(id1,4)=1e9;                %% <--- Change this to something that makes sense for this edege!
      if (delta > 0) % going uphill from node to it's Right neighbor
        G(id1, 4) = costFN_up(delta);
       else
          G(id1, 4) = costFN_dn(delta);
       end; 
   end;  
 end;
end;
fprintf(2,'Maximum edge weight: %f\n',max(G(find(G<1e9))));
fprintf(2,'Minimum edge weight: %f\n',min(G(find(G<1e9))));
fprintf(2,'Entries in G left at 1e9: %f\n',length(G(find(G>=1e9))));

Path=[];        % Use this array to store the path your code will find

% TO DO:
% Once you have a graph, write code to do search on the graph. Do that
% in the function called 'graph_search.m'.

tic;
Path=graph_search(G,Gid,IDXs,IDXg, REG_SIZE, mpp);       
tt=toc;
fprintf(2,'The search process took %f seconds.\n',tt);

%% Plot the path returned by the graph search process, and compute the
%% energy cost for the robot for the returned path.
for i=1:length(Path)
    y=1+(floor(Path(i)/REG_SIZE));
    x=mod(Path(i),REG_SIZE)+1;
    plot(x,y,'k.','markersize',17);
    plot(x,y,'y.','markersize',15);
end;

cost=0;
map=map';
for i=1:length(Path)-1
    v1=map(Path(i));
    v2=map(Path(i+1));
    delta=v2-v1;
    if (delta>0) cost=cost+costFN_up(delta);
    else if (delta<0) cost=cost-costFN_dn(delta); end;
    end;
end;

fprintf(2,'The energy cost for this path is %f energy units\n',cost);

return;
%% The MARS_pathfinding() function ends here

%% Cost functions for the robot - these are used to calculate the final cost of
%% the path, and you can (after you complete your assignment) play with them to
%% see how the cost of the path changes if the robot spends more or less
%% energy going uphill or downhill.

function [cost]=costFN_up(delta)        % Used when robot is going uphill
    cost=delta^2;
    return;
    
function [cost]=costFN_dn(delta)        % Used when robot is going downhill
    cost=delta;                         % delta is already negative, mind that!
    return;
    
