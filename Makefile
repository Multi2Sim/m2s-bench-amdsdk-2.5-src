OPENCL_BINARIES = \
	BinarySearch/BinarySearch_static \
	BinomialOption/BinomialOption_static \
	BitonicSort/BitonicSort_static \
	BlackScholes/BlackScholes_static \
	DCT/DCT_static \
	DwtHaar1D/DwtHaar1D_static \
	EigenValue/EigenValue_static \
	FFT/FFT_static \
	FastWalshTransform/FastWalshTransform_static \
	FloydWarshall/FloydWarshall_static \
	Histogram/Histogram_static \
	MatrixMultiplication/MatrixMultiplication_static \
	MatrixTranspose/MatrixTranspose_static \
	MersenneTwister/MersenneTwister_static \
	PrefixSum/PrefixSum_static \
	QuasiRandomSequence/QuasiRandomSequence_static \
	RadixSort/RadixSort_static \
	RecursiveGaussian/RecursiveGaussian_static \
	Reduction/Reduction_static \
	ScanLargeArrays/ScanLargeArrays_static \
	SimpleConvolution/SimpleConvolution_static \
	SobelFilter/SobelFilter_static \
	URNG/URNG_static

KERNEL_BINARIES = \
	BinarySearch/BinarySearch_Kernels.bin \
	BinomialOption/BinomialOption_Kernels.bin \
	BitonicSort/BitonicSort_Kernels.bin \
	BlackScholes/BlackScholes_Kernels.bin \
	DCT/DCT_Kernels.bin \
	DwtHaar1D/DwtHaar1D_Kernels.bin \
	EigenValue/EigenValue_Kernels.bin \
	FFT/FFT_Kernels.bin \
	FastWalshTransform/FastWalshTransform_Kernels.bin \
	FloydWarshall/FloydWarshall_Kernels.bin \
	Histogram/Histogram_Kernels.bin \
	MatrixMultiplication/MatrixMultiplication_Kernels.bin \
	MatrixTranspose/MatrixTranspose_Kernels.bin \
	MersenneTwister/MersenneTwister_Kernels.bin \
	PrefixSum/PrefixSum_Kernels.bin \
	QuasiRandomSequence/QuasiRandomSequence_Kernels.bin \
	RadixSort/RadixSort_Kernels.bin \
	RecursiveGaussian/RecursiveGaussian_Kernels.bin \
	Reduction/Reduction_Kernels.bin \
	ScanLargeArrays/ScanLargeArrays_Kernels.bin \
	SimpleConvolution/SimpleConvolution_Kernels.bin \
	SobelFilter/SobelFilter_Kernels.bin \
	URNG/URNG_Kernels.bin

SUBDIRS = \
	AESEncryptDecrypt \
	BinarySearch \
	BinomialOption \
	BitonicSort \
	BlackScholes \
	BoxFilter \
	DCT \
	DeviceFission \
	DwtHaar1D \
	EigenValue \
	FastWalshTransform \
	FloydWarshall \
	FFT \
	Histogram \
	HistogramAtomics \
	MatrixMultiplication \
	MatrixMulImage \
	MatrixTranspose \
	MemoryOptimizations \
	MersenneTwister \
	MonteCarloAsian \
	MonteCarloAsianDP \
	PrefixSum \
	QuasiRandomSequence \
	RadixSort \
	RecursiveGaussian \
	Reduction \
	ScanLargeArrays \
	SimpleConvolution \
	SimpleImage \
	SobelFilter \
	URNG

EXTRA_DIST = \
	URNG/images/*.bmp \
	URNG/images/README \
	SobelFilter/images/*.bmp \
	SobelFilter/images/README \
	RecursiveGaussian/images/*.bmp \
	RecursiveGaussian/images/README
    
BENCHMARK_SUITE = m2s-bench-amdapp-2.5

TARBALL_NAME = $(BENCHMARK_SUITE).tar.gz
M2S_ROOT = $(HOME)/multi2sim
M2S_LIB = $(M2S_ROOT)/lib/.libs
M2S_BIN = $(M2S_ROOT)/bin
M2S_INCLUDE = $(M2S_ROOT)/runtime/include
M2S_DEVICE = "Tahiti"

CFLAGS = "-I../include -I. -I../common -I$(M2S_INCLUDE) -g"
LDFLAGS_STATIC = "-L$(M2S_LIB) -lm2s-opencl -lpthread -ldl -lrt -static -m32"
LDFLAGS_DYNAMIC = "-L$(M2S_LIB) -lm2s-opencl -lpthread -ldl -lrt -m32"

all: premake

dist: premake $(TARBALL_NAME)

clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean || exit 1; \
	done
	rm -f $(TARBALL_NAME)

premake:
	make -C $(M2S_ROOT)/runtime/opencl
	for subdir in $(SUBDIRS); do \
		make -C $$subdir \
			M2S_ROOT=$(M2S_ROOT) \
			M2S_LIB=$(M2S_LIB) \
			M2S_BIN=$(M2S_BIN) \
			M2S_INCLUDE=$(M2S_INCLUDE) \
			M2S_DEVICE=$(M2S_DEVICE) \
			CFLAGS=$(CFLAGS) \
			LDFLAGS_STATIC=$(LDFLAGS_STATIC) \
			LDFLAGS_DYNAMIC=$(LDFLAGS_DYNAMIC) \
			M2C=$(M2C) \
			|| exit 1; \
	done

$(TARBALL_NAME): $(OPENCL_BINARIES) $(KERNEL_BINARIES)
	rm -f $(TARBALL_NAME)
	tar -czvf $(TARBALL_NAME) --transform='s,^,$(BENCHMARK_SUITE)/,' $(OPENCL_BINARIES) \
		$(KERNEL_BINARIES) $(EXTRA_DIST)
