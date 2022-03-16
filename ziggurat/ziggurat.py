import random
import math
import numpy as np


def phi(x):
    return math.exp(-x * x / 2)


def phiInv(x):
    return math.sqrt(-2 * math.log(x))


class Ziggurat():
    def __init__(self, seed=0):
        self.seed = seed
        random.seed(self.seed)

        self.N = 256

        self.r = 3.6541528853610088
        self.v = 0.00492867323399

        self.x = np.zeros([self.N])

        self.f = np.zeros([self.N])

        self.x[self.N - 1] = self.r
        self.f[self.N - 1] = phi(self.x[self.N - 1])
        print("x %d is %.2f" % (self.N - 1, self.x[self.N - 1]))

        for i in range(self.N - 2, 0, -1):
            self.x[i] = phiInv(self.v / self.x[i + 1] + self.f[i + 1])
            self.f[i] = phi(self.x[i])
            print("x %d is %.2f" % (i, self.x[i]))

        self.x[0] = 0.0
        self.f[0] = phi(self.x[0])
        print("x %d is %.2f" % (0, self.x[0]))

    def generate(self) -> list:
        while True:
            u1 = random.random()
            u2 = random.random() * 2 - 1

            i = 1 + math.floor((self.N - 1) * u1)
            x = self.x[i] * u2
            if math.fabs(x) < self.x[i - 1]:
                return [x]
            # else:
            #     # if i != self.N - 1:
            #     #     y = (self.f[i - 1] - self.f[i]) * random.random()
            #     #     if y < (phi(x) - self.f[i]):
            #     #         return [x]

            #     if i != self.N - 1:
            #         y = random.random()
            #         if y > ((x) - self.x[i-1]):
            #             return [x]





if "__main__" == __name__:
    print("this is ziggurat grng")

    grng_ = Ziggurat(seed=99)

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
