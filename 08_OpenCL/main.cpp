#include <iostream>
#include <chrono>
#include <CL/cl.hpp>

#define NUM_OF_THREADS 1024

unsigned int  timeAddVectorsCPU(int numberOfElements)
{
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point stop;
	std::chrono::milliseconds duration;

	int* A = new int[numberOfElements];
	int* B = new int[numberOfElements];
	unsigned long int SUM = 0;


	for (int i = 0; i < numberOfElements; ++i) {
		A[i] = i;
		B[i] = numberOfElements - i;
	}

	start = std::chrono::high_resolution_clock::now();

	for (int j = 0; j < numberOfElements; ++j)
		SUM += A[j] * B[j];
	

	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	delete[] A, B;

	return duration.count();
}


int main() 
{
	const std::vector<int> numberOfElements{ 512, 1024, 131072 };
	const int numberOfLoops = 3;         
	const int numberOfADDITIONLoops = 1000;

	// get all platforms (drivers), e.g. NVIDIA
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);

	if (all_platforms.size() == 0) 
	{
		std::cout << " No platforms found. Check OpenCL installation!\n";
		exit(1);
	}
	cl::Platform default_platform = all_platforms[0];
	std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

	// get default device (CPUs, GPUs) of the default platform
	std::vector<cl::Device> all_devices;
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) 
	{
		std::cout << " No devices found. Check OpenCL installation!\n";
		exit(1);
	}

	// use device[1] because that's a GPU; device[0] is the CPU
	cl::Device default_device = all_devices[1];
	std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

	cl::Context context({ default_device });
	cl::Program::Sources sources;

	// calculates for each element; C = A + B
	std::string kernel_code =
		"   void kernel add(global const int* v1, global const int* v2, "
		"                          const int numberOfElements) {"
		"       int ID, NUM_GLOBAL_WITEMS, ratio, start, stop;"
		"       unsigned long int SUM = 0; "
		"       ID = get_global_id(0);"
		"       NUM_GLOBAL_WITEMS = get_global_size(0);"
		""
		"       ratio = (numberOfElements / NUM_GLOBAL_WITEMS);" // elements per thread
		"       start = ratio * ID;"
		"       stop  = ratio * (ID + 1);"
		""
		"       int i, j;" // will the compiler optimize this anyway? probably.
		"           for (j = start; j < stop; ++j)"
		"           {"
		"               SUM += v1[j] * v2[j];"
		"           }"
		"   }";

	sources.push_back({ kernel_code.c_str(), kernel_code.length() });

	cl::Program program(context, sources);
	if (program.build({ default_device }) != CL_SUCCESS) 
	{
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
		exit(1);
	}

	// set up kernels and vectors for GPU code
	cl::CommandQueue queue(context, default_device);
	cl::Kernel add = cl::Kernel(program, "add");

	// running test for 512, 1024, 131072 elements
	for (auto i = numberOfElements.begin(); i != numberOfElements.end(); ++i)
	{
		// run the CPU code
		unsigned int CPUduration = 0;
		for (int ii = 0; ii < numberOfADDITIONLoops; ++ii)
		{
			CPUduration += timeAddVectorsCPU(*i);
		}

		// construct vectors
		int* A = new int[*i];
		int* B = new int[*i];

		for (int j = 0; j < *i; ++j) {
			A[j] = j;
			B[j] = *i - j - 1;
		}

		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point stop;
		std::chrono::milliseconds duration;

		unsigned int  GPUduration;

		// start timer
		start = std::chrono::high_resolution_clock::now();


		// allocate space
		cl::Buffer firstArray(context, CL_MEM_READ_WRITE, sizeof(int) * *i);
		cl::Buffer secondArray(context, CL_MEM_READ_WRITE, sizeof(int) * *i);

		// push write commands to queue
		queue.enqueueWriteBuffer(firstArray, CL_TRUE, 0, sizeof(int)* *i, A);
		queue.enqueueWriteBuffer(secondArray, CL_TRUE, 0, sizeof(int)* *i, B);


		add.setArg(0, firstArray);
		add.setArg(1, secondArray);
		add.setArg(2, numberOfElements);

		for (int ii = 0; ii < numberOfADDITIONLoops; ++ii)
		{
			queue.enqueueNDRangeKernel(add, cl::NullRange,  // kernel, offset
				cl::NDRange(NUM_OF_THREADS), // global number of work items
				cl::NDRange(32));               // local number (per group)

			queue.finish(); 
		}

		stop = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		GPUduration = duration.count();

		std::cout << "For " << *i << " elements: " << std::endl;
		std::cout << "CPU time: " << CPUduration << " milliseconds." << std::endl;
		std::cout << "GPU time: " << GPUduration << " milliseconds." << std::endl;
		std::cout << std::endl;

		delete[] A, B;
	}
	std::cin.get();
	return 0;
}