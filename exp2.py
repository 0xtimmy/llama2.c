import torch

def exp2(x):
    x = (torch.pow(2, torch.floor(x)) * (1 + (x - torch.floor(x))))
    sum = torch.sum(x)
    return x / sum

avg_dev = 0

for i in range(100):
    x = torch.randn(100)
    y0 = exp2(x)
    y1 = torch.exp2(x)
    avg_dev += torch.mean((y0 - y1) / y1)
    
print("average deviation: ", avg_dev/100)