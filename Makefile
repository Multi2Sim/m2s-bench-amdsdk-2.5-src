OPENCL_BINARIES_RUNNING = \
	BinarySearch/BinarySearch \
	BinomialOption/BinomialOption \
	BitonicSort/BitonicSort \
	DCT/DCT \
	DwtHaar1D/DwtHaar1D \
	FastWalshTransform/FastWalshTransform \
	FloydWarshall/FloydWarshall \
	Histogram/Histogram \
	MatrixMultiplication/MatrixMultiplication \
	MatrixTranspose/MatrixTranspose \
	PrefixSum/PrefixSum \
	RadixSort/RadixSort \
	Reduction/Reduction \
	ScanLargeArrays/ScanLargeArrays \
	SobelFilter/SobelFilter \
	URNG/URNG

KERNEL_BINARIES_RUNNING = \
	BinarySearch/BinarySearch_Kernels.bin \
	BinomialOption/BinomialOption_Kernels.bin \
	BitonicSort/BitonicSort_Kernels.bin \
	DCT/DCT_Kernels.bin \
	FastWalshTransform/FastWalshTransform_Kernels.bin \
	DwtHaar1D/DwtHaar1D_Kernels.bin \
	FloydWarshall/FloydWarshall_Kernels.bin \
	Histogram/Histogram_Kernels.bin \
	MatrixMultiplication/MatrixMultiplication_Kernels.bin \
	MatrixTranspose/MatrixTranspose_Kernels.bin \
	PrefixSum/PrefixSum_Kernels.bin \
	RadixSort/RadixSort_Kernels.bin \
	Reduction/Reduction_Kernels.bin \
	ScanLargeArrays/ScanLargeArrays_Kernels.bin \
	SobelFilter/SobelFilter_Kernels.bin \
	URNG/URNG_Kernels.bin

SUBDIRS_RUNNING = \
	BinarySearch \
	BinomialOption \
	BitonicSort \
	DCT \
	DwtHaar1D \
	FastWalshTransform \
	FloydWarshall \
	Histogram \
	MatrixMultiplication \
	MatrixTranspose \
	PrefixSum \
	RadixSort \
	Reduction \
	ScanLargeArrays \
	SobelFilter \
	URNG

EXTRA_DIST_RUNNING = \
	SobelFilter/images/*.bmp \
	URNG/images/*.bmp


OPENCL_BINARIES = \
	AESEncryptDecrypt/AESEncryptDecrypt \
	BinarySearch/BinarySearch \
	BinomialOption/BinomialOption \
	BitonicSort/BitonicSort \
	BlackScholes/BlackScholes \
	BoxFilter/BoxFilter \
	DCT/DCT \
	DeviceFission/DeviceFission \
	DwtHaar1D/DwtHaar1D \
	EigenValue/EigenValue \
	FastWalshTransform/FastWalshTransform \
	FloydWarshall/FloydWarshall \
	FFT/FFT \
	Histogram/Histogram \
	HistogramAtomics/HistogramAtomics \
	MatrixMultiplication/MatrixMultiplication \
	MatrixMulImage/MatrixMulImage \
	MatrixTranspose/MatrixTranspose \
	MemoryOptimizations/MemoryOptimizations \
	MersenneTwister/MersenneTwister \
	MonteCarloAsian/MonteCarloAsian \
	MonteCarloAsianDP/MonteCarloAsianDP \
	PrefixSum/PrefixSum \
	QuasiRandomSequence/QuasiRandomSequence \
	RadixSort/RadixSort \
	RecursiveGaussian/RecursiveGaussian \
	Reduction/Reduction \
	ScanLargeArrays/ScanLargeArrays \
	SimpleConvolution/SimpleConvolution \
	SimpleImage/SimpleImage \
	SobelFilter/SobelFilter \
	URNG/URNG

KERNEL_BINARIES = \
	AESEncryptDecrypt/AESEncryptDecrypt_Kernels.bin \
	BinarySearch/BinarySearch_Kernels.bin \
	BinomialOption/BinomialOption_Kernels.bin \
	BitonicSort/BitonicSort_Kernels.bin \
	BlackScholes/BlackScholes_Kernels.bin \
	BoxFilter/BoxFilter_Kernels.bin \
	DCT/DCT_Kernels.bin \
	DeviceFission/DeviceFission_Kernels.bin \
	DwtHaar1D/DwtHaar1D_Kernels.bin \
	EigenValue/EigenValue_Kernels.bin \
	FastWalshTransform/FastWalshTransform_Kernels.bin \
	FloydWarshall/FloydWarshall_Kernels.bin \
	FFT/FFT_Kernels.bin \
	Histogram/Histogram_Kernels.bin \
	HistogramAtomics/HistogramAtomics_Kernels.bin \
	MatrixMultiplication/MatrixMultiplication_Kernels.bin \
	MatrixMulImage/MatrixMulImage_Kernels.bin \
	MatrixTranspose/MatrixTranspose_Kernels.bin \
	MemoryOptimizations/MemoryOptimizations_Kernels.bin \
	MersenneTwister/MersenneTwister_Kernels.bin \
	MonteCarloAsian/MonteCarloAsian_Kernels.bin \
	MonteCarloAsianDP/MonteCarloAsianDP_Kernels.bin \
	PrefixSum/PrefixSum_Kernels.bin \
	QuasiRandomSequence/QuasiRandomSequence_Kernels.bin \
	RadixSort/RadixSort_Kernels.bin \
	RecursiveGaussian/RecursiveGaussian_Kernels.bin \
	Reduction/Reduction_Kernels.bin \
	ScanLargeArrays/ScanLargeArrays_Kernels.bin \
	SimpleConvolution/SimpleConvolution_Kernels.bin \
	SimpleImage/SimpleImage_Kernels.bin \
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
    
BENCHMARK_SUITE = m2s-benchmarks-opencl-2.5

TARBALL_NAME = $(BENCHMARK_SUITE).tar.gz
TARBALL_NAME_RUNNING = $(BENCHMARK_SUITE)_running.tar.gz
M2S_ROOT = $(HOME)/multi2sim
M2S_LIB = $(M2S_ROOT)/lib/.libs
M2S_BIN = $(M2S_ROOT)/bin
M2S_INCLUDE = $(M2S_ROOT)/runtime/include
M2S_DEVICE = "Tahiti"

CFLAGS = "-I../include -I. -I../common -I$(M2S_INCLUDE) -g"
LDFLAGS_STATIC = "-L$(M2S_LIB) -lm2s-opencl -lpthread -ldl -lrt -static -m32"
LDFLAGS_DYNAMIC = "-L$(M2S_LIB) -lm2s-opencl -lpthread -ldl -lrt -m32"

M2C = "$(M2S_BIN)/m2c --amd"

all: premake

dist: $(TARBALL_NAME)

running: premake $(TARBALL_NAME_RUNNING)

clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean || exit 1; \
	done
	rm -f $(TARBALL_NAME)
	rm -f $(TARBALL_NAME_RUNNING)

#SUBDIRS = MatrixMultiplication ##########
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

$(TARBALL_NAME_RUNNING): $(OPENCL_BINARIES_RUNNING) $(KERNEL_BINARIES_RUNNING)
	rm -f $(TARBALL_NAME_RUNNING)
	tar -czvf $(TARBALL_NAME_RUNNING) --transform='s,^,$(BENCHMARK_SUITE)/,' $(OPENCL_BINARIES_RUNNING) \
		$(KERNEL_BINARIES_RUNNING) $(EXTRA_DIST_RUNNING)

$(TARBALL_NAME): $(OPENCL_BINARIES) $(KERNEL_BINARIES)
	rm -f $(TARBALL_NAME)
	tar -czvf $(TARBALL_NAME) --transform='s,^,$(BENCHMARK_SUITE)/,' $(OPENCL_BINARIES) \
		$(KERNEL_BINARIES) $(EXTRA_DIST)

