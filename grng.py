from matplotlib import pyplot as plt
import numpy as np

# from boxMuller.boxMuller import boxMuller

from boxMuller.boxMuller import *

def plot(list_gauss):
    x = np.linspace(-4, 4, 800, endpoint=True)
    count = x * 0

    for num in list_gauss:
        if -4 < num < 4:
            index = int((num + 4) // 0.01)
            # print (index)
            # print (count)
            count[index] = count[index] + 1

    plt.plot(x, count)
    plt.show()


if "__main__" == __name__:
    print("this is grng")

    grng_ = BoxMuller(seed=99)

    list_gauss = []

    for i in range(10000):
        listTemp = grng_.generate()
        for g in listTemp:
            list_gauss.append(g)

    plot(list_gauss)
