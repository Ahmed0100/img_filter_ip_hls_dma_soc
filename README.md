# img_filter_ip_hls_dma_soc
A DMA system that interfaces with an IP that applies a 3x3 kernel convolution on the image received on an its AXI4 stream interface. A microblaze processor writes the pixels bytes to a bram memory. DMA transfers them from memory to the IP and transfers the results back to the memory. The microblaze reads the results back from memory.
