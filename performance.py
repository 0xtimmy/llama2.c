import sys
import json
from matplotlib import pyplot as plt
import math

if __name__ == "__main__":
    file = open("performance.json")
    data = json.loads(file.read())
    exp_data = [x[0] for x in data]
    exp2_data = [x[1] for x in data]
    bins = math.floor(math.sqrt(len(exp_data)))
    plt.hist(exp_data, color="b", bins=bins, label="exp")
    plt.hist(exp2_data, color="r", bins=bins, label="exp2")
    plt.xlabel("ms")
    plt.legend()
    plt.show()