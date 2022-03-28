from matplotlib import pyplot as plt
import numpy as np

from boxMuller.boxMuller import *
from grand.grand import *
from wallace.wallace import *
from ziggurat.ziggurat import *

def plot(list_gauss):
    x = np.linspace(-4, 4, 800, endpoint=True)
    count = x * 0

    for num in list_gauss:
        # print(num)
        if -4 < num < 4:
            index = int((num + 4) // 0.01)
            # print (index)
            # print (count)
            count[index] = count[index] + 1

    plt.plot(x, count)
    plt.show()


if "__main__" == __name__:
    print("this is grng")


    in_file = open('random.bin', 'rb')

    num = 3000000

    # list_gauss = np.fromfile(in_file, dtype = float)
    import struct
    list_gauss = struct.unpack('f'*num, in_file.read(4*num))

    plot(list_gauss)
