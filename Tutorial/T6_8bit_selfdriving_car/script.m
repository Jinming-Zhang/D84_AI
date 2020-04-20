

function [] = script(s)
state = s
[i1,i2,i3,i4,i5]=indexesFromState(state)
r_state = i1 + ((i2+1)*5) + (i3*50) + (i4*250) + (i5*1250);
[r]=R(r_state,1,-1,-1)
end;

function [i1,i2,i3, i4, i5]=indexesFromState(s)
%% Returns the components i1 through i5 from the state index s

 i5 = floor(s/1250);
 i4 = floor((s-(i5*1250))/250);
 i3 = floor((s-(i5*1250)-(i4*250))/50);
 i2 = floor((s-(i5*1250)-(i4*250)-(i3*50))/5);
 i1 = s-(i5*1250)-(i4*250)-(i3*50)-(i2*5);

 
 % Check that the state is valid (the road in current and next row
 % must be within one block distance)
 if (abs(i4-i1)>1)
     i1=-1;
     i2=-1;
     i3=-1;
     i4=-1;
     i5=-1;
 end;
end;

function [r]=R(s1,Rg,Ro,Rj)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% TO DO:
%%
%% In class, we defined this as R(s,a) which is the
%% reward resulting from taking action a at state s.
%%
%% This is equivalent to evaluating R() at s', the
%% state that results from action a and initial state s.
%% So this function takes only as input s' (here called
%% s1) and the rewards attached to various events.
%%
%% As a reminder, 
%%  Rg is the reward for staying on the road,
%%  Ro is the reward for going off-road,
%%  Rj is the reward for hitting junk
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  [i1,i2,i3, i4, i5]=indexesFromState(s1);

  if (i1 == -1 || i2 == -1 || i3 == -1 || i4 == -1 || i5 == -1) % invalid state
    r = 0;
    fflush(stdout);
    disp("you should not be here!!");
  elseif (i2 < (i4+1)) || (i2 > ((i4+1)+3)) % off road
    r = Ro;
  elseif i5 == 0 % no junk
    r = Rg;
  else % has junk
    if (i5-1+(i4+1)) == i2 % on junk
      r = Rj;
    else
      r = Rg; % not on junk
    end;
  end;
end;