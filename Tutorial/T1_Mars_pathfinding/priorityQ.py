class PriorityQ:
    def __init__(self):
        self.heap = []

    def insert(self, elmt):
        return

    def delete(self, elmt):
        return

    def getRoot(self):
        return self.heap[0]

    def extractRoot(self):
        return

    def constructFromList(self):
        return

    def heapify(self, lst, i):
        # find value at index i
        r = lst[i]
        lci = 2*i+1
        rci = 2*i+2
        print(i),
        print(lci),
        print(rci)
        # find value of left child of index i (2i+1)
        # find value of right child of index i (2i +2)
        # if smallest is not at i, swap smallest with i
        # and recursively call on swapped index untill it's the smalles among 3
        if(lci < len(lst) and lst[lci] < r):
            lc = lst[lci]
            print(lc)
            tmp = r
            lst[i] = lst[lci]
            lst[lci] = tmp
            print(lst)
            self.heapify(lst, lci)
        elif(rci < len(lst) and lst[rci] < r):
            rc = lst[rci]
            print(rc)
            tmp = r
            lst[i] = lst[rci]
            lst[rci] = tmp
            print(lst)
            self.heapify(lst, rci)
        return


if __name__ == '__main__':
    hp = PriorityQ()
    hp.heap = [4, 1, 5, 4, 3, 54, 6, 6, 7, 5, 8]
    hp.heapify(hp.heap, 0)
    print(hp.heap)
