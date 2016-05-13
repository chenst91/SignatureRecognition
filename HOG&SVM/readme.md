#签名识别:HOG & SVM
---

**环境：** *VS2015 + OpenCV3.10*

##文件说明：

###1.MakeList.cpp

 - 生成文件列表。

###2.Preprocess.cpp

 - 预处理，包含阈值化和归一化。

 - 阈值化：采用简单阈值方法，因为其他阈值方法很难处理好采集图像边缘。

 - 归一化：将签名平移至图像中心，忽略签名的放缩和旋转。

###3.HOG.cpp

 - 提取HOG特征。

> **注：**
> 
> 比较重要的是HOG特征的维数，我使用了28800维，维数低导致不精确，维数过多导致过拟合。
> 
> 参考文章：[关于HOG维度的计算](http://gz-ricky.blogbus.com/logs/85326280.html)。

###4.SVM.cpp

 - 训练
 - 预测

> **注：**
> 
> 核函数：LINEAR线性核
>
> 一般认为RBF(径向基，高斯函数)稳定性最好，libSVM作者认为LINEAR最好。
>
> 因为仅是简单的分类问题，所以LINEAR核完全可以满足要求。
>
> 另外需要注意的是OpenCV3.X和OpenCV2.X版本的SVM写法发生了变化。


**该方法的正确率达到了100%**


