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


#include "MemoryOptimizations.hpp"

//Separator
std::string sep = "-----------------------------------------";
int Iterations = 1000;
int 
MemoryOptimizations::setupMemoryOptimizations()
{
    /* Allocate memory required foratmost vec4 elements */
    cl_uint size = length * sizeof(cl_float4);

    input = (cl_float*)malloc(size);
    if(input == NULL)
    { 
        sampleCommon->error("Failed to allocate host memory. (input)");
        return SDK_FAILURE;
    }

    output = (cl_float*)malloc(size);
    if(output == NULL)
    { 
        sampleCommon->error("Failed to allocate host memory. (output)");
        return SDK_FAILURE;
    }
    
    /* random initialisation of input */
    sampleCommon->fillRandom<cl_float>(input,  4 * length, 1, 0, 10);

    return SDK_SUCCESS;
}

int 
MemoryOptimizations::genBinaryImage()
{
    cl_int status = CL_SUCCESS;

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */
    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
    {
        return SDK_FAILURE;
    }
    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
        {
            return SDK_FAILURE;
        }

        char platformName[100];
        for (unsigned i = 0; i < numPlatforms; ++i) 
        {
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(platformName),
                                       platformName,
                                       NULL);

            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetPlatformInfo failed."))
            {
                return SDK_FAILURE;
            }

            platform = platforms[i];
            if (!strcmp(platformName, "Advanced Micro Devices, Inc.")) 
            {
                break;
            }
        }
        std::cout << "Platform found : " << platformName << "\n";
        delete[] platforms;
    }

    if(NULL == platform)
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    /*
     * If we could find our platform, use it. Otherwise use just available platform.
     */
    cl_context_properties cps[5] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        CL_CONTEXT_OFFLINE_DEVICES_AMD,
        (cl_context_properties)1,
        0
    };

    context = clCreateContextFromType(cps,
                                      CL_DEVICE_TYPE_ALL,
                                      NULL,
                                      NULL,
                                      &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateContextFromType failed."))
    {
        return SDK_FAILURE;
    }

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();
    kernelPath.append("MemoryOptimizations_Kernels.cl");
    if(!kernelFile.open(kernelPath.c_str()))
    {
        std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
        return SDK_FAILURE;
    }
    const char * source = kernelFile.source().c_str();
    size_t sourceSize[] = {strlen(source)};
    program = clCreateProgramWithSource(context,
                                        1,
                                        &source,
                                        sourceSize,
                                        &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateProgramWithSource failed."))
    {
        return SDK_FAILURE;
    }
    
    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program,
                            0,
                            NULL,
                            NULL,
                            NULL,
                            NULL);

    size_t numDevices;
    status = clGetProgramInfo(program, 
                           CL_PROGRAM_NUM_DEVICES,
                           sizeof(numDevices),
                           &numDevices,
                           NULL );
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_NUM_DEVICES) failed."))
    {
        return SDK_FAILURE;
    }

    std::cout << "Number of devices found : " << numDevices << "\n\n";
    devices = (cl_device_id *)malloc( sizeof(cl_device_id) * numDevices );
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(devices)");
        return SDK_FAILURE;
    }
    /* grab the handles to all of the devices in the program. */
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_DEVICES, 
                              sizeof(cl_device_id) * numDevices,
                              devices,
                              NULL );
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_DEVICES) failed."))
    {
        return SDK_FAILURE;
    }


    /* figure out the sizes of each of the binaries. */
    size_t *binarySizes = (size_t*)malloc( sizeof(size_t) * numDevices );
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(binarySizes)");
        return SDK_FAILURE;
    }
    
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_BINARY_SIZES,
                              sizeof(size_t) * numDevices, 
                              binarySizes, NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_BINARY_SIZES) failed."))
    {
        return SDK_FAILURE;
    }

    size_t i = 0;
    /* copy over all of the generated binaries. */
    char **binaries = (char **)malloc( sizeof(char *) * numDevices );
    if(binaries == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(binaries)");
        return SDK_FAILURE;
    }

    for(i = 0; i < numDevices; i++)
    {
        if(binarySizes[i] != 0)
        {
            binaries[i] = (char *)malloc( sizeof(char) * binarySizes[i]);
            if(binaries[i] == NULL)
            {
                sampleCommon->error("Failed to allocate host memory.(binaries[i])");
                return SDK_FAILURE;
            }
        }
        else
        {
            binaries[i] = NULL;
        }
    }
    status = clGetProgramInfo(program, 
                              CL_PROGRAM_BINARIES,
                              sizeof(char *) * numDevices, 
                              binaries, 
                              NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetProgramInfo(CL_PROGRAM_BINARIES) failed."))
    {
        return SDK_FAILURE;
    }

    /* dump out each binary into its own separate file. */
    for(i = 0; i < numDevices; i++)
    {
        char fileName[100];
        sprintf(fileName, "%s.%d", dumpBinary.c_str(), (int)i);
        if(binarySizes[i] != 0)
        {
            char deviceName[1024];
            status = clGetDeviceInfo(devices[i], 
                                     CL_DEVICE_NAME, 
                                     sizeof(deviceName),
                                     deviceName, 
                                     NULL);
            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetDeviceInfo(CL_DEVICE_NAME) failed."))
            {
                return SDK_FAILURE;
            }

            printf( "%s binary kernel: %s\n", deviceName, fileName);
            streamsdk::SDKFile BinaryFile;
            if(!BinaryFile.writeBinaryToFile(fileName, 
                                             binaries[i], 
                                             binarySizes[i]))
            {
                std::cout << "Failed to load kernel file : " << fileName << std::endl;
                return SDK_FAILURE;
            }
        }
        else
        {
            printf("Skipping %s since there is no binary data to write!\n",
                    fileName);
        }
    }

    // Release all resouces and memory
    for(i = 0; i < numDevices; i++)
    {
        if(binaries[i] != NULL)
        {
            free(binaries[i]);
            binaries[i] = NULL;
        }
    }

    if(binaries != NULL)
    {
        free(binaries);
        binaries = NULL;
    }

    if(binarySizes != NULL)
    {
        free(binarySizes);
        binarySizes = NULL;
    }

    if(devices != NULL)
    {
        free(devices);
        devices = NULL;
    }

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseProgram failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseContext(context);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseContext failed."))
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}


int
MemoryOptimizations::setupCL(void)
{
    cl_int status = 0;
    size_t deviceListSize;

    cl_device_type dType;
    
    if(deviceType.compare("cpu") == 0)
    {
        dType = CL_DEVICE_TYPE_CPU;
    }
    else //deviceType = "gpu" 
    {
        dType = CL_DEVICE_TYPE_GPU;
    }

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */

    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
    {
        return SDK_FAILURE;
    }
    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
        {
            return SDK_FAILURE;
        }
        for (unsigned i = 0; i < numPlatforms; ++i) 
        {
            char pbuf[100];
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(pbuf),
                                       pbuf,
                                       NULL);

            if(!sampleCommon->checkVal(status,
                                       CL_SUCCESS,
                                       "clGetPlatformInfo failed."))
            {
                return SDK_FAILURE;
            }

            platform = platforms[i];
            if (!strcmp(pbuf, "Advanced Micro Devices, Inc.")) 
            {
                break;
            }
        }
        delete[] platforms;
    }

    if(NULL == platform)
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    // Display available devices.
    if(!sampleCommon->displayDevices(platform, dType))
    {
        sampleCommon->error("sampleCommon::displayDevices() failed");
        return SDK_FAILURE;
    }

    /*
     * If we could find our platform, use it. Otherwise use just available platform.
     */

    cl_context_properties cps[3] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        0
    };

    context = clCreateContextFromType(cps,
                                      dType,
                                      NULL,
                                      NULL,
                                      &status);
    if(!sampleCommon->checkVal(status, 
                               CL_SUCCESS,
                               "clCreateContextFromType failed."))
        return SDK_FAILURE;

    /* First, get the size of device list data */
    status = clGetContextInfo(context, 
                              CL_CONTEXT_DEVICES, 
                              0, 
                              NULL, 
                              &deviceListSize);
    if(!sampleCommon->checkVal(status, 
                               CL_SUCCESS,
                               "clGetContextInfo failed."))
        return SDK_FAILURE;

    int deviceCount = (int)(deviceListSize / sizeof(cl_device_id));
    if(!sampleCommon->validateDeviceId(deviceId, deviceCount))
    {
        sampleCommon->error("sampleCommon::validateDeviceId() failed");
        return SDK_FAILURE;
    }

    /* Now allocate memory for device list based on the size we got earlier */
    devices = (cl_device_id*)malloc(deviceListSize);
    if(devices == NULL) 
    {
        sampleCommon->error("Failed to allocate memory (devices).");
        return SDK_FAILURE;
    }

    /* Now, get the device list data */
    status = clGetContextInfo(context, 
                              CL_CONTEXT_DEVICES, 
                              deviceListSize, 
                              devices, 
                              NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetGetContextInfo failed."))
        return SDK_FAILURE;


    /* Get Device specific Information */
    /* Get device extensions */
    char deviceExtensions[2048];
    status = clGetDeviceInfo(devices[deviceId], 
                             CL_DEVICE_EXTENSIONS, 
                             sizeof(deviceExtensions), 
                             deviceExtensions, 
                             0);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo failed.(extensions)"))
        return SDK_FAILURE;

    if(!strstr(deviceExtensions, "cl_khr_global_int32_base_atomics"))
    {
        sampleCommon->error("Device does not support global_int32_base_atomics!");
        return SDK_EXPECTED_FAILURE;
    }

    status = clGetDeviceInfo(devices[deviceId],
                             CL_DEVICE_MAX_WORK_GROUP_SIZE,
                             sizeof(size_t),
                             (void *)&maxWorkGroupSize,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE failed."))
        return SDK_FAILURE;


    status = clGetDeviceInfo(devices[deviceId],
                             CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                             sizeof(cl_uint),
                             (void *)&maxDimensions,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS failed."))
        return SDK_FAILURE;


    maxWorkItemSizes = (size_t*)malloc(maxDimensions*sizeof(size_t));
    status = clGetDeviceInfo(devices[deviceId],
                             CL_DEVICE_MAX_WORK_ITEM_SIZES,
                             sizeof(size_t)*maxDimensions,
                             (void *)maxWorkItemSizes,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_SIZES failed."))
        return SDK_FAILURE;




    {
        /* The block is to move the declaration of prop closer to its use */
        cl_command_queue_properties prop = 0;
        prop |= CL_QUEUE_PROFILING_ENABLE;

        commandQueue = clCreateCommandQueue(context, 
                                            devices[deviceId], 
                                            prop, 
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   0,
                                   "clCreateCommandQueue failed."))
            return SDK_FAILURE;
    }

    /* Input buffer */
    inputBuffer = clCreateBuffer(context, 
                                 CL_MEM_READ_ONLY,
                                 sizeof(cl_float4) * length,
                                 0, 
                                 &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (inputBuffer)"))
        return SDK_FAILURE;

    /* Write data to buffer */
    status = clEnqueueWriteBuffer(commandQueue,
                                  inputBuffer,
                                  1,
                                  0,
                                  sizeof(cl_float4) * length,
                                  input,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clEnqueueWriteBuffer failed. (inputBuffer)"))
        return SDK_FAILURE;


    outputBuffer = clCreateBuffer(context, 
                                  CL_MEM_WRITE_ONLY,
                                  sizeof(cl_float4) * length,
                                  0, 
                                  &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (outputBuffer)"))
        return SDK_FAILURE;

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();

    if(isLoadBinaryEnabled())
    {
        kernelPath.append(loadBinary.c_str());
        if(!kernelFile.readBinaryFromFile(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }

        const char * binary = kernelFile.source().c_str();
        size_t binarySize = kernelFile.source().size();
        program = clCreateProgramWithBinary(context,
                                            1,
                                            &devices[deviceId], 
                                            (const size_t *)&binarySize,
                                            (const unsigned char**)&binary,
                                            NULL,
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithBinary failed."))
        {
            return SDK_FAILURE;
        }

    }
    else
    {
        kernelPath.append("MemoryOptimizations_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file: " << kernelPath << std::endl;
            return SDK_FAILURE;
        }
        const char * source = kernelFile.source().c_str();
        size_t sourceSize[] = {strlen(source)};
        program = clCreateProgramWithSource(context,
                                            1,
                                            &source,
                                            sourceSize,
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithSource failed."))
            return SDK_FAILURE;
    }

    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(program, 1, &devices[deviceId], NULL, NULL, NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char *buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo (program, 
                devices[deviceId], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                &buildLogSize);
            if(!sampleCommon->checkVal(
                logStatus,
                CL_SUCCESS,
                "clGetProgramBuildInfo failed."))
                return SDK_FAILURE;

            buildLog = (char*)malloc(buildLogSize);
            if(buildLog == NULL)
            {
                sampleCommon->error("Failed to allocate host memory. (buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo (program, 
                devices[deviceId], 
                CL_PROGRAM_BUILD_LOG, 
                buildLogSize, 
                buildLog, 
                NULL);
            if(!sampleCommon->checkVal(
                logStatus,
                CL_SUCCESS,
                "clGetProgramBuildInfo failed."))
            {
                free(buildLog);
                return SDK_FAILURE;
            }

            std::cout << " \n\t\t\tBUILD LOG\n";
            std::cout << " ************************************************\n";
            std::cout << buildLog << std::endl;
            std::cout << " ************************************************\n";
            free(buildLog);
        }

        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clBuildProgram failed."))
            return SDK_FAILURE;
    }

    /* Copy 1D Fast Path */
    kernel[0] = clCreateKernel(program, "copy1DFastPath", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed.(copy1DFastPath)"))
        return SDK_FAILURE;

    /* Copy 1D Complete Path */
    kernel[1] = clCreateKernel(program, "copy1DCompletePath", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (copy1DCompletePath)"))
        return SDK_FAILURE;

    /* Copy 2D float */
    kernel[2] = clCreateKernel(program, "copy2Dfloat", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (copy2Dfloat)"))
        return SDK_FAILURE;

    /* Copy 2D float4 */
    kernel[3] = clCreateKernel(program, "copy2Dfloat4", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (copy2Dfloat4)"))
        return SDK_FAILURE;

    /* Copy 1D float4 */
    kernel[4] = clCreateKernel(program, "copy1Dfloat4", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (copy1Dfloat4)"))
        return SDK_FAILURE;

    /* Copy No Coalesced */
    kernel[5] = clCreateKernel(program, "NoCoal", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (NoCoal)"))
        return SDK_FAILURE;

    /* Copy Split */
    kernel[6] = clCreateKernel(program, "Split", &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateKernel failed. (Split)"))
        return SDK_FAILURE;

    return SDK_SUCCESS;
}

int
MemoryOptimizations::copy(cl_kernel& kernel, int vectorSize)
{
    cl_int status;
    cl_event events[2];

    /* Check group size against kernelWorkGroupSize */
    status = clGetKernelWorkGroupInfo(kernel,
                                      devices[deviceId],
                                      CL_KERNEL_WORK_GROUP_SIZE,
                                      sizeof(size_t),
                                      &kernelWorkGroupSize,
                                      0);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetKernelWorkGroupInfo failed."))
    {
        return SDK_FAILURE;
    }

    if(localThreads[0] * localThreads[1] > kernelWorkGroupSize)
    {
        std::cout << "\nDevice doesn't support required work-group size!\n";
        return SDK_SUCCESS;
    }

    /*** Set appropriate arguments to the kernel ***/
    
    status = clSetKernelArg(kernel,
                            0,
                            sizeof(cl_mem), 
                            (void *)&inputBuffer);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed.(inputBuffer)"))
        return SDK_FAILURE;

    status = clSetKernelArg(kernel,
                            1,
                            sizeof(cl_mem), 
                            (void *)&outputBuffer);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed.(outputBuffer)"))
        return SDK_FAILURE;

    double nsec = 0;

    // Reduce the iterations if verification is enabled.
    if(verify)
        Iterations = 1;

    /* Run the kernel for a number of iterations */
    for(int i = 0; i < Iterations; i++)
    {

        /*Enqueue a kernel run call */
        status = clEnqueueNDRangeKernel(commandQueue,
                                        kernel,
                                        2,
                                        NULL,
                                        globalThreads,
                                        localThreads,
                                        0,
                                        NULL,
                                        &events[0]);
        
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clEnqueueNDRangeKernel failed."))
            return SDK_FAILURE;


        /* wait for the kernel call to finish execution */
        status = clWaitForEvents(1, &events[0]);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clWaitForEvents failed."))
            return SDK_FAILURE;

        /* Calculate performance */
        cl_ulong startTime;
        cl_ulong endTime;
        
        /* Get kernel profiling info */
        status = clGetEventProfilingInfo(events[0],
                                         CL_PROFILING_COMMAND_START,
                                         sizeof(cl_ulong),
                                         &startTime,
                                         0);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetEventProfilingInfo failed.(startTime)"))
            return SDK_FAILURE;


        status = clGetEventProfilingInfo(events[0],
                                         CL_PROFILING_COMMAND_END,
                                         sizeof(cl_ulong),
                                         &endTime,
                                         0);

        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetEventProfilingInfo failed.(endTime)"))
            return SDK_FAILURE;

        /* Cumulate time for each iteration */
        nsec += endTime - startTime;
    }

    /* Copy bytes */
    int numThreads = (int)(globalThreads[0] * globalThreads[1]);
    double bytes = (double)(Iterations * 2 * vectorSize * sizeof(cl_float));
    double perf = (bytes / nsec) * numThreads;
    
    std::cout << ": " << perf << " GB/s" << std::endl;

    if(verify)
    {
        /* Enqueue readBuffer*/
        status = clEnqueueReadBuffer(commandQueue,
                                     outputBuffer,
                                     CL_TRUE,
                                     0,
                                     length * sizeof(cl_float4),
                                     output,
                                     0,
                                     NULL,
                                     0);

        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clEnqueueReadBuffer failed."))
            return SDK_FAILURE;
        
        /* Verify data */
        if(!memcmp(input, output, vectorSize * sizeof(cl_float) * length))
        {
            std::cout << "Passed!\n";
            return SDK_SUCCESS;
        }
        else
        {
            std::cout << "Failed!\n";
            return SDK_FAILURE;
        }
    }

    return SDK_SUCCESS;
}

int 
MemoryOptimizations::runCLKernels(void)
{
    int status;

    globalThreads[0] = length;
    globalThreads[1] = 1;

    localThreads[0] = GROUP_SIZE;
    localThreads[1] = 1;

    std::cout << sep << "\nCopy 1D FastPath\t";

    status = copy(kernel[0], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 1D CompletePath\t";

    status = copy(kernel[1], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (64x2)\t";

    globalThreads[0] = 1024;
    globalThreads[1] = 1024;

    localThreads[0] = 64;
    localThreads[1] = 2;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (64x2)\t";
    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (64x4)\t";

    localThreads[0] = 64;
    localThreads[1] = 4;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (64x4)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (8x8)\t";

    localThreads[0] = 8;
    localThreads[1] = 8;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (8x8)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (256x1)\t";

    localThreads[0] = 256;
    localThreads[1] = 1;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (256x1)\t";
    copy(kernel[3], 4);

    std::cout << sep << "\nCopy 2D 32-bit (32x2)\t";

    localThreads[0] = 32;
    localThreads[1] = 2;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (32x2)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (64x1)\t";

    localThreads[0] = 64;
    localThreads[1] = 1;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (64x1)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (16x16)\t";

    localThreads[0] = 16;
    localThreads[1] = 16;
    
    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (16x16)\t";
    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (16x4)\t";

    localThreads[0] = 16;
    localThreads[1] = 4;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (16x4)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 2D 32-bit (1x64)\t";

    localThreads[0] = 1;
    localThreads[1] = 64;

    status = copy(kernel[2], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << "Copy 2D 128-bit (1x64)\t";

    status = copy(kernel[3], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nCopy 1D 128-bit \t";

    localThreads[0] = length;
    localThreads[1] = 1;

    localThreads[0] = 64;
    localThreads[1] = 1;

    status = copy(kernel[4], 4);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nNoCoal Copy 1D 32-bit \t";

    status = copy(kernel[5], 1);
    if(status != SDK_SUCCESS)
        return status;

    std::cout << sep << "\nSplit Copy 1D 32-bit \t";
    status = copy(kernel[6], 1);
    if(status != SDK_SUCCESS)
        return status;

    return SDK_SUCCESS;
}


int 
MemoryOptimizations::initialize()
{
    /* Call base class Initialize to get default configuration */
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    return SDK_SUCCESS;
}

int 
MemoryOptimizations::setup()
{
    if(setupMemoryOptimizations()!=SDK_SUCCESS)
        return SDK_FAILURE;

    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    int status = setupCL();
    if(status != SDK_SUCCESS)
        return status;

    sampleCommon->stopTimer(timer);

    setupTime = (cl_double)sampleCommon->readTimer(timer);

    return SDK_SUCCESS;
}


int 
MemoryOptimizations::run()
{
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);   

    /* Arguments are set and execution call is enqueued on command buffer */
    int kernelRun = runCLKernels();
    if(kernelRun != SDK_SUCCESS)
    {
        return kernelRun;
    }

    sampleCommon->stopTimer(timer);
    totalKernelTime = (double)(sampleCommon->readTimer(timer));

    return SDK_SUCCESS;
}

int 
MemoryOptimizations::verifyResults()
{
    return SDK_SUCCESS;
}


int 
MemoryOptimizations::cleanup()
{
    /* Releases OpenCL resources (Context, Memory etc.) */
    cl_int status;

    for(int i = 0; i < NUM_KERNELS; i++)
    {
        status = clReleaseKernel(kernel[i]);
        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clReleaseKernel failed."))
            return SDK_FAILURE;
    }

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseProgram failed."))
        return SDK_FAILURE;
 
    status = clReleaseMemObject(inputBuffer);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(outputBuffer);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseCommandQueue(commandQueue);
     if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseCommandQueue failed."))
        return SDK_FAILURE;

    status = clReleaseContext(context);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseContext failed."))
        return SDK_FAILURE;

    /* release program resources (input memory etc.) */
    if(input) 
        free(input);

    if(output)
        free(output);

    /* release device list */
    if(devices)
        free(devices);

    if(maxWorkItemSizes)
        free(maxWorkItemSizes);

    return SDK_SUCCESS;
}

int 
main(int argc, char * argv[])
{
    MemoryOptimizations clMemoryOptimizations("OpenCL Matrix Multiplication");

    clMemoryOptimizations.initialize();
    if(!clMemoryOptimizations.parseCommandLine(argc, argv))
        return SDK_FAILURE;

    if(clMemoryOptimizations.isDumpBinaryEnabled())
    {
        return clMemoryOptimizations.genBinaryImage();
    }
    else
    {
        int state = clMemoryOptimizations.setup();
        if(state != SDK_SUCCESS)
        {
            if(state == SDK_EXPECTED_FAILURE)
                return SDK_SUCCESS;
            else
                return SDK_FAILURE;
        }

        state = clMemoryOptimizations.run();

        if(state == SDK_FAILURE)
          return SDK_FAILURE;
        else
        {
            if(clMemoryOptimizations.verifyResults()==SDK_FAILURE)
                return SDK_FAILURE;
        }
        if(clMemoryOptimizations.cleanup()==SDK_FAILURE)
          return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}
