#include"ExractFeature.h"
int main()
{
	const cv::Size VideoSize_360p(540, 360);
	const cv::Size VideoSize_240p(320, 240);
	const int feature_set_num = 10;
	
	ExtractFeature extractFeature("D:/data/video_data/mp4/nomark/",
		"D:/data/video_data/mp4/super_pixel/",
		"D:/data/video_data/mp4/360p/",
		"D:/data/ÑµÁ·Êý¾Ý/feature_position/");

	std::vector<std::string> video_name = { "DJI_0027.mp4","DJI_0028.mp4" ,"DJI_0029.mp4" ,"DJI_0030.mp4" ,"DJI_0031.mp4" ,"DJI_0032.mp4"
	"DJI_0033.mp4" ,"DJI_0034.mp4" ,"DJI_0035.mp4" ,"DJI_0036.mp4" ,"DJI_0037.mp4" ,"DJI_0038.mp4" ,"DJI_0039.mp4" ,"DJI_0040.mp4"
	,"DJI_0041.mp4","DJI_0042.mp4","DJI_0043.mp4","DJI_0044.mp4"};

	//for (int i = 0; i < feature_set_num; i++)
	//{

	//}
	

	for (int i = 2; i < video_name.size(); i++)
	{
		//resize org video to 360p
		extractFeature.ResizeVideo(video_name[i],VideoSize_240p);
		//find super pixel
		extractFeature.FindSuperPixel(video_name[i]);
	}
}