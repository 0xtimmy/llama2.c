import torch
import torch.nn.functional as F
import matplotlib.pyplot as plt

def softmax(x, dim):
    #return F.softmax(x)
    x = x.exp()
    sum = x.sum(dim, keepdim=True)
    return x / sum

t = torch.randn(4, 4, 4)

expected = F.softmax(t, dim=-1)
actual = softmax(t, dim=-1)

plt.scatter(t, expected, color="b", marker="x")
plt.scatter(t, actual, color="r", marker="+")
plt.show()