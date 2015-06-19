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


#include "DeviceFission.hpp"
#include <malloc.h>

static clCreateSubDevicesEXT_fn pfn_clCreateSubDevicesEXT = NULL;

int
DeviceFission::setupDeviceFission()
{
    // make sure length is multiple of group size * numSubDevices
    unsigned int mulFactor = GROUP_SIZE * numSubDevices;
    length = (length < mulFactor) ? mulFactor : length;
    length = (length / mulFactor) * mulFactor;

    input = (float*)malloc(length * sizeof(float));
    if(NULL == input)
    {
        sampleCommon->error("Failed to allocate host memory. (input)");
        return SDK_FAILURE;
    }

    // Random initialisation of input
    sampleCommon->fillRandom<cl_float>(input, length, 1, 0, 5);

    // Unless quiet mode has been enabled, print the INPUT array
    if(!quiet) 
        sampleCommon->printArray<cl_float>("Input", input, VALUES_PRINTED, 1);

    rOutput = (float*)malloc(length * sizeof(float));
    if(NULL == rOutput)
    {
        sampleCommon->error("Failed to allocate host memory. (output)");
        return SDK_FAILURE;
    }
    memset(rOutput, 0, length * sizeof(cl_float));

    subOutput = (float*)malloc(length * sizeof(float));
    if(NULL == subOutput)
    {
        sampleCommon->error("Failed to allocate host memory. (subOutput)");
        return SDK_FAILURE;
    }
    memset(subOutput, 0, length * sizeof(cl_float));

    return SDK_SUCCESS;
}

int
DeviceFission::setupCLPlatform()
{
    cl_int status = CL_SUCCESS;
    cl_device_type dType;
    
    if(deviceType.compare("cpu") == 0)
        dType = CL_DEVICE_TYPE_CPU;
    else //deviceType = "gpu" 
        dType = CL_DEVICE_TYPE_GPU;

    size_t deviceListSize;

    // Have a look at the available platforms and pick either
    // the AMD one if available or a reasonable default.
    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetPlatformIDs failed."))
        return SDK_FAILURE;

    if (0 < numPlatforms) 
    {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clGetPlatformIDs failed."))
            return SDK_FAILURE;

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
                return SDK_FAILURE;

            platform = platforms[i];
            if (!strcmp(pbuf, "Advanced Micro Devices, Inc.")) 
                break;
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


    // If we could find our platform, use it. Otherwise use just available platform.
    cl_context_properties cps[3] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform, 
        0
    };

    rContext = clCreateContextFromType(cps,
                                      dType,
                                      NULL,
                                      NULL,
                                      &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateContextFromType failed."))
        return SDK_FAILURE;

    /* First, get the size of device list data */
    status = clGetContextInfo(rContext, 
                              CL_CONTEXT_DEVICES, 
                              0, 
                              NULL, 
                              &deviceListSize);
    if(!sampleCommon->checkVal(status, 
                               CL_SUCCESS,
                               "clGetContextInfo failed."))
        return SDK_FAILURE;

    numRootDevices = (cl_uint)(deviceListSize / sizeof(cl_device_id));
    if(!sampleCommon->validateDeviceId(deviceId, (int)numRootDevices))
    {
        sampleCommon->error("sampleCommon::validateDeviceId() failed");
        return SDK_FAILURE;
    }

    // Allocate memory for device list based on the size we got earlier
    rootDevices = (cl_device_id*)malloc(deviceListSize);
    if(NULL == rootDevices) 
    {
        sampleCommon->error("Failed to allocate memory (rootDevices).");
        return SDK_FAILURE;
    }


   // Get the device list data 
    status = clGetContextInfo(rContext, 
                              CL_CONTEXT_DEVICES, 
                              deviceListSize, 
                              rootDevices, 
                              NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetContextInfo failed."))
        return SDK_FAILURE;

    // Get Device specific Information 
    status = clGetDeviceInfo(rootDevices[deviceId],
                             CL_DEVICE_MAX_WORK_GROUP_SIZE,
                             sizeof(size_t),
                             (void*)&maxWorkGroupSize,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo"
                               "CL_DEVICE_MAX_WORK_GROUP_SIZE failed."))
        return SDK_FAILURE;


    status = clGetDeviceInfo(rootDevices[deviceId],
                             CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                             sizeof(cl_uint),
                             (void*)&maxDimensions,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo"
                               "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS failed."))
        return SDK_FAILURE;

    maxWorkItemSizes = (size_t*)malloc(maxDimensions * sizeof(size_t));
    if(NULL == maxWorkItemSizes) 
    {
        sampleCommon->error("Failed to allocate memory (maxWorkItemSizes).");
        return SDK_FAILURE;
    }

    status = clGetDeviceInfo(rootDevices[deviceId],
                             CL_DEVICE_MAX_WORK_ITEM_SIZES,
                             sizeof(size_t) * maxDimensions,
                             (void*)maxWorkItemSizes,
                             NULL);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clGetDeviceInfo"
                               "CL_DEVICE_MAX_WORK_ITEM_SIZES failed."))
        return SDK_FAILURE;


    char* deviceExtensions = NULL;;
    size_t extStringSize = 0;

    // Get device extensions 
    status = clGetDeviceInfo(rootDevices[deviceId], 
        CL_DEVICE_EXTENSIONS, 
        0, 
        deviceExtensions, 
        &extStringSize);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo failed.(CL_DEVICE_EXTENSIONS)"))
        return SDK_FAILURE;
    
    deviceExtensions = new char[extStringSize];
    if(NULL == deviceExtensions)
    {
        sampleCommon->error("Failed to allocate memory(deviceExtensions)");
        return  SDK_FAILURE;
    }

    status = clGetDeviceInfo(rootDevices[deviceId], 
        CL_DEVICE_EXTENSIONS, 
        extStringSize, 
        deviceExtensions, 
        NULL);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetDeviceInfo failed.(CL_DEVICE_EXTENSIONS)"))
        return SDK_FAILURE;


    // Check if byte-addressable store is supported
    if(!strstr(deviceExtensions, "cl_ext_device_fission"))
    {
        sampleCommon->error("Device does not support device fission extension!");
        return SDK_EXPECTED_FAILURE;
    }

    delete deviceExtensions;

    // Initialize required partition property
    cl_device_partition_property_ext partitionPrty[3] =
    {
        CL_DEVICE_PARTITION_EQUALLY_EXT, 
        1, 
        CL_PROPERTIES_LIST_END_EXT
    };

    // Initialize clCreateSubDevicesEXT function pointer
    INIT_CL_EXT_FCN_PTR(clCreateSubDevicesEXT);

    // Get number of sub-devices
    status = pfn_clCreateSubDevicesEXT(rootDevices[deviceId],
                                        partitionPrty, 
                                        0, 
                                        NULL, 
                                        &numSubDevices);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clCreateSubDevicesEXT failed."))
        return SDK_FAILURE;

    subDevices = (cl_device_id*)malloc(numSubDevices * sizeof(cl_device_id));
    if(NULL == subDevices)
    {
        sampleCommon->error("Failed to allocate memory(subDevices)");
        return SDK_FAILURE;
    }

    status = pfn_clCreateSubDevicesEXT(rootDevices[deviceId],
                                        partitionPrty, 
                                        numSubDevices, 
                                        subDevices,
                                        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clCreateSubDevicesEXT failed."))
        return SDK_FAILURE;

    // Create context for sub-devices
    subContext = clCreateContext(cps,
                                 numSubDevices,
                                 subDevices,
                                 NULL,
                                 NULL,
                                 &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateContext failed."))
        return SDK_FAILURE;

     return SDK_SUCCESS;
}

int 
DeviceFission::genBinaryImage()
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

    rContext = clCreateContextFromType(cps,
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
    kernelPath.append("DeviceFission_Kernels.cl");
    if(!kernelFile.open(kernelPath.c_str()))
    {
        std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
        return SDK_FAILURE;
    }
    const char * source = kernelFile.source().c_str();
    size_t sourceSize[] = {strlen(source)};
    rProgram = clCreateProgramWithSource(rContext,
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
    status = clBuildProgram(rProgram,
                            0,
                            NULL,
                            NULL,
                            NULL,
                            NULL);

    size_t numDevices;
    status = clGetProgramInfo(rProgram, 
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
    cl_device_id* devices = (cl_device_id *)malloc( sizeof(cl_device_id) * numDevices );
    if(devices == NULL)
    {
        sampleCommon->error("Failed to allocate host memory.(devices)");
        return SDK_FAILURE;
    }
    /* grab the handles to all of the devices in the program. */
    status = clGetProgramInfo(rProgram, 
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
    
    status = clGetProgramInfo(rProgram, 
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
    status = clGetProgramInfo(rProgram, 
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

    status = clReleaseProgram(rProgram);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseProgram failed."))
    {
        return SDK_FAILURE;
    }

    status = clReleaseContext(rContext);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseContext failed."))
    {
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}


int
DeviceFission::setupCLRuntime()
{
    cl_int status = CL_SUCCESS;

    // Create command queue 
    rCmdQueue = clCreateCommandQueue(rContext,
                                     rootDevices[deviceId],
                                     0,
                                     &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateCommandQueue failed."))
        return SDK_FAILURE;

    // Create and initialize memory objects
    rInBuf = clCreateBuffer(rContext,
                            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            length * sizeof(cl_float),
                            input,
                            &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (rInBuf)"))
        return SDK_FAILURE;

    // Create memory objects for root device output 
    rOutBuf = clCreateBuffer(rContext,
                                    CL_MEM_WRITE_ONLY | 
                                    CL_MEM_USE_HOST_PTR,
                                    length * sizeof(cl_float),
                                    rOutput,
                                    &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (rOutBuf)"))
        return SDK_FAILURE;

    // create a CL program using the kernel source
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();

    char * source = NULL;
    size_t sourceSize[] = {0};
    char * binary = NULL;
    size_t binarySize = 0;

    if(isLoadBinaryEnabled())
    {
        kernelPath.append(loadBinary.c_str());
        if(!kernelFile.readBinaryFromFile(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }

        binary = (char*)kernelFile.source().c_str();
        binarySize = kernelFile.source().size();
        rProgram = clCreateProgramWithBinary(rContext,
                                            1,
                                            rootDevices, 
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
        kernelPath.append("DeviceFission_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }

        source = (char*)kernelFile.source().c_str();
        sourceSize[0] = strlen(source);
        rProgram = clCreateProgramWithSource(rContext,
                                            1,
                                            (const char**)&source,
                                            sourceSize,
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithSource failed."))
            return SDK_FAILURE;
    }
    
    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(rProgram,
                            1,
                            rootDevices,
                            NULL,
                            NULL,
                            NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char * buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo(rProgram, 
                                              rootDevices[deviceId], 
                                              CL_PROGRAM_BUILD_LOG, 
                                              buildLogSize, 
                                              buildLog, 
                                              &buildLogSize);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
                return SDK_FAILURE;

            buildLog = (char*)malloc(buildLogSize);
            if(NULL == buildLog)
            {
                sampleCommon->error("Failed to allocate host memory.(buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo(rProgram, 
                                              rootDevices[deviceId], 
                                              CL_PROGRAM_BUILD_LOG, 
                                              buildLogSize, 
                                              buildLog, 
                                              NULL);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
            {
                  free(buildLog);
                  return SDK_FAILURE;
            }

            std::cout << " \n\t\t\tBUILD LOG(ROOT DEVICE)\n";
            std::cout << " ************************************************\n";
            std::cout << buildLog << std::endl;
            std::cout << " ************************************************\n";
            free(buildLog);
        }

        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clBuildProgram failed."))
            return SDK_FAILURE;
    }

    // Get a kernel object handle for a kernel with the given name 
    rKernel = clCreateKernel(rProgram,
                            "copy",
                            &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed."))
        return SDK_FAILURE;

    // Check whether specified groupSize is plausible on current kernel
    status = clGetKernelWorkGroupInfo(rKernel,
                                      rootDevices[deviceId],
                                      CL_KERNEL_WORK_GROUP_SIZE,
                                      sizeof(size_t),
                                      &kernelWorkGroupSize,
                                      0);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clGetKernelWorkGroupInfo failed."))
        return SDK_FAILURE;

    // If groupSize exceeds the maximum supported on kernel, fall back 
    if(groupSize > kernelWorkGroupSize)
    {
        if(!quiet)
        {
            std::cout << "Out of Resources!" << std::endl;
            std::cout << "Group Size specified : " << groupSize << std::endl;
            std::cout << "Max Group Size supported on the kernel : " 
                      << kernelWorkGroupSize << std::endl;
            std::cout << "Falling back to " << kernelWorkGroupSize << std::endl;
        }
        groupSize = kernelWorkGroupSize;
    }

    //Setup sub-devices runtime resoureces

    subCmdQueue = (cl_command_queue*)malloc(numSubDevices * sizeof(cl_command_queue));
    if(NULL == subCmdQueue)
    {
        sampleCommon->error("Failed to allocate memory(subCmdQueue)");
        return SDK_FAILURE;
    }

    for(cl_uint i = 0; i < numSubDevices; i++)
    {
        // Create command queue 
        subCmdQueue[i] = clCreateCommandQueue(subContext,
                                         subDevices[i],
                                         0,
                                         &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateCommandQueue failed."))
            return SDK_FAILURE;
    }


    // Create and initialize memory objects
    
    subInBuf = (cl_mem*)malloc(numSubDevices * sizeof(cl_mem));
    if(NULL == subInBuf)
    {
        sampleCommon->error("Failed to allocate memory(subInBuf)");
        return SDK_FAILURE;
    }

    for(cl_uint i = 0; i < numSubDevices; i++)
    {
        subInBuf[i] = clCreateBuffer(subContext,
                                  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  (length / numSubDevices) * sizeof(cl_float),
                                  &input[(length / numSubDevices) * i],
                                  &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateBuffer failed. (rInBuf)"))
            return SDK_FAILURE;
    }

    // Create memory objects for root device output 
    subOutBuf = clCreateBuffer(subContext,
                               CL_MEM_WRITE_ONLY,
                               (length / numSubDevices) * sizeof(cl_float) ,
                               NULL,
                               &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (rOutBuf)"))
        return SDK_FAILURE;

    if(isLoadBinaryEnabled())
    {
        if(!kernelFile.readBinaryFromFile(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }

        // Get binaries and binary sizes for all devices
        char** subBinaries = (char**)malloc(numSubDevices * sizeof(char*));
        if(subBinaries == NULL)
        {
            sampleCommon->error("Failed to allocate memory(subBinaries)");
            return SDK_FAILURE;
        }

        size_t* subBinariesSize = (size_t*)malloc(numSubDevices * sizeof(size_t*));
        if(subBinariesSize == NULL)
        {
            sampleCommon->error("Failed to allocate memory(subBinariesSize)");
            return SDK_FAILURE;
        }

        for(cl_uint i = 0; i < numSubDevices; ++i)
        {
            subBinaries[i] = (char*)kernelFile.source().c_str();
            subBinariesSize[i] = kernelFile.source().size();
        }
        
        subProgram = clCreateProgramWithBinary(subContext,
                                            numSubDevices,
                                            subDevices, 
                                            (const size_t *)subBinariesSize,
                                            (const unsigned char**)subBinaries,
                                            NULL,
                                            &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithBinary failed."))
        {
            return SDK_FAILURE;
        }

        free(subBinaries);
        free(subBinariesSize);
        subBinariesSize = NULL;
        subBinaries = NULL;

    }
    else
    {
        // create a CL program using the kernel source
        subProgram = clCreateProgramWithSource(subContext,
                                                1,
                                                (const char**)&source,
                                                sourceSize,
                                                &status);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clCreateProgramWithSource failed."))
            return SDK_FAILURE;
    }
    
    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(subProgram,
                            numSubDevices,
                            subDevices,
                            NULL,
                            NULL,
                            NULL);
    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_int logStatus;
            char * buildLog = NULL;
            size_t buildLogSize = 0;
            logStatus = clGetProgramBuildInfo(subProgram, 
                                              subDevices[0], 
                                              CL_PROGRAM_BUILD_LOG, 
                                              buildLogSize, 
                                              buildLog, 
                                              &buildLogSize);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
                return SDK_FAILURE;

            buildLog = (char*)malloc(buildLogSize);
            if(NULL == buildLog)
            {
                sampleCommon->error("Failed to allocate host memory.(buildLog)");
                return SDK_FAILURE;
            }
            memset(buildLog, 0, buildLogSize);

            logStatus = clGetProgramBuildInfo(subProgram, 
                                              subDevices[0], 
                                              CL_PROGRAM_BUILD_LOG, 
                                              buildLogSize, 
                                              buildLog, 
                                              NULL);
            if(!sampleCommon->checkVal(logStatus,
                                       CL_SUCCESS,
                                       "clGetProgramBuildInfo failed."))
            {
                  free(buildLog);
                  return SDK_FAILURE;
            }

            std::cout << " \n\t\t\tBUILD LOG(SUB-DEVICES)\n";
            std::cout << " ************************************************\n";
            std::cout << buildLog << std::endl;
            std::cout << " ************************************************\n";
            free(buildLog);
        }

        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clBuildProgram failed."))
            return SDK_FAILURE;
    }

    // Get a kernel object handle for a kernel with the given name 
    subKernel = clCreateKernel(subProgram,
                               "copy",
                               &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed."))
        return SDK_FAILURE;


    return SDK_SUCCESS;
}


int
DeviceFission::runCLRootDeviceKerenls()
{
    cl_int status;
    cl_event events[1];

    // Set appropriate arguments to the kernel 
    
    // Set input 
    status = clSetKernelArg(rKernel, 0, sizeof(cl_mem),(void*)&rInBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clSetKernelArg failed. (rInBuf)"))
        return SDK_FAILURE;

    // Set rOutBuf as second argument 
    status = clSetKernelArg(rKernel, 1, sizeof(cl_mem), (void*)&rOutBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (rOutBuf)"))
        return SDK_FAILURE;


    // Set global and local work items
    size_t globalThreads[] = {length};
    size_t localThreads[] = {groupSize};

    // Enqueue kernel
    status = clEnqueueNDRangeKernel(rCmdQueue,
                                    rKernel,
                                    1,
                                    NULL,
                                    globalThreads,
                                    localThreads,
                                    0,
                                    NULL,
                                    NULL);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clEnqueueNDRangeKernel failed."))
        return SDK_FAILURE;

    status = clFinish(rCmdQueue);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clFinish failed."))
        return SDK_FAILURE;

    /* Enqueue readBuffer*/
    status = clEnqueueReadBuffer(rCmdQueue,
                                 rOutBuf,
                                 CL_TRUE,
                                 0,
                                 length * sizeof(cl_float),
                                 rOutput,
                                 0,
                                 NULL,
                                 &events[0]);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clEnqueueReadBuffer failed."))
        return SDK_FAILURE;
    
    // Wait for the read buffer to finish execution */
    status = clWaitForEvents(1, &events[0]);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clWaitForEvents failed."))
        return SDK_FAILURE;

    clReleaseEvent(events[0]);

    return SDK_SUCCESS;
}

int
DeviceFission::runCLSubDeviceKerenls()
{
    cl_int status;
    cl_event events[1];

    // Set appropriate arguments to the kernel 
    
    // Set subOutBuf as second argument 
    status = clSetKernelArg(subKernel, 1, sizeof(cl_mem), (void*)&subOutBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (subOutBuf)"))
        return SDK_FAILURE;


    // Set global and local work items
    size_t globalThreads[] = {length / numSubDevices};
    size_t localThreads[] = {groupSize};

    for(cl_uint i = 0; i < numSubDevices; ++i)
    {
        // Set input 
        status = clSetKernelArg(subKernel, 0, sizeof(cl_mem),(void*)&subInBuf[i]);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS, 
                                   "clSetKernelArg failed. (subInBuf)"))
            return SDK_FAILURE;

        // Enqueue kernel
        status = clEnqueueNDRangeKernel(subCmdQueue[i],
                                        subKernel,
                                        1,
                                        NULL,
                                        globalThreads,
                                        localThreads,
                                        0,
                                        NULL,
                                        NULL);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS, 
                                   "clEnqueueNDRangeKernel failed."))
            return SDK_FAILURE;

        status = clFinish(subCmdQueue[i]);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS, 
                                   "clFinish failed."))
            return SDK_FAILURE;

        /* Enqueue readBuffer*/
        status = clEnqueueReadBuffer(subCmdQueue[i],
                                     subOutBuf,
                                     CL_TRUE,
                                     0,
                                     (length / numSubDevices) * sizeof(cl_float),
                                     &subOutput[(length / numSubDevices) * i],
                                     0,
                                     NULL,
                                     &events[0]);

        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clEnqueueReadBuffer failed."))
            return SDK_FAILURE;
        
        // Wait for the read buffer to finish execution */
        status = clWaitForEvents(1, &events[0]);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clWaitForEvents failed."))
            return SDK_FAILURE;

        clReleaseEvent(events[0]);
    }

    return SDK_SUCCESS;
}

int 
DeviceFission::runCLKernels()
{
    // Run kernel on root device
    if(runCLRootDeviceKerenls() != CL_SUCCESS)
        return SDK_FAILURE;

    // Run kernsl on all sub-devices
    if(runCLSubDeviceKerenls() != CL_SUCCESS)
        return SDK_FAILURE;

    return SDK_SUCCESS;
}

int DeviceFission::initialize()
{
    // Call base class Initialize to get default configuration
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    // Now add customized options
    streamsdk::Option* array_length = new streamsdk::Option;
    if(NULL == array_length)
    {
        sampleCommon->error("Memory allocation error.\n");
        return SDK_FAILURE;
    }
    
    array_length->_sVersion = "x";
    array_length->_lVersion = "length";
    array_length->_description = "Length of the Input array";
    array_length->_type = streamsdk::CA_ARG_INT;
    array_length->_value = &length;
    sampleArgs->AddOption(array_length);
    delete array_length;

    return SDK_SUCCESS;
}

int 
DeviceFission::setup()
{
    cl_int retValue = setupCLPlatform();
    if(retValue != SDK_SUCCESS)
        return (retValue == SDK_EXPECTED_FAILURE)? SDK_EXPECTED_FAILURE:SDK_FAILURE;

    if(setupDeviceFission() != SDK_SUCCESS)
        return SDK_FAILURE;

    if(setupCLRuntime() != SDK_SUCCESS)
        return SDK_FAILURE;

    return SDK_SUCCESS;
}


int DeviceFission::run()
{
    // Execute kernel on device
    if(runCLKernels() != SDK_SUCCESS)
        return SDK_FAILURE;

    if(!quiet)
        sampleCommon->printArray<cl_float>("Output", rOutput, VALUES_PRINTED, 1);

    return SDK_SUCCESS;
}

int DeviceFission::verifyResults()
{
    if(verify)
    {
        bool rootDeviceRlt = sampleCommon->compare(input, rOutput, length);
        bool subDevicesRlt =sampleCommon->compare(input, subOutput, length);

        if(rootDeviceRlt && subDevicesRlt)
        {
            std::cout << "Passed!\n";
            return SDK_SUCCESS;
        }
        else
        {
            std::cout << "Failed\n";
            return SDK_FAILURE;
        }
    }

    return SDK_SUCCESS;
}

int
DeviceFission::cleanup()
{

    // Releases all OpenCL resources of root device 
    cl_int status;

    status = clReleaseKernel(rKernel);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseKernel failed."))
        return SDK_FAILURE;

    status = clReleaseProgram(rProgram);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseProgram failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(rInBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(rOutBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseCommandQueue(rCmdQueue);
     if(!sampleCommon->checkVal(status,
                                CL_SUCCESS,
                                "clReleaseCommandQueue failed."))
        return SDK_FAILURE;

    status = clReleaseContext(rContext);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseContext failed."))
        return SDK_FAILURE;


    // Release all OpenCL resources for sub-devices
    status = clReleaseKernel(subKernel);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseKernel failed."))
        return SDK_FAILURE;

    status = clReleaseProgram(subProgram);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseProgram failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(subOutBuf);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseMemObject failed."))
        return SDK_FAILURE;

    for(cl_uint i = 0; i < numSubDevices; ++i)
    {
        status = clReleaseMemObject(subInBuf[i]);
        if(!sampleCommon->checkVal(status,
                                   CL_SUCCESS,
                                   "clReleaseMemObject failed."))
            return SDK_FAILURE;

        status = clReleaseCommandQueue(subCmdQueue[i]);
         if(!sampleCommon->checkVal(status,
                                    CL_SUCCESS,
                                    "clReleaseCommandQueue failed."))
            return SDK_FAILURE;
    }

    status = clReleaseContext(subContext);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clReleaseContext failed."))
        return SDK_FAILURE;


    return SDK_SUCCESS;
}

DeviceFission::~DeviceFission()
{
    /* release program resources (input memory etc.) */
    if(input) 
    {
        free(input);
        input = NULL;
    }

    if(rOutput) 
    {
        free(rOutput);
        rOutput = NULL;
    }

    if(subOutput) 
    {
        free(subOutput);
        subOutput = NULL;
    }

    if(rootDevices)
    {
        free(rootDevices);
        rootDevices = NULL;
    }

    if(subDevices)
    {
        free(subDevices);
        subDevices = NULL;
    }


    if(subCmdQueue)
    {
        free(subCmdQueue);
        subCmdQueue = NULL;
    }

    if(subInBuf)
    {
        free(subInBuf);
        subInBuf = NULL;
    }

    if(maxWorkItemSizes)
    {
        free(maxWorkItemSizes);
        maxWorkItemSizes = NULL;
    }
}

int 
main(int argc, char * argv[])
{
    DeviceFission clDeviceFission("OpenCL DeviceFission");

    clDeviceFission.initialize();
    if(!clDeviceFission.parseCommandLine(argc, argv))
        return SDK_FAILURE;

    if(clDeviceFission.isDumpBinaryEnabled())
    {
        return clDeviceFission.genBinaryImage();
    }
    else
    {
        cl_int retValue = clDeviceFission.setup();
        if(retValue != SDK_SUCCESS)
            return (retValue == SDK_EXPECTED_FAILURE)? SDK_SUCCESS: SDK_FAILURE;

        if(clDeviceFission.run() != SDK_SUCCESS)
            return SDK_FAILURE;

        if(clDeviceFission.verifyResults() != SDK_SUCCESS)
            return SDK_FAILURE;

        if(clDeviceFission.cleanup() != SDK_SUCCESS)
            return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}
