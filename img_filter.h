#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "common/xf_params.hpp"
#include "common/xf_common.hpp"
#include "common/xf_video_mem.hpp"
#include "ap_int.h"
#define IMG_WIDTH 6
#define IMG_HEIGHT 6

#define KERNEL_DIM  3

typedef ap_axiu<32,1,1,1> uint8_sc;

int convolute(xf::cv::Window<KERNEL_DIM,KERNEL_DIM, int> * window, int kernal[KERNEL_DIM*KERNEL_DIM]);
//
//int maxWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> *window);
//
//int minWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> *window);
int sumWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> *window);


void img_filter(hls::stream<uint8_sc> & inStream,hls::stream<uint8_sc> &outStream,int kernel[KERNEL_DIM*KERNEL_DIM], int op);
