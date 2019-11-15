#include"ExractFeature.h"

inline void Random(int start,int range,std::vector<int>& vec)
{
	unsigned seed;
	seed = time(0);
	srand(seed);
	for (int i = 0;i < vec.size(); i++)
	{
		vec[i] = rand()% range +start;
	}
	
}

ExtractFeature::ExtractFeature(std::string video_file, std::string SuperPixel_video_save_file, std::string resize_video_save_file, std::string feature_position_save_file)
{
	this->video_file = video_file;
	this->SuperPixel_video_save_file = SuperPixel_video_save_file;
	this->resize_video_save_file = resize_video_save_file;
	this->feature_position_save_file = feature_position_save_file;
}

ExtractFeature::~ExtractFeature()
{

}

void ExtractFeature::ResizeVideo(std::string video_name,cv::Size video_size)
{
	cv::VideoCapture video_capture;
	cv::VideoWriter video_writer;
	std::string single_video_file = video_file + video_name;
	std::string video_save_file = resize_video_save_file + video_name;
	
	video_capture.open(single_video_file);
	double fps = video_capture.get(CV_CAP_PROP_FPS);
	int height = video_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = video_capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_num = video_capture.get(CV_CAP_PROP_FRAME_COUNT);

	this->fps = fps;
	this->resize_video_height = height;
	this->resize_video_width = width;

	video_writer.open(video_save_file, CV_FOURCC('D', 'I', 'V', 'X'), fps, video_size);
	if (!video_writer.isOpened())
	{
		std::printf("write video error\n");
		return;
	}

	for(int i=0;i<frame_num;i++)
	{
		if (i % 100 == 0)
		{
			std::printf("already resized the %i frame\n", i);
		}
		cv::Mat dst_frame;
		cv::Mat org_frame;
		video_capture >> org_frame;
		
		cv::resize(org_frame, dst_frame, video_size);
		video_writer << dst_frame;
	}

	std::printf("resize video over\n");
}

template<class T> void ExtractFeature::RecoverVector(std::vector<T>& vec)
{
	std::vector<T> empty;
	empty.swap(vec);
}

void ExtractFeature::FindSuperPixel(std::string video_name)
{
	//read video
	cv::VideoCapture video_caputure;
	cv::VideoWriter video_writer;
	

	video_caputure.open(resize_video_save_file + video_name);
	if (!video_caputure.isOpened())
	{
		std::cout << "can't open video" << std::endl;
		return;
	}

	int frame_count = video_caputure.get(CV_CAP_PROP_FRAME_COUNT);
	int height = video_caputure.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = video_caputure.get(CV_CAP_PROP_FRAME_WIDTH);
	int fps = video_caputure.get(CV_CAP_PROP_FPS);

	video_writer.open(SuperPixel_video_save_file + video_name, CV_FOURCC('D', 'I', 'V', 'X'), fps, cv::Size(width, height));
	if (!video_writer.isOpened())
	{
		std::cout << "can't build super pixel video" << std::endl;
		return;
	}
	
	for (int i = 0; i < frame_count; i++)
	{
		if (i % 100 == 0)
		{
			printf("process %s %i th frame\n", video_name,i);
		}

		cv::Mat frame;
		cv::Mat labels;
		cv::Mat mask;
		video_caputure >> frame;

		cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(frame, 101, 5);
		slic->iterate(10);
		slic->enforceLabelConnectivity();
		slic->getLabelContourMask(mask);
		slic->getLabels(labels);

		//compute and process the averge of Super Pixel
		int label_num = slic->getNumberOfSuperpixels();
		super_piexl_num = label_num;
		
		std::vector<std::vector<cv::Point2i>> labels_collector;
		std::vector<cv::Vec3i> SuperPixel_color;
		SuperPixel_color.reserve(label_num);
		labels_collector.resize(label_num);

		for (int m = 0; m < height; m++)
		{
			for (int n = 0; n < width; n++)
			{
				int single_label = labels.at<int>(m, n);
				labels_collector[single_label].push_back(cv::Point2i(n, m));
			}
		}

		//compute the averge
		for (int m = 0; m < label_num; m++)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			for (int n = 0; n < labels_collector[m].size(); n++)
			{
				r += frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[2];
				g += frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[1];
				b += frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[0];
			}

			r = double(r) / labels_collector[m].size();
			g = double(g) / labels_collector[m].size();
			b = double(b) / labels_collector[m].size();

			SuperPixel_color.push_back(cv::Vec3i(r, g, b));

			//adopt the averge
			for (int n = 0; n < labels_collector[m].size(); n++)
			{
				frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[2] = r;
				frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[1] = g;
				frame.at<cv::Vec3b>(labels_collector[m][n].y, labels_collector[m][n].x)[0] = b;
			}
		}

		//done 

		//test
		cv::namedWindow("test");
		cv::imshow("test", frame);
		cv::waitKey(10);

		//save the new video
		video_writer << frame;
		
		//save SuperPixel color
		//SaveTxt(video_name, SuperPixel_color);

		//recover the memory
		for (int m = 0; m < labels_collector.size(); m++)
		{
			RecoverVector(labels_collector[m]);
		}
		RecoverVector(labels_collector);
		RecoverVector(SuperPixel_color);
	}
}

void ExtractFeature::BuildFeature(std::string video_name,int feature_index)
{

}

void ExtractFeature::RandomFeaturePosition(int index)
{
	
	std::string r, g, b;
	std::ofstream writer;
	std::stringstream exchange;
	cv::VideoCapture video_capture;
	std::string feature_save_file = resize_video_save_file + "feature_position_" + std::to_string(index) + "csv";
	
	//get the video info

	int height = resize_video_height;
	int width = resize_video_width;

	//random build feature;
	//random frame num
	std::vector<int> random_frame_1;
	std::vector<int> random_frame_2;

	std::vector<int> random_x_1;
	std::vector<int> random_x_2;

	std::vector<int> random_y_1;
	std::vector<int> random_y_2;

	random_frame_1.resize(feature_num);
	random_frame_2.resize(feature_num);

	random_x_1.resize(feature_num);
	random_x_2.resize(feature_num);

	random_y_1.resize(feature_num);
	random_y_2.resize(feature_num);

	Random(-frame_range / 2, frame_range, random_frame_1);
	Random(-frame_range / 2, frame_range, random_frame_2);

	Random(0, width, random_x_1);
	Random(0, width, random_x_2);

	Random(0, height, random_y_1);
	Random(0, height, random_y_2);
	
	
	//save feature
	writer.open(feature_save_file, std::ios::out);
	if (!writer.is_open())
	{
		printf("save feature position txt error\n");
	}
	for (int i = 0; i < feature_num; i++)
	{
		writer << std::to_string(random_frame_1[i]) << ","
			<< std::to_string(random_frame_2[i]) << ","
			<< std::to_string(random_x_1[i]) << ","
			<< std::to_string(random_x_2[i]) << ","
			<< std::to_string(random_y_1[i]) << ","
			<< std::to_string(random_y_2[i]) << "\n";
			
	}
	writer.close();


}

void ExtractFeature::SaveTxt(std::string video_name, std::vector<cv::Vec3i> data)
{
	//open the file
	std::ofstream writer;
	//delete .mp4

	video_name.erase(video_name.end() - 4, video_name.end());
	writer.open(SuperPixelColorSaveFile + video_name+".csv", std::ios::out|std::ios::app);
	if (!writer.is_open())
	{
		std::cout << "error open file" << std::endl;
	}

	for (int i = 0; i < data.size(); i++)
	{
		std::stringstream exchange;
		std::string r, g, b;
		exchange << data[i][0] <<" "<< data[i][1]<<" " << data[i][2];
		exchange >> r >> g >> b;

		writer << r << "," << g << "," << b << "\n";
	}

	writer.close();
}