class Node:
    def __init__(self, name, hn):
        self.name = name
        self.neighbours = []
        self.visited = False
        self.predesessor = None
        self.dist_to_goal = 9999
        self.hn_dist_to_goal = hn
        self.fn = 99999

    def append_neighbor(self, neighbor, weight):
        self.neighbours.append([neighbor, weight])


def sortByWeight(edgeQ):
    if(len(edgeQ) <= 1):
        return edgeQ
    else:
        leftQ = edgeQ[0: len(edgeQ)//2]
        rightQ = edgeQ[len(edgeQ)//2: len(edgeQ)]
        leftQ = sortByWeight(leftQ)
        rightQ = sortByWeight(rightQ)

        left_i = 0
        right_i = 0
        res = []
        while(left_i < len(leftQ) or right_i < len(rightQ)):
            if(left_i >= len(leftQ)):
                res += rightQ[right_i: len(rightQ)]
                break
            elif(right_i >= len(rightQ)):
                res += leftQ[left_i: len(leftQ)]
                break
            elif(leftQ[left_i].fn < rightQ[right_i].fn):
                res.append(leftQ[left_i])
                left_i += 1
            else:
                res.append(rightQ[right_i])
                right_i += 1
        return res


def Astar(goalsname, priQ):
    ''' priQ: Q of nodes
    '''
    if(len(priQ) <= 0):
        return None
    else:
        cur_node = priQ[0]
        if(cur_node.visited is False):
            print('expending: ' + cur_node.name)
            cur_node.visited = True
            if(cur_node.name in goalsname):
                # reached goal, print path
                print('cost to goal: {}'.format(cur_node.dist_to_goal))
                path = []
                path.append(cur_node)
                while(cur_node.predesessor is not None):
                    path.append(cur_node.predesessor)
                    cur_node = cur_node.predesessor
                path.reverse()
                for n in path:
                    print(n.name + "->"),
                return
            else:
                for edge in cur_node.neighbours:
                    neighbor = edge[0]
                    weight = edge[1]
                    hn = neighbor.hn_dist_to_goal
                    # print(str(neighbor.dist_to_goal)+' '),
                    # print(str(cur_node.dist_to_goal)+' '),
                    # print(hn)
                    if(neighbor.dist_to_goal > cur_node.dist_to_goal + hn):
                        neighbor.dist_to_goal = cur_node.dist_to_goal + weight
                        neighbor.fn = cur_node.dist_to_goal + weight + hn
                        neighbor.predesessor = cur_node
                        priQ.append(neighbor)
                priQ = priQ[1:len(priQ)]
                priQ = sortByWeight(priQ)
                for ww in priQ:
                    print('({} {})'.format(ww.name, ww.fn)),
                print('')
                return Astar(goalsname, priQ)
        else:
            return Astar(goalsname, priQ[1:len(priQ)])


if __name__ == "__main__":
    na = Node('a', 5)
    nb = Node('b', 4)
    nc = Node('c', 4)
    nd = Node('d', 2)
    ne = Node('e', 2)
    nf = Node('f', 2)
    ng = Node('g', 2)
    nh = Node('h', 0)
    ni = Node('i', 3)
    nj = Node('j', 0)
    nk = Node('k', 4)
    nl = Node('l', 3)
    nm = Node('m', 0)
    nn = Node('n', 0)
    no = Node('o', 2)
    np = Node('p', 0)
    nq = Node('q', 0)
    nr = Node('r', 0)
    ns = Node('s', 1)
    nt = Node('t', 0)
    nu = Node('u', 0)

    na.append_neighbor(nb, 2)
    na.append_neighbor(nc, 1)

    nb.append_neighbor(ne, 1)
    nb.append_neighbor(ng, 3)
    nb.append_neighbor(nd, 1)

    nc.append_neighbor(nd, 4)
    nc.append_neighbor(nf, 3)
    nc.append_neighbor(ni, 1)

    nd.append_neighbor(ng, 2)
    nd.append_neighbor(nh, 2)

    ne.append_neighbor(nm, 3)
    ne.append_neighbor(nj, 3)
    ne.append_neighbor(np, 5)

    nf.append_neighbor(nh, 2)
    nf.append_neighbor(nl, 3)
    nf.append_neighbor(ni, 2)

    ng.append_neighbor(nk, 2)
    ng.append_neighbor(nh, 2)

    nh.append_neighbor(nn, 2)

    ni.append_neighbor(no, 1)
    ni.append_neighbor(nq, 4)

    nj.append_neighbor(nr, 2)

    nk.append_neighbor(np, 3)

    nl.append_neighbor(no, 2)
    nl.append_neighbor(nq, 3)

    nm.append_neighbor(nr, 2)

    # nn no neighbor

    no.append_neighbor(ns, 1)

    np.append_neighbor(nr, 4)

    nq.append_neighbor(nt, 2)
    nq.append_neighbor(nu, 3)

    # nr no neighbor

    ns.append_neighbor(nt, 1)

    nt.append_neighbor(nu, 1)

    # nu no neighbor
    na.dist_to_goal = 0
    # Astar(['z'], [)
    Astar(['r', 'n', 'u'], [na])
    print("end")
