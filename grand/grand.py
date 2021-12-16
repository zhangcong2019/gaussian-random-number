import numpy as np
import random
import math


class Grand():
    def __init__(self, seed=0, n=15):
        self.seed = seed
        self.n = n
        random.seed(self.seed)
        self.a = np.zeros([self.n])

        self.a[0] = float(0)

        d = [0.674489750, 0.475859630, 0.383771164,
             0.328611323, 0.291142827, 0.263684322,
             0.242508452, 0.225567444, 0.211634166,
             0.199924267, 0.189910758, 0.181225181,
             0.173601400, 0.166841909, 0.160796729
             ]
        for i in range(1,self.n):
            self.a[i] = self.a[i-1] + d[i - 1]

    def generate(self) -> list:
        i = 0
        x = random.random()

        while x < 0.5 and i < self.n - 2:
            i += 1
            x *= 2

        while True:
            u1 = random.random()
            u = (self.a[i+1] - self.a[i]) * u1
            v = u * (u/2 + self.a[i])

            while True:
                u2 = random.random()
                if v < u2:
                    u3 = random.random()
                    if u3 < 0.5:
                        return [self.a[i] + u]
                    else:
                        return [-self.a[i] - u]
                else:
                    u4 = random.random()
                    v = u4
                u5 = random.random()
                if v < u5:
                    break


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
