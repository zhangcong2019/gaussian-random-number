import numpy
import random
import math


class Grand():
    def __init__(self, seed=0):
        self.seed = seed
        random.seed(self.seed)

    def generate(self) -> list:

        u1 = random.random()
        u2 = random.random()

        a = math.sqrt(-2 * math.log(u1))
        b = 2 * math.pi * u2

        r1 = a * math.sin(b)
        r2 = a * math.cos(b)

        r = [r1, r2]

        return r


if "__main__" == __name__:
    print("this is grand grng")

    grng_ = Grand(seed=99)

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
