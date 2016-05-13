**离线签名识别 Off-Line Signature Recognition**

任务描述：


	给定了5组各75张离线样本图像，然后给定新的测试图像，要求将测试图像正确分类。

这是《模式识别》课程的一项大作业。

这个问题被简化了很多，与其说是离线签名识别（Recognition），不如说是分类（Classification）
更加贴切。

给定的样本在*IMAGE*文件夹中。样本总体来说没有明显的旋转和放缩，有比较明显的位移。所以可以通过预处理将签名归一化到中心。旋转和放缩可以忽略。

这个问题就是特征点和分类器的选择问题。

考虑特征点。从整体上考虑，边缘或者灰度。从细节上考虑，SIFT算子等。当然还有其他的方法，这里我仅是简单考虑了。

考虑分类器。因为样本数比较少，又是OCR问题，维度不会低。那么SVM无疑是一个非常好的选择。当然BP神经网络也是备选方案。

最后我选择的结果是HOG + SVM和SIFT两种方法。

个人觉得HOG方法本身不一定很好，但是对于这个简单分类的任务足够了。

SIFT相对更好一些，可以识别出伪造签名。测试集为 *Testset_4NsigCamp 2012*.可以从网上下载。

