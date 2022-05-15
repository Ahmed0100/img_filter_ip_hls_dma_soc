#include "img_filter.h"
void img_filter(hls::stream<uint8_sc> & inStream,hls::stream<uint8_sc> &outStream,int kernel[KERNEL_DIM*KERNEL_DIM], int op)
{
#pragma HLS INTERFACE mode=s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE mode=s_axilite port=op bundle=CRTL_BUS
#pragma HLS INTERFACE mode=s_axilite port=kernel bundle=CRTL_BUS
#pragma HLS INTERFACE mode=axis port=outStream register
#pragma HLS INTERFACE mode=axis port=inStream register
	xf::cv::LineBuffer<KERNEL_DIM,IMG_WIDTH,unsigned int> lineBuffer;
	xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> window;

	int colIndex=0;
	int rowIndex=0;
	int pixConvoluted=0;
	int waitTicks=(IMG_WIDTH*(KERNEL_DIM-1)+KERNEL_DIM)/2;
	int countWait=0;
	int sentPixels=0;

	uint8_sc dataOutSc;
	uint8_sc currPixelSc;
	for(int i=0;i<(IMG_WIDTH*IMG_HEIGHT);i++)
	{
#pragma HLS PIPELINE
		currPixelSc = inStream.read();
		unsigned int pixelIn = currPixelSc.data;
		lineBuffer.shift_up(colIndex);
		lineBuffer.insert_top(pixelIn,colIndex);
		for(int row=0;row<KERNEL_DIM;row++)
		{
			for(int col=0;col<KERNEL_DIM;col++)
			{
				int val =(int) lineBuffer.getval(row,col+pixConvoluted);
				val = (int)kernel[(row*KERNEL_DIM)+col]*val;
				window.insert(val,row,col);
			}
		}
		int pixelOut=0;
		if(rowIndex>=KERNEL_DIM-1 && colIndex>=KERNEL_DIM-1)
		{
//			switch(op)
//			{
//			case 0:
//			{
//				pixelOut = sumWindow(&window);
//				if(pixelOut<0)
//					pixelOut=0;
//				break;
//			}
//			case 1:
//			{
//				pixelOut=minWindow(&window);
//				break;
//			}
//			case 2:
//			{
//				pixelOut=maxWindow(&window);
//				break;
//			}
//			}
			pixelOut = sumWindow(&window);
			if(pixelOut<0)
				pixelOut=0;
			pixConvoluted++;

		}
		if(colIndex<IMG_WIDTH-1)
		{
			colIndex++;
		}
		else
		{
			rowIndex++;
			pixConvoluted=0;
			colIndex=0;
		}
		dataOutSc.data = pixelOut;
		dataOutSc.keep = currPixelSc.keep;
		dataOutSc.dest = currPixelSc.dest;
		dataOutSc.id = currPixelSc.id;
		dataOutSc.strb = currPixelSc.strb;
		if(i== (IMG_WIDTH*IMG_HEIGHT)-1)
			dataOutSc.last = 1;
		else
			dataOutSc.last = 0;

		dataOutSc.user = currPixelSc.user;
		outStream.write(dataOutSc);
	}
}

//int minWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> * window)
//{
//	unsigned int minVal = 255;
//	for(int row=0;row<KERNEL_DIM;row++)
//	{
//		for(int col=0;col<KERNEL_DIM;col++)
//		{
//			unsigned int val = (unsigned int) window->getval(row,col);
//			if(val<minVal)
//			{
//				minVal=val;
//			}
//		}
//	}
//	return minVal;
//}
//int maxWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> * window)
//{
//	unsigned int maxVal = 0;
//	for(int row=0;row<KERNEL_DIM;row++)
//	{
//		for(int col=0;col<KERNEL_DIM;col++)
//		{
//			unsigned int val = (unsigned int) window->getval(row,col);
//			if(val>maxVal)
//			{
//				maxVal=val;
//			}
//		}
//	}
//	return maxVal;
//}
int sumWindow(xf::cv::Window<KERNEL_DIM,KERNEL_DIM,int> * window)
{
	unsigned int acc = 0;
	for(int row=0;row<KERNEL_DIM;row++)
	{
		for(int col=0;col<KERNEL_DIM;col++)
		{
			acc = acc + (unsigned int) window->getval(row,col);
		}
	}
	return acc;
}
