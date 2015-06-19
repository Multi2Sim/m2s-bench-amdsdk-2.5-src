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


#include "BoxFilterGLSAT.hpp"
#include <cmath>

#ifndef _WIN32
#include <GL/glx.h>
#endif //!_WIN32


int
BoxFilterGLSAT::readInputImage(std::string inputImageName)
{
    /* load input bitmap image */
    std::string filePath = sampleCommon->getPath() + std::string(INPUT_IMAGE);
    inputBitmap.load(filePath.c_str());

    /* error if image did not load */
    if(!inputBitmap.isLoaded())
    {
        sampleCommon->error("Failed to load input image!");
        return SDK_FAILURE;
    }

    /* get width and height of input image */
    height = inputBitmap.getHeight();
    width = inputBitmap.getWidth();

    /* allocate memory for input & output image data  */
    inputImageData  = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));

    /* error check */
    if(inputImageData == NULL)
    {
        sampleCommon->error("Failed to allocate memory! (inputImageData)");
        return SDK_FAILURE;
    }

    /* allocate memory for output image data */
    outputImageData = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));


    /* error check */
    if(outputImageData == NULL)
    {
        sampleCommon->error("Failed to allocate memory! (outputImageData)");
        return SDK_FAILURE;
    }

    /* initializa the Image data to NULL */
    memset(outputImageData, 0, width * height * pixelSize);

    /* get the pointer to pixel data */
    pixelData = inputBitmap.getPixels();

    /* error check */
    if(pixelData == NULL)
    {
        sampleCommon->error("Failed to read pixel Data!");
        return SDK_FAILURE;
    }


    /* Copy pixel data into inputImageData */
    memcpy(inputImageData, pixelData, width * height * pixelSize);


    /* allocate memory for verification output */
    verificationOutput = (cl_uchar4*)malloc(width * height * sizeof(cl_uchar4));

    /* error check */
    if(verificationOutput == NULL)
    {
        sampleCommon->error("verificationOutput heap allocation failed!");
        return SDK_FAILURE;
    }

    /* initialize the data to NULL */
    memset(verificationOutput, 0, width * height * pixelSize);

    /* Calculate passes required for SAT computation */
    cl_float tempN = (cl_float)((cl_float)log((float)width) / 
        log((float)rHorizontal));
    cl_float tempM = (cl_float)((cl_float)log((float)height) / 
        log((float)rVertical));

    /* Calculate horizontal passes */
    while(fabs(tempN - (cl_int)tempN) > 1e-7)
    {
        if(rHorizontal < 2)
        {
            std::cout << 
                "Width should be a multiple of power of 2 atleast!" << 
                std::endl;
            return SDK_FAILURE;
        }
        rHorizontal /= 2;
        tempN = (cl_float)((cl_float)log((float)width) /
            log((float)rHorizontal));
    }
    n = (cl_uint)tempN;

    /* Calculate vertical passes */
    while(fabs(tempM - (cl_int)tempM) > 1e-7)
    {
        if(rVertical < 2)
        {
            std::cout <<
                "Height should be a multiple of power of 2 atleast!" << 
                std::endl;
            return SDK_FAILURE;
        }
        rVertical /= 2;
        tempM = (cl_float)((cl_float)log((float)width) 
            / log((float)rVertical));
    }
    m = (cl_uint)tempM;

    return SDK_SUCCESS;

}


int
BoxFilterGLSAT::writeOutputImage(std::string outputImageName)
{
    /* copy output image data back to original pixel data */
    memcpy(pixelData, outputImageData, width * height * pixelSize);

    /* write the output bmp file */
    if(!inputBitmap.write(outputImageName.c_str()))
    {
        sampleCommon->error("Failed to write output image!");
        return SDK_FAILURE;
    }

    return SDK_SUCCESS;
}

int 
BoxFilterGLSAT::genBinaryImage()
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
    kernelPath.append("BoxFilterGL_Kernels.cl");
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
BoxFilterGLSAT::setupCL()
{
    cl_int status = CL_SUCCESS;
    cl_device_type dType;

    if(deviceType.compare("cpu") == 0)
    {
        dType = CL_DEVICE_TYPE_CPU;
    }
    else //deviceType = "gpu" 
    {
        dType = CL_DEVICE_TYPE_GPU;
    }

    size_t deviceListSize;

    /*
    * Have a look at the available platforms and pick either
    * the AMD one if available or the system default.
    */

    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (CL_SUCCESS != status) {
        fputs("clGetPlatformIDs() failed", stderr);
        exit(-1);
    }
    if (0 < numPlatforms) {
        cl_platform_id* platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if (CL_SUCCESS != status) {
            fputs("clGetPlatformIDs() failed", stderr);
            exit(-1);
        }
        for (unsigned i = 0; i < numPlatforms; ++i) {
            char pbuf[100];
            status = clGetPlatformInfo(
                platforms[i],
                CL_PLATFORM_VENDOR,
                sizeof(pbuf),
                pbuf,
                NULL);
            if (!strcmp(pbuf, "Advanced Micro Devices, Inc.")) {
                platform = platforms[i];
                break;
            }
        }
        delete platforms;
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
    #ifdef _WIN32
    HGLRC glCtx = wglGetCurrentContext();
#else //!_WIN32
    GLXContext glCtx = glXGetCurrentContext();
#endif //!_WIN32
    
    cl_context_properties cpsGL[] = 
    {
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)platform,
#ifdef _WIN32
        CL_WGL_HDC_KHR,
        (intptr_t)wglGetCurrentDC(),
#else //!_WIN32
        CL_GLX_DISPLAY_KHR,
        (intptr_t)glXGetCurrentDisplay(),
#endif //!_WIN32
        CL_GL_CONTEXT_KHR,
        (intptr_t)glCtx,
        0
    };


    context = clCreateContextFromType(
        cpsGL,
        dType,
        NULL,
        NULL,
        &status);

    if(!sampleCommon->checkVal(status,
        CL_SUCCESS,
        "clCreateContextFromType failed."))
    {
        return SDK_FAILURE;
    }

    /* First, get the size of device list data */
    status = clGetContextInfo(
        context, 
        CL_CONTEXT_DEVICES, 
        0, 
        NULL, 
        &deviceListSize);
    if(!sampleCommon->checkVal(
        status, 
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
    status = clGetContextInfo(
        context, 
        CL_CONTEXT_DEVICES, 
        deviceListSize, 
        devices, 
        NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetContextInfo failed."))
        return SDK_FAILURE;

    /* Create command queue */

    cl_command_queue_properties prop = 0;


    commandQueue = clCreateCommandQueue(
        context,
        devices[deviceId],
        CL_QUEUE_PROFILING_ENABLE,
        &status);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateCommandQueue failed."))
    {
        return SDK_FAILURE;
    }

    /*
    * Create and initialize memory objects
    */

    /* Create memory object for input Image */
    inputImageBuffer = clCreateBuffer(context,
                                      CL_MEM_READ_ONLY,
                                      width * height * pixelSize,
                                      0,
                                      &status);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }

    /* Write Data to inputImageBuffer */
    status = clEnqueueWriteBuffer(commandQueue,
                                  inputImageBuffer,
                                  CL_TRUE,
                                  0,
                                  width * height * sizeof(cl_uchar4),
                                  inputImageData,
                                  0,
                                  0,
                                  0);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clEnqueueWriteBuffer failed. (inputImageBuffer)"))
    {
        return SDK_FAILURE;
    }


    /* Create memory object for temp0 */
    tempImageBuffer0 = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        width * height * sizeof(cl_uint4),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (tempImageBuffer0)"))
    {
        return SDK_FAILURE;
    }

    /* Create memory object for temp1 */
    tempImageBuffer1 = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        width * height * sizeof(cl_uint4),
        0,
        &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateBuffer failed. (tempImageBuffer1)"))
    {
        return SDK_FAILURE;
    }

    /* 
     * Create texture object 
     */
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    /* Set parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
     * Create pixel-buffer object
     */
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_ARRAY_BUFFER, pbo);

    // initialize buffer object
    unsigned int size = width * height * sizeof(cl_uchar4);

    // buffer data
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Create OpenCL buffer from GL PBO */
    outputImageBuffer = clCreateFromGLBuffer(context, 
                                             CL_MEM_WRITE_ONLY, 
                                             pbo, 
                                             &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clCreateFromGLBuffer failed. (outputImageBuffer)"))
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
        kernelPath.append("BoxFilterGL_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }
        const char *source = kernelFile.source().c_str();
        size_t sourceSize[] = {strlen(source)};
        program = clCreateProgramWithSource(context,
            1,
            &source,
            sourceSize,
            &status);
        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "clCreateProgramWithSource failed."))
            return SDK_FAILURE;
    }

    /* create a cl program executable for all the devices specified */
    status = clBuildProgram(
        program,
        1,
        &devices[deviceId],
        NULL,
        NULL,
        NULL);
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
    /* get a kernel object handle for a kernel with the given name */
    kernel = clCreateKernel(program, "box_filter",&status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed.(box)"))
        return SDK_FAILURE;

    /* Create a kernel object for first horizontal pass of SAT computation */
    horizontalSAT0 = clCreateKernel(program, "horizontalSAT0", &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed.(horizontalSAT0)"))
        return SDK_FAILURE;

    /* Create a kernel object for rest horizontal pass of SAT computation */
    horizontalSAT = clCreateKernel(program, "horizontalSAT", &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed.(horizontalSAT)"))
        return SDK_FAILURE;

    /* Create a kernel object for vertical pass of SAT computation */
    verticalSAT = clCreateKernel(program, "verticalSAT", &status);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clCreateKernel failed.(verticalSAT)"))
        return SDK_FAILURE;

    size_t workGroupSizeKernel;
    size_t workGroupSizeHorizontalSAT0;
    size_t workGroupSizeHorizontalSAT;
    size_t workGroupSizeVerticalSAT;

    /* Check group size against group size returned by kernel */
    status = clGetKernelWorkGroupInfo(kernel,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &workGroupSizeKernel,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    status = clGetKernelWorkGroupInfo(horizontalSAT0,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &workGroupSizeHorizontalSAT0,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    status = clGetKernelWorkGroupInfo(horizontalSAT,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &workGroupSizeHorizontalSAT,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    status = clGetKernelWorkGroupInfo(verticalSAT,
        devices[deviceId],
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(size_t),
        &workGroupSizeVerticalSAT,
        0);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clGetKernelWorkGroupInfo  failed."))
    {
        return SDK_FAILURE;
    }

    size_t minA = min(workGroupSizeVerticalSAT, workGroupSizeHorizontalSAT);
    size_t minB = min(workGroupSizeKernel, workGroupSizeHorizontalSAT0);
    kernelWorkGroupSize = min(minA, minB);

    if((blockSizeX * blockSizeY) > kernelWorkGroupSize)
    {
        if(!quiet)
        {
            std::cout << "Out of Resources!" << std::endl;
            std::cout << "Group Size specified : "
                << blockSizeX * blockSizeY << std::endl;
            std::cout << "Max Group Size supported on the kernel : "
                << kernelWorkGroupSize << std::endl;
            std::cout << "Falling back to " << kernelWorkGroupSize << std::endl;
        }

        /* Three possible cases */
        if(blockSizeX > kernelWorkGroupSize)
        {
            blockSizeX = kernelWorkGroupSize;
            blockSizeY = 1;
        }
    }
    return SDK_SUCCESS;
}

int
BoxFilterGLSAT::runSatKernel(cl_kernel kernel,
                        cl_mem *input,
                        cl_mem *output,
                        cl_uint pass,
                        cl_uint r)
{
    int status;
    cl_event events[2];

    /* Setup arguments to the kernel */

    /* input buffer */
    status = clSetKernelArg(kernel, 
                            0, 
                            sizeof(cl_mem), 
                            input);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (input)"))
    {
        return SDK_FAILURE;
    }

    /* output buffer */
    status = clSetKernelArg(kernel, 
                            1, 
                            sizeof(cl_mem), 
                            output);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (satHorizontalPass[0])"))
    {
        return SDK_FAILURE;
    }

    /* current pass */
    status = clSetKernelArg(kernel, 
                            2, 
                            sizeof(cl_uint), 
                            &pass);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (pass)"))
    {
        return SDK_FAILURE;
    }

    /* Memory Fetches for SAT computation */
    status = clSetKernelArg(kernel, 
                            3, 
                            sizeof(cl_uint), 
                            &r);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (r)"))
    {
        return SDK_FAILURE;
    }

    /* width of input image */
    status = clSetKernelArg(kernel, 
                            4, 
                            sizeof(cl_uint), 
                            &width);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (width)"))
    {
        return SDK_FAILURE;
    }


    size_t globalThreads[] = {width, height};
    size_t localThreads[] = {blockSizeX, blockSizeY};

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
    {
        return SDK_FAILURE;
    }

    clFinish(commandQueue);

    cl_ulong startTime=0;
    cl_ulong endTime=0;
    /* Display proiling info */
    status = clGetEventProfilingInfo(events[0],
                                     CL_PROFILING_COMMAND_START,
                                     sizeof(cl_ulong),
                                     &startTime,
                                     0);

    status = clGetEventProfilingInfo(events[0],
                                     CL_PROFILING_COMMAND_END,
                                     sizeof(cl_ulong),
                                     &endTime,
                                     0);

    cl_double elapsedTime = (endTime - startTime) * 1e-6;

    if(!quiet)
        std::cout <<" \t" << elapsedTime <<" ms"<<std::endl;
    clReleaseEvent(events[0]);

    return SDK_SUCCESS;

}

int 
BoxFilterGLSAT::runBoxFilterKernel()
{
    int status;
    cl_mem *input;
    cl_event events;

    if((n - 1 + m) % 2)
        input = &tempImageBuffer1;
    else
        input = &tempImageBuffer0;

    /* Setup arguments to the kernel */

    /* input buffer */
    status = clSetKernelArg(kernel, 
                            0, 
                            sizeof(cl_mem), 
                            input);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (input)"))
    {
        return SDK_FAILURE;
    }

    /* Acquire outputImageBuffer from GL */
    status = clEnqueueAcquireGLObjects(commandQueue, 
                                       1, 
                                       &outputImageBuffer, 
                                       0, 
                                       0,
                                       NULL);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "clEnqueueAcquireGLObjects failed."))
        return SDK_FAILURE;


    /* output buffer */
    status = clSetKernelArg(kernel, 
                            1, 
                            sizeof(cl_mem), 
                            &outputImageBuffer);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed.(output)"))
    {
        return SDK_FAILURE;
    }

    /* current pass */
    status = clSetKernelArg(kernel, 
                            2, 
                            sizeof(cl_uint), 
                            &filterWidth);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS,
                               "clSetKernelArg failed. (pass)"))
    {
        return SDK_FAILURE;
    }

    size_t globalThreads[] = 
    {
        width,
        height
    };
    size_t localThreads[] = {blockSizeX, blockSizeY};

    status = clEnqueueNDRangeKernel(commandQueue,
                                    kernel,
                                    2,
                                    NULL,
                                    globalThreads,
                                    localThreads,
                                    0,
                                    NULL,
                                    &events);

    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clEnqueueNDRangeKernel failed."))
    {
        return SDK_FAILURE;
    }

    status = clFinish(commandQueue);
    if(!sampleCommon->checkVal(status,
                               CL_SUCCESS, 
                               "clFinish failed."))
    {
        return SDK_FAILURE;
    }

    if(verify)
    {

        /* Read back the value  */
        cl_int status = clEnqueueReadBuffer(commandQueue,
            outputImageBuffer,
            CL_TRUE,
            0,
            width * height * sizeof(cl_uchar4),
            outputImageData,
            0,
            0,
            0);
        if(!sampleCommon->checkVal(status,
            CL_SUCCESS, 
            "clEnqueueReadBuffer failed."))
        {
            return SDK_FAILURE;
        }
    }

    /* Now OpenGL gets control of outputImageBuffer */
    status = clEnqueueReleaseGLObjects(commandQueue, 
                                       1, 
                                       &outputImageBuffer, 
                                       0, 
                                       0, 
                                       0);
    if(!sampleCommon->checkVal(
         status,
         CL_SUCCESS, 
         "clEnqueueReleaseGLObjects failed."))
         return SDK_FAILURE;

    return SDK_SUCCESS;

}


int 
BoxFilterGLSAT::runCLKernels()
{
    cl_uint pass = 0;

    if(!quiet)
    {
        std::cout << "Computing SAT \t\t Time" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "HorizontalPass[" << pass << "]";
    }

    /* SAT : Summed Area table computation 
     * This is done in log(N) time
     * This is done in 2 steps : HorizontalPasses and VerticalPasses 
     * This will compute a data structure(i.e SAT) to do fast blur on the image
     * as it allows to apply filter of any size with only 4 fetches from SAT
     * Hence this algorithm is ideal for interactive applications */

    //Compute Horizontal pass = 0
    if(!runSatKernel(horizontalSAT0, 
                     &inputImageBuffer, 
                     &tempImageBuffer0,
                     pass,
                     rHorizontal))
    {

        cl_mem *inputHorizontal = &tempImageBuffer0;
        cl_mem *outputHorizontal = &tempImageBuffer1;

        //Do rest of the horizontal passes
        for(pass = 1; pass < n; pass++)
        {
            if(!quiet)
                std::cout << "HorizontalPass[" << pass << "]";

            if(!runSatKernel(horizontalSAT, 
                             inputHorizontal, 
                             outputHorizontal,
                             pass,
                             rHorizontal))
            {
 
            }

            /* Swap buffers : tempImageBuffer0 and tempImageBuffer1 */
            cl_mem *temp = inputHorizontal;
            inputHorizontal = outputHorizontal;
            outputHorizontal = temp;
        }
    }

    cl_mem *inputVertical;
    cl_mem *outputVertical;

    if(n % 2)
    {
        inputVertical = &tempImageBuffer0;
        outputVertical = &tempImageBuffer1;
    }
    else
    {
        inputVertical = &tempImageBuffer1;
        outputVertical = &tempImageBuffer0;
    }

    //Do vertical passes
    for(pass = 0; pass < m; pass++)
    {
        if(!quiet)
            std::cout << "VerticalPass[" << pass << "]";

            if(!runSatKernel(verticalSAT,
                             inputVertical,
                             outputVertical,
                             pass,
                             rVertical))
            {
  
            }
            /* Swap Buffers here */
            cl_mem *temp = inputVertical;
            inputVertical = outputVertical;
            outputVertical = temp;
    }

    return SDK_SUCCESS;
}



int 
BoxFilterGLSAT::initialize()
{
    // Call base class Initialize to get default configuration
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    streamsdk::Option* iteration_option = new streamsdk::Option;
    if(!iteration_option)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }
    iteration_option->_sVersion = "i";
    iteration_option->_lVersion = "iterations";
    iteration_option->_description = "Number of iterations to execute kernel";
    iteration_option->_type = streamsdk::CA_ARG_INT;
    iteration_option->_value = &iterations;

    sampleArgs->AddOption(iteration_option);
    delete iteration_option;

    streamsdk::Option* filter_width = new streamsdk::Option;
    if(!filter_width)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }
    filter_width->_sVersion = "x";
    filter_width->_lVersion = "width";
    filter_width->_description = "Filter width";
    filter_width->_type = streamsdk::CA_ARG_INT;
    filter_width->_value = &filterWidth;

    sampleArgs->AddOption(filter_width);
    delete filter_width;

    /* Dummy command line option for sep */
    int dummy_sep_variable = false;
    streamsdk::Option* dummySep = new streamsdk::Option;
    if(!dummySep)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }
    dummySep->_sVersion = "sep";
    dummySep->_lVersion = "separable";
    dummySep->_description = "Flag for separable version";
    dummySep->_type = streamsdk::CA_NO_ARGUMENT;
    dummySep->_value = &dummy_sep_variable;

    sampleArgs->AddOption(dummySep);
    delete dummySep;

    /* Dummy command line option for SAT */
    int dummy_sat_variable = false;
    streamsdk::Option* dummySAT = new streamsdk::Option;
    if(!dummySAT)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }
    dummySAT->_sVersion = "sat";
    dummySAT->_lVersion = "sat";
    dummySAT->_description = "Flag for SAT version";
    dummySAT->_type = streamsdk::CA_NO_ARGUMENT;
    dummySAT->_value = &dummy_sat_variable;

    sampleArgs->AddOption(dummySAT);
    delete dummySAT;


    return SDK_SUCCESS;
}

int 
BoxFilterGLSAT::setup()
{
    

    /* create and initialize timers */
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    if(setupCL() != SDK_SUCCESS)
        return SDK_FAILURE;

    sampleCommon->stopTimer(timer);
    /* Compute setup time */
    setupTime = (double)(sampleCommon->readTimer(timer));

    return SDK_SUCCESS;

}

int 
BoxFilterGLSAT::run()
{
    /* create and initialize timers */
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);

    if(!quiet)
    {
        std::cout << "Executing kernel for " << iterations << 
            " iterations" <<std::endl;
        std::cout << "-------------------------------------------" << std::endl;
    }

    sampleCommon->startTimer(timer);

    for(int i = 0; i < iterations; i++)
    {
        /* Set kernel arguments and run kernel */
        if(runCLKernels() != SDK_SUCCESS)
            return SDK_FAILURE;
        if(runBoxFilterKernel() != SDK_SUCCESS)
            return SDK_FAILURE;
    }

    sampleCommon->stopTimer(timer); 
    /* Compute kernel time */
    kernelTime = (double)(sampleCommon->readTimer(timer)) / iterations;

    if(!verify && !quiet)
    {
        std::cout << "\nPress key w to increase the filter size \n";
        std::cout << "Press key s to decrease the filter size \n";
        std::cout << "Press ESC key to quit \n";
        /* Start the main glut loop */
        glutMainLoop();
    }

    /* write the output image to bitmap file */
    if(writeOutputImage(OUTPUT_SAT_IMAGE) != SDK_SUCCESS)
    {
       return SDK_FAILURE;
    }

    

    return SDK_SUCCESS;
}

int 
BoxFilterGLSAT::cleanup()
{
    if(!byteRWSupport)
        return SDK_SUCCESS;

    /* Releases OpenCL resources (Context, Memory etc.) */
    cl_int status;

    status = clReleaseKernel(kernel);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseKernel failed."))
        return SDK_FAILURE;

    status = clReleaseProgram(program);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseProgram failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(inputImageBuffer);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "clReleaseMemObject failed."))
        return SDK_FAILURE;

    status = clReleaseMemObject(outputImageBuffer);
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
    if(inputImageData) 
        free(inputImageData);

    if(outputImageData)
        free(outputImageData);

    if(verificationOutput) 
        free(verificationOutput);

    if(devices)
        free(devices);

    return SDK_SUCCESS;
}


void 
BoxFilterGLSAT::boxFilterCPUReference()
{
    std::cout << "Verifying results...";
    int t = (filterWidth - 1) / 2;
    int filterSize = filterWidth * filterWidth;

    for(int y = 0; y < (int)height; y++)
    {
        for(int x = 0; x < (int)width; x++)
        {
            /* Only threads inside apron will calculate their pixel value */
            if(x >= t && x < (int)(width - t) && y >= t && y < (int)(height - t))
            {
                cl_int4 sum = {0, 0, 0, 0};
                /* For all pixels inside box */
                for(int y1 = -t; y1 <= t; y1++)
                {
                    for(int x1 = -t; x1 <= t; x1++)
                    {
                        sum.s[0] += inputImageData[x + x1 + (y + y1) * width].s[0];
                        sum.s[1] += inputImageData[x + x1 + (y + y1) * width].s[1];
                        sum.s[2] += inputImageData[x + x1 + (y + y1) * width].s[2];
                        sum.s[3] += inputImageData[x + x1 + (y + y1) * width].s[3];
                    }
                }
                verificationOutput[x + y * width].s[0] = sum.s[0] / filterSize;
                verificationOutput[x + y * width].s[1] = sum.s[1] / filterSize;
                verificationOutput[x + y * width].s[2] = sum.s[2] / filterSize;
                verificationOutput[x + y * width].s[3] = sum.s[3] / filterSize;
            }
        }
    }
    std::cout<<"done!" <<std::endl;

}


int 
BoxFilterGLSAT::verifyResults()
{
    if(!byteRWSupport)
        return SDK_SUCCESS;

    if(verify)
    {
        /* reference implementation */
        boxFilterCPUReference();

        /* Compare between outputImageData and verificationOutput */
        if(!memcmp(outputImageData, 
                   verificationOutput, 
                   width * height * sizeof(cl_uchar4)))
        {
            std::cout << "Passed!\n" <<std::endl;
        }
        else
        {
            std::cout << "Failed!\n" <<std::endl;
        }
    }
    return SDK_SUCCESS;
}

void 
BoxFilterGLSAT::printStats()
{
    std::string strArray[4] = 
    {
        "Width", 
        "Height", 
        "Time(sec)", 
        "kernelTime(sec)"
    };
    std::string stats[4];

    totalTime = setupTime + kernelTime;

    stats[0] = sampleCommon->toString(width, std::dec);
    stats[1] = sampleCommon->toString(height, std::dec);
    stats[2] = sampleCommon->toString(totalTime, std::dec);
    stats[3] = sampleCommon->toString(kernelTime, std::dec);

    this->SDKSample::printStats(strArray, stats, 4);
}


void
BoxFilterGLSAT::displayFunc()
{

    t1 = clock() * CLOCKS_PER_SEC;
    frameCount++;

    /* Execute the kernel which applies the boxfilter */
    runBoxFilterKernel();

    /* Bind PBO and texture */
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
    glBindTexture(GL_TEXTURE_2D, tex);

    /* Copy pixels from pbo to texture */
    glTexSubImage2D(GL_TEXTURE_2D, 
                    0, 
                    0, 
                    0, 
                    width, 
                    height, 
                    GL_RGBA, 
                    GL_UNSIGNED_BYTE, 
                    NULL);

    /* Display image using texture*/
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, width, height);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.5);

    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.5);

    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, 0.5);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, 0.5);

    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glutSwapBuffers();
    glutPostRedisplay();

    t2 = clock() * CLOCKS_PER_SEC;
    totalElapsedTime += (double)(t2 - t1);
    if(frameCount && frameCount > frameRefCount)
    {
        // set GLUT Window Title
        char title[256];
        double fMs = (double)((totalElapsedTime / (double)CLOCKS_PER_SEC) / (double) frameCount);
        int framesPerSec = (int)(1.0 / (fMs / CLOCKS_PER_SEC));
#if defined (_WIN32) && !defined(__MINGW32__)
        sprintf_s(title, 256, "BoxFilterGL SAT | %d fps ", framesPerSec);
#else 
        sprintf(title, "BoxFilterGL SAT | %d fps ", framesPerSec);
#endif
        glutSetWindowTitle(title);
        frameCount = 0;
        totalElapsedTime = 0.0;
    }
}


void
BoxFilterGLSAT::displayFuncWrapper()
{
    /* Call non-static function */
    boxFilterGLSAT->displayFunc();
}

void
BoxFilterGLSAT::keyboardFuncWrapper(unsigned char key, int x, int y)
{
    /* Call non-static function */
    boxFilterGLSAT->keyboardFunc(key , x, y);
}

/* Initialize the value to NULL */
BoxFilterGLSAT *BoxFilterGLSAT::boxFilterGLSAT = NULL;


void 
BoxFilterGLSAT::keyboardFunc(unsigned char key, int /*x*/, int /*y*/)
{
    switch(key)
    {
        /* If the user hits escape or Q, then exit */
        /* ESCAPE_KEY = 27 */
    case 'w':
        {
            filterWidth += 2;
            break;
        }
    case 's':
        {
            if(filterWidth > 2)
                filterWidth -= 2;
            break;
        }
    case 27 :
        {
            if(cleanup() != SDK_SUCCESS)
                exit(1);
            else
                exit(0);
        }
    default:
        break;
    }
}


int BoxFilterGLSAT::initializeGL(int argc, char * argv[])
{
    /* Allocate host memory and read input image */
    if(readInputImage(INPUT_IMAGE) != SDK_SUCCESS)
        return SDK_FAILURE;

    // Create GL context
    //glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("BoxFilter Separable");
	

    // GL init
    glewInit();
    if (! glewIsSupported("GL_VERSION_2_0 " "GL_ARB_pixel_buffer_object"))
    {
          std::cerr
              << "Support for necessary OpenGL extensions missing."
              << std::endl;
          return SDK_FAILURE;
    }

    //glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60.0,
        (GLfloat)width / (GLfloat)height,
        0.1,
        10.0);

    glutDisplayFunc(displayFuncWrapper);
    glutKeyboardFunc(keyboardFuncWrapper);
    return SDK_SUCCESS;
}






