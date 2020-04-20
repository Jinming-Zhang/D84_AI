class Node(object):
    """docstring for Node"""

    def __init__(self, name='', utility=None, alpha=None, beta=None):
        super(Node, self).__init__()
        self.children = []
        self.name = name
        self.utility = utility
        self.alpha = alpha
        self.beta = beta


def abp_minmaxSearch(node, agent):
    '''
    agent = -1, 1 (min, max)
    '''
    if node.utility is not None:
        return node.utility
    else:
        utils = []
        for i in range(0, len(node.children)):
            node.children[i].alpha = node.alpha
            node.children[i].beta = node.beta
            child_utility = abp_minmaxSearch(node.children[i], -agent)
            if(agent <= 0):  # min, update beta
                if(child_utility < node.beta):
                    node.beta = child_utility
            else:
                if(child_utility > node.alpha):
                    node.alpha = child_utility
            utils.append(child_utility)
            # ab pruning
            if(node.alpha >= node.beta):
                print('pruning nodes after {}'.format(node.children[i].name))
                break
        # print(utils)
        if(agent <= 0):  # min
            node.utility = min(utils)
        else:
            node.utility = max(utils)
        return node.utility


if __name__ == '__main__':
    # l0
    n0 = Node('n0', alpha=float('-inf'), beta=float('inf'))
    # l1
    n11 = Node('n11')
    n12 = Node('n12')
    n13 = Node('n13')
    n0.children += [n11, n12, n13]
    # l2
    n21 = Node('n21')
    n22 = Node('n22')
    n23 = Node('n23')
    n11.children += [n21, n22, n23]
    n24 = Node('n24')
    n25 = Node('n25')
    n26 = Node('n26')
    n12.children += [n24, n25, n26]
    n27 = Node('n27')
    n28 = Node('n28')
    n29 = Node('n29')
    n13.children += [n27, n28, n29]
    # terminal
    nt1 = Node('nt1', 13)
    nt2 = Node('nt2', -3)
    nt3 = Node('nt3', 6)
    n21.children += [nt1, nt2, nt3]
    nt4 = Node('nt4', -19)
    nt5 = Node('nt5', 6)
    nt6 = Node('nt6', -7)
    n22.children += [nt4, nt5, nt6]
    nt7 = Node('nt7', 7)
    nt8 = Node('nt8', -14)
    nt9 = Node('nt9', -10)
    n23.children += [nt7, nt8, nt9]
    nt10 = Node('nt10', 16)
    nt11 = Node('nt11', 7)
    nt12 = Node('nt12', 10)
    n24.children += [nt10, nt11, nt12]
    nt13 = Node('nt13', -4)
    nt14 = Node('nt4', 15)
    nt15 = Node('nt15', 18)
    n25.children += [nt13, nt14, nt15]
    nt16 = Node('nt16', 12)
    nt17 = Node('nt17', -14)
    nt18 = Node('nt18', -7)
    n26.children += [nt16, nt17, nt18]
    nt19 = Node('nt19', 15)
    nt20 = Node('nt20', -20)
    nt21 = Node('nt21', 7)
    n27.children += [nt19, nt20, nt21]
    nt22 = Node('nt22', 5)
    nt23 = Node('nt23', 0)
    nt24 = Node('nt24', 11)
    n28.children += [nt22, nt23, nt24]
    nt25 = Node('nt25', 8)
    nt26 = Node('nt26', 19)
    nt27 = Node('nt27', 0)
    n29.children += [nt25, nt26, nt27]

    res = abp_minmaxSearch(n0, 1)
    # track path
    tracknode = n0
    path = n0.name
    while(len(tracknode.children) > 0):
        for i in range(0, len(tracknode.children)):
            if(tracknode.children[i].utility == tracknode.utility):
                tracknode = tracknode.children[i]
                path += ' {}'.format(tracknode.name)
                break
    print(res)
    print(path)
