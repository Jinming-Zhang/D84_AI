class Node:
    def __init__(self, name):
        self.name = name
        self.neighbours = []
        self.visited = False
        self.predesessor = None
        self.dist_to_goal = 9999

    def append_neighbor(self, neighbor):
        self.neighbours.append(neighbor)


def bfs(queue, goalsname):
    if(len(queue) <= 0):
        return None
    else:
        node = queue[0]
        if(node.name in goalsname):
            # reached goal, print path
            path = []
            path.append(node)
            while(node.predesessor is not None):
                path.append(node.predesessor)
                node = node.predesessor
            path.reverse()
            for n in path:
                print(n.name + "->"),
            return
        else:
            queue = queue[1:len(queue)]
            if(node.visited is True):
                return bfs(queue, goalsname)
            else:
                node.visited = True
                print('visited node {}'.format(node.name))
                for neighbor in node.neighbours:
                    if(neighbor.visited is False and neighbor not in queue):
                        queue.append(neighbor)
                        neighbor.predesessor = node
                for n in queue:
                    print(n.name+" "),
                print(' ')
                return bfs(queue, goalsname)


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

    na.append_neighbor(nb)
    na.append_neighbor(nc)

    nb.append_neighbor(ne)
    nb.append_neighbor(ng)
    nb.append_neighbor(nd)

    nc.append_neighbor(nd)
    nc.append_neighbor(nf)
    nc.append_neighbor(ni)

    nd.append_neighbor(ng)
    nd.append_neighbor(nh)

    ne.append_neighbor(nm)
    ne.append_neighbor(nj)
    ne.append_neighbor(np)

    nf.append_neighbor(nh)
    nf.append_neighbor(nl)
    nf.append_neighbor(ni)

    ng.append_neighbor(nk)
    ng.append_neighbor(nh)

    nh.append_neighbor(nn)

    ni.append_neighbor(no)
    ni.append_neighbor(nq)

    nj.append_neighbor(nr)

    nk.append_neighbor(np)

    nl.append_neighbor(no)
    nl.append_neighbor(nq)

    nm.append_neighbor(nr)

    # nn no neighbor

    no.append_neighbor(ns)

    np.append_neighbor(nr)

    nq.append_neighbor(nt)
    nq.append_neighbor(nu)

    # nr no neighbor

    ns.append_neighbor(nt)

    nt.append_neighbor(nu)

    bfs([na], ['r', 'n', 'u'])
