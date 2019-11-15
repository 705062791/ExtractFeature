#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/ximgproc.hpp>
#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<ctime>
 
/*
����
��1������Ƶ�Ĵ�������SuperPixel{ƽ��������ɫ,�����µ���Ƶ}
��2����������{����ԭ���ķ����Ϳ��ԣ����ǵ㵽���������ÿ��SuperPixel����ɫ����һ����}
��3����ȡ����{����ԭ���ķ���}
��4��ѵ��
��5����Ⱦ
*/

class ExtractFeature
{
private:
	//data

	//orgrion video file
	std::string video_file;
	//new SuperPixel video
	std::string SuperPixel_video_save_file;
	//pixel color save file
	std::string SuperPixelColorSaveFile;
	//resize video
	std::string resize_video_save_file;
	//feature position save file
	std::string feature_position_save_file;
	//feature num
	int feature_num = 2500;
	//feature range
	int frame_range = 700;
	//SuperPiexl num
	int super_piexl_num = 0;
	//video info
	int resize_video_height = 0;
	int resize_video_width = 0;
	double fps = 0;

	template <class T> void RecoverVector(std::vector<T>& vec);
	void SaveTxt(std::string video_name,std::vector<cv::Vec3i> data);
public:
	ExtractFeature(std::string video_file,
		std::string SuperPixel_video_save_file, 
		std::string resize_video_save_file, 
		std::string feature_position_save_file);
	~ExtractFeature();

	void FindSuperPixel(std::string video_name);
	void RandomFeaturePosition(int index);
	void BuildFeature(std::string video_name, int feature_index);
	void ResizeVideo(std::string video_name,cv::Size video_size);
};