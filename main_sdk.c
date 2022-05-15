#include "xparameters.h"
#include "xaxidma.h"
#include "ximg_filter.h"
#include "xio.h"
#define N 16


int kernel[9] = {	1,1,1,
					1,8,1,
					1,1,1,
				 };

int main()
{
	xil_printf("S\n");

	const int image[N] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int status;
	int init_addr = XPAR_BRAM_0_BASEADDR;
	for(int i=0;i<N;i++)
	{
		XIo_Out32(init_addr,image[i]);
		init_addr += 4;
	}
//dma init
	XAxiDma my_dma;
	XAxiDma_Config * cnfg;
	cnfg = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
	XAxiDma_CfgInitialize(&my_dma,cnfg);
//core start
	XImg_filter xf;
	XImg_filter_Initialize(&xf,XPAR_IMG_FILTER_0_DEVICE_ID);
	XImg_filter_Write_kernel_Words(&xf,0,(int*)kernel,9);
	XImg_filter_Set_op(&xf,0);
	XImg_filter_Start(&xf);

//data transfer
	status = XAxiDma_SimpleTransfer(&my_dma,XPAR_BRAM_0_BASEADDR,sizeof(int)*N,XAXIDMA_DMA_TO_DEVICE);
	if(status != XST_SUCCESS)
	{
		xil_printf("READING DATA FAILED\n");
	}
	status = XAxiDma_SimpleTransfer(&my_dma,XPAR_BRAM_0_BASEADDR+sizeof(int)*N,sizeof(int)*N,XAXIDMA_DEVICE_TO_DMA);
	if(status != XST_SUCCESS)
	{
		xil_printf("WRITING DATA FAILED\n");
	}
	while ((XAxiDma_Busy(&my_dma,XAXIDMA_DEVICE_TO_DMA)) ||
		(XAxiDma_Busy(&my_dma,XAXIDMA_DMA_TO_DEVICE))) {
			xil_printf("w.\n");
	}
	xil_printf("transfer done\n");
//read output
	int out_addr = XPAR_BRAM_0_BASEADDR + N*sizeof(int);
	for(int i=0;i<N;i++)
	{
		xil_printf("HW %d = %d",i,XIo_In32(out_addr));
		out_addr += 4;
	}

}