#签名识别:SIFT or ORB
----

环境： *VS2015 + OpenCV2.4.12*



**注：**

1.使用OpenCV2.X的原因：

> 因为在OpenCV3.X版本，移除了一些专利性算法，如SIFT,SURF等。
 
> 相关代码可以从[opencv_contrib](https://github.com/Itseez/opencv_contrib)下载。之后 利用[CMake](https://cmake.org/download/)编译即可。

> 然而，我和周围的小伙伴并不会使用CMake，在编译过程中一直出现问题，查找了相关资料之后仍然没有解决。

> 因而使用了OpenCV2.4.12。

2.代码未完成的原因：

> OpenCV2.X并没有完美支持VS2015，可能存在个别BUG。

> 一旦使用了 detect() 或者 compute() 函数，在程序生命周期结束后就会报"Ptr强制类型转换错误"，错误文件为"xmemory0.cpp"

> 经过了很多次尝试最终发现这个BUG应该是OpenCV2.X在VS2015上的BUG。

> 因而这份代码并不是完整的。KNN分类器的部分还没有实现。

3.解决方法

> 在OpenCV3.X和VS2015平台上，使用[ORB](http://blog.csdn.net/sunanger_wang/article/details/7949202)特征点替代SIFT特征点。




