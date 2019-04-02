/*=================================================================================
 *                      Copyleft! 2019 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2019 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : theme.h
 * Description             : 
 * Reference               : https://color.adobe.com/explore/?filter=most-popular&time=month
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.03.31, William Yu
                             
=================================================================================*/
#ifndef THEME_H
#define THEME_H


#include <iostream>
#include <opencv2/opencv.hpp>

struct Theme{
    std::string title;
	std::string x_title;
	std::string y_title;
    bool grid;
    cv::Scalar background;
	cv::Scalar axis;
	cv::Scalar grid_line;
	
    cv::Scalar pointS1;
    cv::Scalar pointS2;
    cv::Scalar pointS3;
    cv::Scalar pointS4;
	cv::Scalar pointS5;
    cv::Scalar pointS6;
    cv::Scalar pointS7;
};

static Theme ThemeArray[] = {
	/* Chinese Palette */
	{ 
        "Chinese",
        "Chinese",
		"Chinese",
		1,
		cv::Scalar(241,240,236),//rgb(236, 240, 241)
        cv::Scalar(80,62,44),//rgb(44, 62, 80)
		cv::Scalar(199,195,189),//rgb(189, 195, 199)

		cv::Scalar(113,204,46),//rgb(46, 204, 113)
		cv::Scalar(34,126,230),//rgb(230, 126, 34)
		cv::Scalar(219,152,52),// rgb(52, 152, 219)
		cv::Scalar(60,76,231),// rgb(231, 76, 60)
		cv::Scalar(156,188,26),// rgb(26, 188, 156)
		cv::Scalar(18,156,243),// rgb(243, 156, 18)
		cv::Scalar(94,73,52)// rgb(52, 73, 94)
	},

 	/* American Palette */
	{ 
        "American",
        "American",
		"American",
		1,
		cv::Scalar(233,230,223),//rgb(223, 230, 233)
        cv::Scalar(54,52,45),//rgb(45, 52, 54)
		cv::Scalar(195,190,178),//rgb(178, 190, 195)

		cv::Scalar(196,239,85),//rgb(85, 239, 196)
		cv::Scalar(117,118,255),//rgb(255, 118, 117)
		cv::Scalar(148,184,0),//rgb(0, 184, 148)
		cv::Scalar(227,132,9),//rgb(9, 132, 227)
		cv::Scalar(147,67,232),//rgb(232, 67, 147)
		cv::Scalar(231,92,108),//rgb(108, 92, 231)
		cv::Scalar(110,203,253)//rgb(253, 203, 110)
	},
};

enum  ThemeName{
	Chinese,
	American
};

static Theme getThemeInfo(enum ThemeName type)
{
	return ThemeArray[type];
}

#endif // !THEME_H