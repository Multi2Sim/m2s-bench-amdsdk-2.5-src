# m2s-bench-amdapp-2.5

##Build Instructions

All of the benchmark host programs can be built by running the `make` command from the top level directory. The GPU binaries for each benchmark are all provided pre-compiled. The provided Makefile assumes that Multi2Sim source has been downloaded onto the home folder of the user's system in a folder called 'multi2sim'. Furthermore, the user copy of Multi2Sim should have been configured and built according to the instructions provided at https://www.multi2sim.org/development/multi2sim.php. The building of the benchmarks was tested on a system running Ubuntu 14.04 and GCC 4.8.4

Note: BoxFilterGL, Mandelbrot, and NBody will not be built due to the OpenGL requirements of the benchmarks.

##Run Instructions

All of the benchmarks can be run by providing the Multi2Sim binary, the benchmark host program, and the GPU binary via the `--load` command:

	`<multi2sim_binary> <benchmark_host_program> --load <benchmark_GPU_binary>`

For example, running the BinarySearch benchmark would be done in the following manner:

	`multisim/bin/m2s m2s-bench-amdapp-2.5/BinarySearch/BinarySearch --load BinarySearch/BinarySearch_Kernels.bin`

Each of the benchmarks provides several command line options which can be explored via the `--help` option such as a verification option which will run the GPU binary as well as a CPU only implementation of the benchmark to compare the GPU results against.

##Disassembly and Emulation Support

| Benchmark            | Disassembly   | Emulation     |
|----------------------|---------------|---------------|
| AESEncryptDecrypt    | not supported | not supported |
| BinarySearch         | supported     | supported     |
| BinomialOption       | supported     | supported     |
| BitonicSort          | supported     | supported     |
| BlackScholes         | supported     | supported     |
| BoxFilter            | not supported | not supported |
| BoxFilterGL          | not supported | not supported |
| DCT                  | supported     | supported     |
| DeviceFission        | supported     | not supported |
| DwtHaar1D            | supported     | supported     |
| EigenValue           | supported     | supported     |
| FFT                  | supported     | supported     |
| FastWalshTransform   | supported     | supported     |
| FloydWarshall        | supported     | supported     |
| FluidSimulation2D    | not supported | not supported |
| Histogram            | supported     | supported     |
| HistogramAtomics     | not supported | not supported |
| Mandelbrot           | not supported | not supported |
| MatrixMulImage       | supported     | not supported |
| MatrixMultiplication | supported     | supported     |
| MatrixTranspose      | supported     | supported     |
| MemoryOptimizations  | not supported | not supported |
| MersenneTwister      | supported     | supported     |
| MonteCarloAsian      | supported     | not supported |
| MonteCarloAsianDP    | supported     | not supported |
| NBody                | not supported | not supported |
| PrefixSum            | supported     | supported     |
| QuasiRandomSequence  | supported     | supported     |
| RadixSort            | supported     | supported     |
| RecursiveGaussian    | supported     | supported     |
| Reduction            | supported     | supported     |
| ScanLargeArrays      | supported     | supported     |
| SimpleConvolution    | supported     | supported     |
| SimpleImage          | not supported | supported     |
| SobelFilter          | supported     | supported     |
| URNG                 | supported     | supported     |


