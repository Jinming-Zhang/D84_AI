%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% CSC D84 - Artificial Intelligence - UTSC
%
% This script will let you practice using local search
% to approximately solve an interesting problem.
%
% The problem at hand is as follows:
%
% We're setting up a chain of pizza restaurants (be
% sure to make a name and logo for them!), and we
% have obtained funding for K locations to be
% opened across the GTA.
%
% We have data on the population density across the
% GTA and we want to set up the restaurant locations
% so that travel time between anyone in the GTA and
% oen of our pizza locations is minimized.
%
% This can be formulated as a k-medians problem:
%
% Given a set of N points (x_i,y_i)
% Select  k  'centroid' locations (x_j, y_j)
%  such that the sum of distances from each
%  (x_i,y_i) to the nearest centroid is
%  minimized.
%
% This problem doesn't lend itself to a nice analytic
% solution, so we will use local search.
%
% Notice that this is strictly speaking NOT a CSP!
% but we're not limited to solving CSPs with local
% search!
%
% Thus - the issues facing you are:
%
% * Generate an initial guess for K locations for
%   our pizza restaurants.
% * Define a neighbourhood that makes sense for this
%   problem (given a current solution, what is a
%   similar solution, and how do we build it from
%   the current one?)
% * Define a set of operations that change the values
%   of your K centroids so as to obtain different
%   solutions in the neighbourhood of the current one
% * Implement the local search algorithm to find an
%   approximate solution to this problem.
%
% * Crunchy: Once you've done all of the above and
%   have a good idea how well/badly it works. Try
%   changing the utility function and see what that does.
%   for example, make the target function the sum of
%   squared distances from points to their nearest
%   centroid - this doesn't sound too different, but
%   in fact it changes the problem a lot - think about
%   why :).
%   You can then invent your own funky utility function
%   for instance - maximize population living within
%   a radius D of one of your restaurant, or something
%   else.
%   The point is, given a utility function, your local
%   search algorithm remains unchanged :)! you can
%   use it to solve *that* problem too.
%
% * One more thing - I tried deterministic annealing
%   on the standard problem and it doesn't improve
%   things at all, but takes much longer. If you
%   define your own utility function, it may be
%   worth your while to implement D.A. and see if
%   it helps! And I would like to know what you
%   discovered!
%
% Inputs:
%
%   K - The number of pizza restaurants we want to find a
%       good location for
%
% Outputs:
%
%   cent - an array of Kx2 centroid coordinates (x,y)
%
% Starter code: F. Estrada, Jan. 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [cent]=pizzaChainer(K)

load points.mat;        % This load  the map and the population data.
                        % Gives a 'likely_map' which is a map of the GTA with
                        % values proportional to population density (for plotting)
                        % An array 'pts' with 2D coordinates (10000 of them!)
                        % of potential customers, distributed according to the
                        % population density map. Hence, for thbis problem N=10000

who                     % Have a look at your variables :)

cent=zeros(K,2);        % This array will hold K centroids, each with (x,y) coordinates

colormap(jet);
cols=.33*colormap;

%%%%%%%%%%%%%%%%%
% TO DO: Implement code to generate an initial guess for where the
% centroids should be. A smart choice may help your search
% find a better local minimum.
%%%%%%%%%%%%%%%%%
%% ADD your centroid initialization here, before the plot commands below!
% amount = ceil(length(pts(:,1)) / K);
% for i = 1:K
%     cent(i,:) = mean(pts(max(amount*(i-1), 1):amount*i, :));
% end;
minx = min(pts(:,1))
maxx = max(pts(:,1))
miny = min(pts(:,2))
maxy = max(pts(:,2))
for i = 1:K
    cent(i,1) = minx + ((maxx-minx) / K) * max(i, 1);
    cent(i,2) = miny + ((maxy-miny) / K) * max(i, 1);
end;

% Plot map and initial centroids (you can copy/paste the lines below into your local-search
% loop to display updated guesses)length
figure(1);clf;imagesc(likely_map);axis image;axis off;colormap(cols);title('Population Density in the GTA');hold on;
plot(pts(:,1),pts(:,2),'y.','markersize',4);
plot(cent(:,1),cent(:,2),'.','color',[0 0 0],'markersize',33);
plot(cent(:,1),cent(:,2),'.','color',[1 1 1],'markersize',30);
drawnow;
disp ("initial centroids: "), disp(cent)
[d_sum, dist, clust_id]=distSum(pts,cent);
disp ("initial dist sum: "), disp(d_sum)
%%%%%%%%%%%%%%%%
% TO DO: Implement a local search loop that updates the location of the centroids
%  so that a new possible solution in the neighbourhood of the current one is
%  found. The solution is kept if it is *better* than the current one.
%  Better in this context means, the sum of distances from points in pts to
%  the nearest centroid (pizza restaurant location) is smaller.
%
%  Helpfully, I've given you a function distSum() to compute this distance for
%  you!
%%%%%%%%%%%%%%%%
disp ("local search--")
% count = 0;
% do
% [d_sum, dist, clust_id]=distSum(pts,cent); % total dist, dist for closest cent, cent_id
% m = [[1:ceil(length(pts(:,1)))]', dist, clust_id];
% for i = 1:K
%     cent_i = m(m(:,end)==i,:);
%     cent(i,:) = mean(pts(cent_i(:,1),:));
% end;


% count += 1;
count = 0;
do
[d_sum, dist, clust_id]=distSum(pts,cent); % total dist, dist for closest cent, cent_id
neighbor_cent = cent;
r = ceil(rand() * K);
d = ceil(rand()*4);
if(d == 1) %top
    neighbor_cent(r,2) = min(neighbor_cent(r,2) + 10, maxy);
end;
if(d == 2) %right
    neighbor_cent(r,1) = min(neighbor_cent(r,1) + 10, maxx);
end;
if(d == 3) %bottom
    neighbor_cent(r,2) = max(neighbor_cent(r,2) - 10, miny);
end;
if(d == 4) %left
    neighbor_cent(r,1) = max(neighbor_cent(r,1) - 10, minx);
end;
[new_sum, dist, clust_id]=distSum(pts,neighbor_cent); % total dist, dist for closest cent, cent_id

if(new_sum < d_sum)
    cent = neighbor_cent;
end;

count += 1;
until(count>100000)
[d_sum, dist, clust_id]=distSum(pts,cent);
disp('finnal dist sum'), disp(d_sum);
figure(2);clf;imagesc(likely_map);axis image;axis off;colormap(cols);title('Population Density in the GTA');hold on;
plot(pts(:,1),pts(:,2),'y.','markersize',4);
plot(cent(:,1),cent(:,2),'.','color',[0 0 0],'markersize',33);
plot(cent(:,1),cent(:,2),'.','color',[1 1 1],'markersize',30);
drawnow;


return;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% If you want to change the utility function, you'll need to write something
% similar to the function 'distSum()' shown below, but returning the
% corresponding value for your own utility function. Make sure you understand
% how the one below works!
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [d_sum, dist, clust_id]=distSum(pts,centroids)
%% Compute and return the distances from points in 'pts' to the
%% nearest centroid in 'centroids'. It also returns for each point
% the distance to the nearest centroid, and the centroid's index
% in the 'centroids' array

%% AND THIS HERE IS WHY WE USE MATLAB :)
di=sum(abs(repmat(pts,[1 1 size(centroids,1)])-repmat(reshape(centroids',[1 2 size(centroids,1)]),[size(pts,1) 1])),2);
[dist,clust_id]=min(di,[],3);
d_sum=sum(dist);

return;
