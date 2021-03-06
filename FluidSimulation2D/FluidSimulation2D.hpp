/* ============================================================

Copyright (c) 2009 Advanced Micro Devices, Inc.  All rights reserved.

Redistribution and use of this material is permitted under the following 
conditions:

Redistributions must retain the above copyright notice and all terms of this 
license.

In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERA TION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */


#ifndef FLUID_SIMULATION2D_H_
#define FLUID_SIMULATION2D_H_

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <SDKCommon.hpp>
#include <SDKApplication.hpp>
#include <SDKCommandArgs.hpp>
#include <SDKFile.hpp>


#define GROUP_SIZE 256
#define	LBWIDTH		256
#define	LBHEIGHT	256

int winwidth = LBWIDTH;
int winheight = LBHEIGHT;


/**
* FluidSimulation2D 
* Class implements OpenCL  FluidSimulation2D sample
* Derived from SDKSample base class
*/

class FluidSimulation2D : public SDKSample
{
    cl_double setupTime;                /**< time taken to setup OpenCL resources and building kernel */
    cl_double kernelTime;               /**< time taken to run kernel and read result back */

    size_t maxWorkGroupSize;            /**< Max allowed work-items in a group */
    cl_uint maxDimensions;              /**< Max group dimensions allowed */
    size_t* maxWorkItemSizes;           /**< Max work-items sizes in each dimensions */
    cl_ulong totalLocalMemory;          /**< Max local memory allowed */
    cl_ulong usedLocalMemory;           /**< Used local memory */

    int dims[2];                                // Dimension of LBM simulation area

    // 2D Host buffers
    cl_double *rho;                              // Density
    cl_double2 *u;                               // Velocity
    cl_double *h_if0, *h_if1234, *h_if5678;      // Host input buffers
    cl_double *h_of0, *h_of1234, *h_of5678;      // Host output buffers

    cl_double *v_ef0, *v_ef1234, *v_ef5678;      // Host Eq distribution buffers for verification
    cl_double *v_of0, *v_of1234, *v_of5678;      // Host output buffers for verification

    cl_bool *h_type;                            // Cell Type - Boundary = 1 or Fluid = 0
    cl_double *h_weight;                         // Weights for each direction
    cl_double8 dirX, dirY;                       // Directions

    // Device buffers
    cl_mem d_if0, d_if1234, d_if5678;           // Input distributions
    cl_mem d_of0, d_of1234, d_of5678;           // Output distributions
    cl_mem type;                                // Constant bool array for position type = boundary or fluid
    cl_mem weight;                              // Weights for each distribution
    cl_mem velocity;                            // 2D Velocity vector buffer
    cl_mem density;                             // Density buffer

    //OpenCL objects
    cl_context          context;
    cl_device_id        *devices;
    cl_command_queue    commandQueue;
    cl_program program;
    cl_kernel  kernel;

    size_t kernelWorkGroupSize;         /**< Group size returned by kernel */
    size_t groupSize;                   /**< Work-Group size */
    int iterations;

    cl_bool reqdExtSupport;


    

public:

    void reset();
    cl_bool isBoundary(int x, int y);
    bool isFluid(int x, int y);
    cl_double2 getVelocity(int x, int y);
    void setSite(int x, int y, bool cellType, double u[2]);
    void setUOutput(int x, int y, double u[2]);

    // Host functions for verification
    void collide(int x, int y);
    void streamToNeighbors(int x, int y);

    /** 
    * Constructor 
    * Initialize member variables
    * @param name name of sample (string)
    */
    explicit FluidSimulation2D(std::string name)
        : SDKSample(name),
        setupTime(0),
        kernelTime(0),
        
        devices(NULL),
        maxWorkItemSizes(NULL),
        groupSize(GROUP_SIZE),
        iterations(1),
        reqdExtSupport(true)
    {
        dims[0] = LBWIDTH;
        dims[1] = LBHEIGHT;
        rho = NULL;
        u = NULL;
        h_if0 = NULL;
        h_if1234 = NULL;
        h_if1234 = NULL;
        h_of0 = NULL;
        h_of1234 = NULL;
        h_of1234 = NULL;
        v_ef0 = NULL;
        v_ef1234 = NULL;
        v_ef5678 = NULL;
        v_of0 = NULL;
        v_of1234 = NULL;
        v_of5678 = NULL;
        h_type = NULL;
        h_weight = NULL;

    }

    /** 
    * Constructor 
    * Initialize member variables
    * @param name name of sample (const char*)
    */
    explicit FluidSimulation2D(const char* name)
        : SDKSample(name),
        setupTime(0),
        kernelTime(0),
        reqdExtSupport(true),
        devices(NULL),
        maxWorkItemSizes(NULL),
        groupSize(GROUP_SIZE),
        iterations(1)
    {
        dims[0] = LBWIDTH;
        dims[1] = LBHEIGHT;
        rho = NULL;
        u = NULL;
        h_if0 = NULL;
        h_if1234 = NULL;
        h_if1234 = NULL;
        h_of0 = NULL;
        h_of1234 = NULL;
        h_of1234 = NULL;
        v_ef0 = NULL;
        v_ef1234 = NULL;
        v_ef5678 = NULL;
        v_of0 = NULL;
        v_of1234 = NULL;
        v_of5678 = NULL;
        h_type = NULL;
        h_weight = NULL;
    }

    ~FluidSimulation2D();

    /**
    * Allocate and initialize host memory array with random values
    * @return 1 on success and 0 on failure
    */
    int setupFluidSimulation2D();

    /**
     * Override from SDKSample, Generate binary image of given kernel 
     * and exit application
     */
    int genBinaryImage();

    /**
    * OpenCL related initializations. 
    * Set up Context, Device list, Command Queue, Memory buffers
    * Build CL kernel program executable
    * @return 1 on success and 0 on failure
    */
    int setupCL();

    /**
    * Set values for kernels' arguments
    * @return 1 on success and 0 on failure
    */
    int setupCLKernels();

    /**
    * Enqueue calls to the kernels
    * on to the command queue, wait till end of kernel execution.
    * Get kernel start and end time if timing is enabled
    * @return 1 on success and 0 on failure
    */
    int runCLKernels();

    /**
    * Reference CPU implementation of Binomial Option
    * for performance comparison
    */
    void CPUReference();

    /**
    * Override from SDKSample. Print sample stats.
    */
    void printStats();

    /**
    * Override from SDKSample. Initialize 
    * command line parser, add custom options
    */
    int initialize();

    /**
    * Override from SDKSample, adjust width and height 
    * of execution domain, perform all sample setup
    */
    int setup();

    /**
    * Override from SDKSample
    * Run OpenCL FluidSimulation2D
    */
    int run();

    /**
    * Override from SDKSample
    * Cleanup memory allocations
    */
    int cleanup();

    /**
    * Override from SDKSample
    * Verify against reference implementation
    */
    int verifyResults();
};

#endif // FLUID_SIMULATION2D_H_
