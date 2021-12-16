import numpy as np
import random
import math

class Wallace():
    def __init__(self, seed=0):
        self.seed = seed
        random.seed(self.seed)

        self.R = 2
        self.L = 1024
        self.K = 4
        self.N = self.L * self.K

        m = [-1, 1, 1, 1,
             1, -1, 1, 1,
             -1, -1, 1, -1,
             -1, -1, -1, 1]

        self.A = [np.zeros([self.K, self.K]), np.zeros([self.K, self.K])]

        for i in range(self.K):
            for j in range(self.K):
                self.A[0][i][j] = m[i * self.K + j]
        self.A[0] *= 0.5
        self.A[1] = -self.A[0]

        self.pool = np.random.randn(self.K * self.L)

        self.shuffle = [i for i in range(self.N)]

        for i in range(self.N - 1):
            j = random.randint(i, self.N - 1)
            [self.shuffle[i], self.shuffle[j]] = [self.shuffle[j], self.shuffle[i]]


    def generate(self) -> list:
        for i in range(self.N - 1):
            j = random.randint(i, self.N - 1)
            [self.shuffle[i], self.shuffle[j]] = [self.shuffle[j], self.shuffle[i]]
        for i in range(self.R):
            for j in range(self.L):
                pos = self.shuffle[(j * self.K) : (j * self.K) + self.K]
                x = self.pool[pos]
                x = np.reshape(x,[4,1])
                x_ = np.matmul(self.A[i], x)
                for z in range(self.K):
                    self.pool[pos[z]] = x_[z]


        # s = math.sqrt(math.fabs(self.pool[self.N -1]))
        return self.pool[:-1]


if "__main__" == __name__:
    print("this is wallace grng")

    grng_ = Wallace(seed=99)

    list_gauss = []

    for i in range(10000):
        listTemp = grng_.generate()
        for g in listTemp:
            list_gauss.append(g)

    import os, sys

    scriptPath = os.path.realpath(os.path.dirname(sys.argv[0]))
    sys.path.append(scriptPath + "/..")

    import grng

    grng.plot(list_gauss)
