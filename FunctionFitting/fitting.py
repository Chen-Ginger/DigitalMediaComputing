# @Start date : 2021/12/5 

# @Last update date : 2021/12/5 

# @Author : jc

# @File : fitting

# @Software: PyCharm

from torch.utils.data import DataLoader
from torch.utils.data import TensorDataset
import torch.nn as nn
import numpy as np
import torch

# 准备数据
x=np.linspace(-8*np.pi,8*np.pi,800)
y=np.sin(x)+np.cos(x)
# 将数据做成数据集的模样
X=np.expand_dims(x,axis=1)
Y=y.reshape(800,-1)
# 使用批训练方式
dataset=TensorDataset(torch.tensor(X,dtype=torch.float),torch.tensor(Y,dtype=torch.float))
dataloader=DataLoader(dataset,batch_size=100,shuffle=True)

# 神经网络主要结构，这里就是一个简单的线性结构

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.net=nn.Sequential(
            nn.Linear(in_features=1,out_features=10),nn.ReLU(),
            nn.Linear(10,100),nn.ReLU(),
            nn.Linear(100,10),nn.ReLU(),
            nn.Linear(10,1)
        )

    def forward(self, input:torch.FloatTensor):
        return self.net(input)

net=Net()

# 定义优化器和损失函数
optim=torch.optim.Adam(Net.parameters(net),lr=0.001)
Loss=nn.MSELoss()


# 下面开始训练：
# 一共训练 1000次
for epoch in range(1000):
    loss=None
    for batch_x,batch_y in dataloader:
        y_predict=net(batch_x)
        loss=Loss(y_predict,batch_y)
        optim.zero_grad()
        loss.backward()
        optim.step()
    # 每100次 的时候打印一次日志
    if (epoch+1)%100==0:
        print("step: {0} , loss: {1}".format(epoch+1,loss.item()))


# 使用训练好的模型进行预测
predict=net(torch.tensor(X,dtype=torch.float))
x2 = np.linspace(-8*np.pi,8*np.pi,800)
X2 = np.expand_dims(x2,axis=1)
predict2=net(torch.tensor(X2,dtype=torch.float))
y2 = np.sin(x2) + np.cos(x2)
# 绘图展示预测的和真实数据之间的差异
import matplotlib.pyplot as plt
plt.plot(x2,y2,label="fact")
plt.plot(x2,predict2.detach().numpy(),label="predict")
plt.title("sinx + cosx")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.savefig(fname="result.png",figsize=[10,10])
plt.show()
