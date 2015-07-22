# m2s-bench-amdapp-2.5

##Build Instructions

All of the benchmark host programs can be built by running the `make` command from the top level directory. This will create both a static and dynamically linked version of the host program. The GPU binaries for each benchmark are all provided pre-compiled. The provided Makefile assumes that Multi2Sim source has been downloaded onto the home folder of the user's system in a folder called 'multi2sim'. Furthermore, the user copy of Multi2Sim should have been configured and built according to the instructions provided at https://www.multi2sim.org/development/multi2sim.php. The building of the benchmarks was tested on a system running Ubuntu 14.04, Linux kernel 3.13.0-32-generic, and GCC 4.8.4

In addition, the user can create a tarball of the statically linked host programs and GPU kernel binaries by running `make dist`.

Note: BoxFilterGL, Mandelbrot, and NBody will not be built due to the OpenGL requirements of the benchmarks.

##Run Instructions

All of the benchmarks can be run by providing the Multi2Sim binary, the benchmark host program, and the GPU binary via the `--load` command:

	`<multi2sim_binary> <benchmark_host_program> --load <benchmark_GPU_binary>`

For example, running the BinarySearch benchmark would be done in the following manner:

	`multisim/bin/m2s m2s-bench-amdapp-2.5/BinarySearch/BinarySearch --load BinarySearch/BinarySearch_Kernels.bin`

Each of the benchmarks provides several command line options which can be explored via the `--help` option such as a verification option which will run the GPU binary as well as a CPU only implementation of the benchmark to compare the GPU results against.

##Support on Multi2Sim

| Benchmark            | Disassembly   | Emulation     |
|----------------------|---------------|---------------|
| AESEncryptDecrypt    | No            | No            |
| BinarySearch         | Yes           | Yes           |
| BinomialOption       | Yes           | Yes           |
| BitonicSort          | Yes           | Yes           |
| BlackScholes         | Yes           | Yes           |
| BoxFilter            | No            | No            |
| BoxFilterGL          | No            | No            |
| DCT                  | Yes           | Yes           |
| DeviceFission        | Yes           | No            |
| DwtHaar1D            | Yes           | Yes           |
| EigenValue           | Yes           | Yes           |
| FFT                  | Yes           | Yes           |
| FastWalshTransform   | Yes           | Yes           |
| FloydWarshall        | Yes           | Yes           |
| FluidSimulation2D    | No            | No            |
| Histogram            | Yes           | Yes           |
| HistogramAtomics     | No            | No            |
| Mandelbrot           | No            | No            |
| MatrixMulImage       | Yes           | No            |
| MatrixMultiplication | Yes           | Yes           |
| MatrixTranspose      | Yes           | Yes           |
| MemoryOptimizations  | No            | No            |
| MersenneTwister      | Yes           | Yes           |
| MonteCarloAsian      | Yes           | No            |
| MonteCarloAsianDP    | Yes           | No            |
| NBody                | No            | No            |
| PrefixSum            | Yes           | Yes           |
| QuasiRandomSequence  | Yes           | Yes           |
| RadixSort            | Yes           | Yes           |
| RecursiveGaussian    | Yes           | Yes           |
| Reduction            | Yes           | Yes           |
| ScanLargeArrays      | Yes           | Yes           |
| SimpleConvolution    | Yes           | Yes           |
| SimpleImage          | No            | Yes           |
| SobelFilter          | Yes           | Yes           |
| URNG                 | Yes           | Yes           |


