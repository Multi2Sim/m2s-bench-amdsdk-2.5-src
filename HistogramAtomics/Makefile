CC = g++

M2S_LIBOPENCL = $(M2S_LIB)/libm2s-opencl.so

BENCHMARK_NAME = HistogramAtomics
BENCHMARKS_ROOT = ..

PROGRAM_BINARY = $(BENCHMARK_NAME)
KERNEL_SOURCES = $(BENCHMARK_NAME)_Kernels.cl

all: $(PROGRAM_BINARY) $(KERNEL_BINARIES)

clean:
	rm -f $(PROGRAM_BINARY) \
		*_disasm *_fmetadata *_metadata *_amdil *_header *_kernel
	rm -rf $(BENCHMARK_NAME)_Kernels_AMDAPP_files


$(PROGRAM_BINARY): *.cpp $(BENCHMARKS_ROOT)/common/*.cpp $(M2S_LIBOPENCL)
	$(CC) $(CFLAGS) *.cpp $(BENCHMARKS_ROOT)/common/*.cpp -o $(PROGRAM_BINARY) $(LDFLAGS)
