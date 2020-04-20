class Node:
    def __init__(self, name):
        self.name = name
        self.neighbours = []
        self.visited = False
        self.predesessor = None
        self.dist_to_goal = 9999

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
            elif(leftQ[left_i].dist_to_goal < rightQ[right_i].dist_to_goal):
                res.append(leftQ[left_i])
                left_i += 1
            else:
                res.append(rightQ[right_i])
                right_i += 1
        return res
        # while(left_i < len(leftQ) or right_i < len(rightQ)):
        #     if(left_i >= len(leftQ)):
        #         res += rightQ[right_i: len(rightQ)]
        #         break
        #     elif(right_i >= len(rightQ)):
        #         res += leftQ[left_i: len(leftQ)]
        #         break
        #     elif(leftQ[left_i][1] < rightQ[right_i][1]):
        #         res.append(leftQ[left_i])
        #         left_i += 1
        #     else:
        #         res.append(rightQ[right_i])
        #         right_i += 1
        # return res


def ucs(goalsname, priQ):
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
                    if(neighbor.dist_to_goal > cur_node.dist_to_goal + weight):
                        neighbor.dist_to_goal = cur_node.dist_to_goal + weight
                        neighbor.predesessor = cur_node
                        priQ.append(neighbor)
                priQ = priQ[1:len(priQ)]
                priQ = sortByWeight(priQ)

                for ww in priQ:
                    print('({} {})'.format(ww.name, ww.dist_to_goal)),
                print('')
                return ucs(goalsname, priQ)
        else:
            return ucs(goalsname, priQ[1:len(priQ)])


if __name__ == "__main__":
    na = Node('a')
    nb = Node('b')
    nc = Node('c')
    nd = Node('d')
    ne = Node('e')
    nf = Node('f')
    ng = Node('g')
    nh = Node('h')
    ni = Node('i')
    nj = Node('j')
    nk = Node('k')
    nl = Node('l')
    nm = Node('m')
    nn = Node('n')
    no = Node('o')
    np = Node('p')
    nq = Node('q')
    nr = Node('r')
    ns = Node('s')
    nt = Node('t')
    nu = Node('u')

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
    # ucs(['z'], [na])
    ucs(['r', 'n', 'u'], [na])
    print("end")
