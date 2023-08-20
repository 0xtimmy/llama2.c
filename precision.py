import sys
import json
from matplotlib import pyplot as plt
import math
import torch
from statistics import mean 

if __name__ == "__main__":
    file = open("precision.json")
    data = json.loads(file.read())
    x_data = [x[0] for x in data]
    y_data = [x[1] for x in data]
    z_data = [x[2] for x in data]
    
    diff = []
    for i in range(len(x_data)):
        diff.append((y_data[i] - z_data[i]) / z_data[i])
        
    fig, (ax0, ax1) = plt.subplots(1, 2)
    ax0.scatter(x_data, z_data, color="b", label="target")
    ax0.scatter(x_data, y_data, color="r", label="real")
    ax0.scatter(x_data, torch.exp2(torch.tensor(x_data)), color="g", label="torch")
    ax1.hist(diff, bins=math.floor(math.sqrt(len(x_data))), label=f"average diff = {mean(diff)}")
    ax0.legend()
    plt.show()