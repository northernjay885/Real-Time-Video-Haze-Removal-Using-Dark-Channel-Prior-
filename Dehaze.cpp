#include "stdafx.h"

using namespace cv;
using namespace cv::cuda;
using std::cout;
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;
Mat src1;
Mat src2;
Mat dst;

void dark_prior(GpuMat &gpuimg, int r, GpuMat &dark_prior)
{
	int win_size = 2 * r + 1;
	GpuMat gpuimg_splitted[3];
	GpuMat temp1, temp2;

	//GpuMat dark_prior;
	Mat kernel((win_size, win_size), 1);
	
	gpuimg_splitted = cv::cuda::split(gpuimg);
	cv::cuda::min(gpuimg_splitted[0], gpuimg_splitted[1], temp1);
	cv::cuda::min(gpuimg_splitted[2], temp1, temp2);
	Ptr<cuda::Filter>minfilter = cv::cuda::createMorphologyFilter(MORPH_ERODE, kernel, iteration = 1);
	minfilter->apply(temp2, dark_prior);

}

Mat AL_estimation(Mat img, GpuMat dark_prior)
{
	Mat dark_prior_cpu(dark_prior);
	Mat Idx;
	Mat img_temp;
	Mat dark_temp;
	Mat A;
	Mat AL_sum;
	int num_pixel;
	int img_size;

	img_size = Size(dark_prior_cpu)[0] * Size(dark_prior_cpu)[1];
	num_pixel = int(img_size / 1000 > 1 ? img_size / 1000 : 1)

	cv::reshape(img, img_size, 3, img_temp);
	cv::reshape(dark_prior_cpu, img_size, 1, dark_temp);
	sortIdx(dark_temp, Idx, SORT_EVERY_ROW + SORT_DESCENDING);
	index_use = index[img_size - num_pixel:]
	for (int i = 0; i < num_pixel, i++) {
		AL_sum = AL_sum + img_temp[index_use[i]];
		AL = AL_sum / num_pixel;
		A = cv::min(AL, thread);
		}
	return A;
}

void Trans_estimation(GpuMat gpuimg, Mat A, int r, int omega = 0.95, GpuMat &t)
{
	GpuMat gpuimg_splitted[3];
	gpuimg_splitted = cv::cuda::split(gpuimg);
	GpuMat temp[3];
	GpuMat mergedimg;
	GpuMat t;
	for (int i = 0, i < 3, i++)
	{
		cv::cuda::divide(gpuimg_splitted[i], A<vec3f>[i], temp[i]);
	}
	cv::cuda::merge(temp, mergedimg);
	dark_prior(mergedimg, r, t);
	cv::cuda::multiply(t, omega, t);
	cv::cuda::subtract(1, t, t);
}

void Guidedfilter(GpuMat gpuimg, GpuMat t, GpuMat &q ,int r, float eps) {

	GpuMat mean_I, mean_p, corr_I, corr_Ip;
	GpuMat temp1, temp2, temp3;
	Mat kernel;
	Ptr<cuda::Filter>meanfilter = cv::cuda::createBoxFilter(kernel, iteration = 1);
	meanfilter->apply(mean_I, gpuimg);
	meanfilter->apply(mean_p, t);

	cuda::multiply(gpuimg, gpuimg, temp1);
	cuda::multiply(gpuimg, t, temp2);

	meanfilter->apply(corr_I,temp1);
	meanfilter->apply(corr_Ip, temp2);

	var_I = corr_I - mean_I*mean_I;
	cov_Ip = corr_Ip - mean_I*mean_p;

	a = cov_Ip / (var_I + eps);
	b = mean_p - a*mean_I;

	meanfilter->apply(mean_a, a);
	meanfilter->apply(mean_b, b);
	

	q = mean_a * I + mean_b;


}

static void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);
	addWeighted(src1, alpha, src2, beta, 0.0, dst);
	imshow("Dehazed", dst);
}
int main(void)
{
	Mat frame;
	cv::VideoCapture(cap);
	cap.open(0);
	cap->frame;
	GpuMat A;
	GpuMat t;
	GpuMat x;
	for () {
		GpuMat gpuimg(frame);
		dark_prior(gpuimg, 7, dark_prior);
		AL_estimation(img, dark_prior);
		Trans_estimation(gpuimg,A, 7,omega = 0.95, t);
		Guidedfilter(gpuimg, t, q, 7, 0.001);
		if (src1.empty()) { cout << "Error loading src1 \n"; return -1; }
		if (src2.empty()) { cout << "Error loading src2 \n"; return -1; }
		alpha_slider = 0;
		namedWindow("Dehazed", WINDOW_AUTOSIZE); // Create Window
		char TrackbarName[50];
		sprintf_s(TrackbarName, "Alpha x %d", alpha_slider_max);
		createTrackbar(TrackbarName, "Dehazed", &alpha_slider, alpha_slider_max, on_trackbar);
		imshow(frame, cv2DRotationMatrix);
		on_trackbar(alpha_slider, 0);
		waitKey(0);
		return 0;
	}
}
