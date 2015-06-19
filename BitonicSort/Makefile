CC = g++

NATIVE_CFLAGS = -Wall -I$(AMDAPPSDKROOT)/include -I$(BENCHMARKS_ROOT)/include - I. -g
NATIVE_LDFLAGS = -L$(AMDAPPSDKROOT)/lib/x86 -lOpenCL

M2S_LIBOPENCL = $(M2S_LIB)/libm2s-opencl.so

BENCHMARKS_ROOT = ..
BENCHMARK_NAME = BitonicSort

PROGRAM_BINARY = $(BENCHMARK_NAME)
NATIVE_BINARY = $(BENCHMARK_NAME)_native
KERNEL_SOURCES = $(BENCHMARK_NAME)_Kernels.cl
KERNEL_BINARIES = \
	$(BENCHMARK_NAME)_Kernels_si.bin \
	$(BENCHMARK_NAME)_Kernels_x86.bin

all: $(PROGRAM_BINARY) $(NATIVE_BINARY) $(KERNEL_BINARIES)

clean:
	rm -f $(PROGRAM_BINARY) $(NATIVE_BINARY) $(KERNEL_BINARIES) \
		*_disasm *_fmetadata *_metadata *_amdil *_header *_kernel
	rm -rf $(BENCHMARK_NAME)_Kernels_AMDAPP_files

$(NATIVE_BINARY): *.cpp $(BENCHMARKS_ROOT)/common/*.cpp $(M2S_LIBOPENCL)
	$(CC) $(NATIVE_CFLAGS) *.cpp $(BENCHMARKS_ROOT)/common/*.cpp -o $(NATIVE_BINARY) $(NATIVE_LDFLAGS)

$(PROGRAM_BINARY): *.cpp $(BENCHMARKS_ROOT)/common/*.cpp $(M2S_LIBOPENCL)
	$(CC) $(CFLAGS) *.cpp $(BENCHMARKS_ROOT)/common/*.cpp -o $(PROGRAM_BINARY) $(LDFLAGS)

$(KERNEL_BINARIES): $(KERNEL_SOURCES)
	$(M2C) --amd-device Tahiti $(KERNEL_SOURCES) -o $(BENCHMARK_NAME)_Kernels_si.bin
	$(M2C) --amd-device APU $(KERNEL_SOURCES) -o $(BENCHMARK_NAME)_Kernels_x86.bin








