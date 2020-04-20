# Let us try to move a set of N tigers and M people across a river
from random import *    # I apologize, but not too much
import sys
import time
# print sys.getrecursionlimit()
# sys.setrecursionlimit(5000)


class chalupa:
    """
    This is a snall boat - typically found floating in Xochimilco
    It can also be a tasty form of street food. Take your pick.
    """

    def __init__(self):
        self.side = 0   # Boat always starts on the left river bank
        self.contents = list()  # and is empty


def initialize_game(N, M, Left):
    """
    Sets up the initial creatures list
    All creatures always start on the left river bank
    """
    for i in range(N):
        Left.append('T')
    for i in range(M):
        Left.append('P')
    print "Initial creatures list: ", Left_Side


def reset_game(Left, Right, Boat, Backup):
    """
    Reset lists to initial configuration - needed during search for command sequence
    evaluation
    """
    while len(Left) > 0:
        Left.pop()
    for i in range(len(Backup)):
        Left.append(Backup[i])
    while len(Right) > 0:
        Right.pop()
    while len(Boat.contents) > 0:
        Boat.contents.pop()
    Boat.side = 0


def load_type(t, Left, Right, Boat):
    """
    Loads an entity of type t in ['T' 'P'] onto the boat. If there
    is no such entity left on the corresponding side of the river
    it does nothing.
    Assumes there is space in the boat for at least one item, else
    does nothing.
    """

    if len(Boat.contents) > 1:
        return 0

    if Boat.side == 0:
        Active_List = Left
    else:
        Active_List = Right

    try:
        x = Active_List.index(t)
        Boat.contents.append(Active_List.pop(x))
        return 1
    except ValueError:
        # None left on this side
        return 0


def unload_type(t, Left, Right, Boat):
    """
    Unload a creature of the specified type onto the current side
    of the river
    If no such creature is on the boat does nothing
    """

    if len(Boat.contents) == 0:
        return 0

    if Boat.side == 0:
        Active_List = Left
    else:
        Active_List = Right

    try:
        x = Boat.contents.index(t)
        Active_List.append(Boat.contents.pop(x))
        return 1
    except ValueError:
        # No such creature left
        return 0


def swap_type(t, Left, Right, Boat):
    """
    Swap one creature between the boat and the riverside. In this case 't' is the type
    of creature on the Boat that will be swapped out.
    """

    if len(Boat.contents) == 0:
        return 0

    if t == 'P':
        tt = 'T'
    else:
        tt = 'P'

    try:
        x = Boat.contents.index(t)
    except ValueError:
        # No such creature left on Boat
        return 0

    if Boat.side == 0:
        Active_List = Left
    else:
        Active_List = Right

    try:
        y = Active_List.index(tt)
    except ValueError:
        # No such creature left on river bank
        return 0

    # Swap!
    Active_List[y] = t
    Boat.contents[x] = tt
    return 1


def cross(Boat):
    """
    Takes the Boat across the river
    Boat must have at least one person (Tigers can't drive boats, ask Pi)
    """

    try:
        x = Boat.contents.index('P')
        if (Boat.side == 0):
            Boat.side = 1
        else:
            Boat.side = 0
        return 1
    except ValueError:
        # No people in the boat
        return 0


def numP(where):
    """
    Return the number of people within the list 'where'
    """
    pp = 0
    for i in range(len(where)):
        if where[i] == 'P':
            pp = pp+1

    return pp


def numT(where):
    """
    Return the number of tigers within the list 'where'
    """
    tt = 0
    for i in range(len(where)):
        if where[i] == 'T':
            tt = tt+1

    return tt


def check_state(Left, Right, Boat):
    """
    Verifies the current configuration to ensure no people are
    being lunched on.
    Returns: -1 if the current config. causes people to be eaten
             0 if the current config is safe
             1 if the current config is a solution
    """

    if numT(Left) > numP(Left) and numP(Left) > 0:
        return -1

    if numT(Right) > numP(Right) and numP(Right) > 0:
        return -1

    if numT(Right)-numP(Right) > 2:   # Unsolvable!
        return -1

    if len(Left) == 0 and len(Boat.contents) == 0:
        print "Found a solution!"
        print Left, "****", Boat.contents, "****", Right
        return 1
    else:
        return 0


def print_plan(some_plan):
    if(len(some_plan) <= 0):
        print(some_plan)
        return
    for i in range(0, len(some_plan)):
        if(some_plan[i][0] == load_type):
            print('load {}'.format(some_plan[i][1])),
        if(some_plan[i][0] == unload_type):
            print('unload_type {}'.format(some_plan[i][1])),
        if(some_plan[i][0] == swap_type):
            print('swap_type {}'.format(some_plan[i][1])),
        if(some_plan[i][0] == cross):
            print('crossing'),
        if(i < len(some_plan) - 1):
            print('->'),
    print('')


def SearchForPlan(some_plan, Left, Right, Boat, Backup):
    """
    Generate a sequence of program actions to solve the transportation problem
    Starts with an empty plan, progressively adds commands from the following
    choices:
            * load_type(t)     (append to plan [load_type, t])
            * cross()    (append to plan [cross, []])
            * unload_type(t)   (append to plan [unload_type, t])
            * swap_type(t)     (append to plan [swap, t])

    In the commands above, t can be either 'T' or 'P' and your search code can
    add commands in any order.

    Once you have created a new plan, execute the plan by calling RunPlan(plan)

    You will receive a return value of:
            -1  if your plan leads to a configuration where people are eaten.
                In this case you must backtrack (up to you how much) and
                keep searching
            0   The current plan results in a safe configuration, you can keep
                adding commands to it
            1   A solution has been found! you're done
    """

    ##############################################################################
    # TO DO:
    # Add your search code below. Mind the following constraints:
    # You are not to use graph search to come up with your plan (i.e. you
    # are not allowed to solve the problem in order to solve the problem!)
    # You are not allowed to call commands directly. All actions must be
    # part of the plan you are searching for
    # You are not allowed to hardcode the solution for a given N and M
    # (yes, I know you can solve the problem in your head. So can I. We
    # are studying how non-deterministic-programming works. So work
    # with it and learn!)
    # You are not allowed to change or update the contents of:
    # Left_Side
    # Right_Side
    # Boat
    # creatures
    # tries
    # The simplest plan generation method will find a solution - given enough
    # time. But you should aim for a smarter search process that does not
    # require a ridiculous amount of search.
    ##
    # You can choose to use recursion, or not. As you please. But explain your
    # choice in the report.
    ###############################################################################

    # print "DEBUG: ***** "
    # Run input plan
    print "Current plan:"
    print_plan(some_plan)
    print "Backup plan:"
    print_plan(Backup)
    x = RunPlan(some_plan, Left, Right, Boat, Backup)
    while(x != 1):
        time.sleep(1)
        if x == 0:  # current plan is ok, not a solution yet
            print(' Enpanding plan...')
            # keep going
            for nextplan in some_plan:
                Backup.append(nextplan)
            # check for possible actions
            possible_actions = list()
            possible_actions.append([[cross, []]])
            possible_actions.append([[load_type, 'T'], [cross, []]])
            possible_actions.append([[load_type, 'P'], [cross, []]])
            possible_actions.append([[unload_type, 'T'], [cross, []]])
            possible_actions.append([[unload_type, 'P'], [cross, []]])
            possible_actions.append([[swap_type, 'T'], [cross, []]])
            possible_actions.append([[swap_type, 'P'], [cross, []]])
            # select possible actions randomly
            dice = (int)(random() * len(possible_actions))
            print(dice)
            for nextplan in possible_actions[dice]:
                some_plan.append(nextplan)

            print "     Expanded plan:"
            print_plan(some_plan)
            print "     Backup plan:"
            print_plan(Backup)

            x = RunPlan(some_plan, Left, Right, Boat, Backup)
        else:  # current plan is bad, return to the previous plan
            print(' Revoking bad plan...')
            print_plan(some_plan)
            del some_plan[:]
            for nextplan in Backup:
                some_plan.append(Backup)
            print('     Revoed plan:')
            print_plan(some_plan)
            x = RunPlan(some_plan, Left, Right, Boat, Backup)
            # # check for possible actions
            # possible_actions = list()
            # possible_actions.append([cross, []])
            # possible_actions.append([load_type, 'T'])
            # possible_actions.append([load_type, 'P'])
            # possible_actions.append([unload_type, 'T'])
            # possible_actions.append([unload_type, 'P'])
            # possible_actions.append([swap_type, 'T'])
            # possible_actions.append([swap_type, 'P'])
            # # select possible actions randomly
            # dice = (int)(random() * len(possible_actions))
            # print(dice)
            # some_plan.append(possible_actions[dice])
            # x = RunPlan(some_plan, Left, Right, Boat, Backup)


def RunPlan(some_plan, Left, Right, Boat, Backup):
    """
    Execute the current command sequence - on a clean game state

    If the command sequence does not cause an unsafe configuration, this function will
    return:
            0 -   If the game state is not a solution
            1 -   If the game state is a solution
    Otherwise the function returns -1
    """
    global tries

    reset_game(Left, Right, Boat, Backup)

    tries = tries+1

    if len(some_plan) == 0:   # Initial empty plan
        return 0

    for i in range(len(some_plan)):
        if (some_plan[i][0] == cross):
            y = some_plan[i][0](Boat)
        else:
            y = some_plan[i][0](some_plan[i][1], Left, Right, Boat)
        x = check_state(Left, Right, Boat)
        if x == -1 or y == 0:
            return -1  # Results in an unsafe configuration

        if x == 1:
            print "Solution found!, total tries=", tries, " plan length=", len(some_plan)
            return 1
    return 0


if __name__ == "__main__":

    Left_Side = list()
    Right_Side = list()
    Left_Backup = list()
    tries = 0
    Boat = chalupa()
    population_limit = (int)(random() * 1 // 1)
    initialize_game(1, 1, Left_Side)
    Left_Backup = list()

    plan = list()  # Here! I'm giving you an empty plan! Am I Not Merciful?
    # (if you don't get that, you haven't seen Gladiator)

    SearchForPlan(plan, Left_Side, Right_Side, Boat, Left_Backup)
    reset_game(Left_Side, Right_Side, Boat, Left_Backup)

    # Execute plan and show game progress

    print "Left Side    Boat      Right Side"
    for i in range(len(plan)):
        if plan[i][0] == cross:
            print "Crossing!"
            plan[i][0](Boat)
        else:
            plan[i][0](plan[i][1], Left_Side, Right_Side, Boat)
        if Boat.side == 0:
            print Left_Side, "  ", Boat.contents, "     ", Right_Side
        else:
            print Left_Side, "      ", Boat.contents, " ", Right_Side

    print "Done!"

 # if(Boat.side == 0):  # if boat on left
    #     if('P' in Boat.contents):  # if a person is on the boat
    #         if('T' in Left):  # load tiger and go
    #             possible_actions.append([[load_type, 'T'], [cross, []]])
    #         if('P' in Left):  # load another person and go
    #             possible_actions.append([[load_type, 'P'], [cross, []]])
    #     else:  # if no person is on the boat (at the beginning)
    #         if('P' in Left and 'T' in Left):  # load a person and a tiger then go
    #             possible_actions.append(
    #                 [[load_type, 'P'], [load_type, 'T'], [cross, []]])
    #         elif(Left.count('P') >= 2):  # load 2 people and go
    #             possible_actions.append(
    #                 [[load_type, 'P'], [load_type, 'P'], [cross, []]])
    #         elif('P' in Left):  # load 1 person and go
    #             possible_actions.append([[load_type, 'P'], [corss, []]])

    # if(Boat.side == 1):  # if boat on the right
    #     if(Boat.contents.count('P') == 2):  # boat has 1 person
    #         possible_actions.append([[unload_type, 'P'], [unload_type, 'P']])
    #         possible_actions.append([[unload_type, 'P'], [cross, []]])
    #     if('P' in Boat.contents and 'T' in Boat.contents):
    #         possible_actions.append([[unload_type, 'T'], [unload_type, 'P']])
    #         possible_actions.append([[swap_type, 'T'], [cross, []]])
    #         possible_actions.append([[unload_type, 'T'], [cross, []]])

    #     if(len(Left) > 0):
    #         possible_actions.append([swap_type, 'P'])

