/*=====================================================================================
 *	            Copyleft! 2017 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *              Copyleft! 2017 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *				
 * Filename                : 
 * Description             : 双目匹配获得深度图片(编译环境：OpenCV3.3.1+Ubuntu16.04)
 * Reference               : 重点参考：http://blog.csdn.net/wangyaninglm/article/details/41181731
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2017.05.16, William Yu
                            
=====================================================================================*/


# 1.Compile this program:

g++ stereo_match.cpp -o stereo_match `pkg-config  --cflags --libs opencv`



# 2.Usage:
./stereo_match aloeL.jpg aloeR.jpg --algorithm=bm --blocksize=5 --max-disparity=256  --scale=1.0 --no-display -o aloe.png

这一条效果比较好
 ./stereo_match aloeL.jpg aloeR.jpg --algorithm=bm --blocksize=35 --max-disparity=256  --scale=1.0
