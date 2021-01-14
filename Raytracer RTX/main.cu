#include <iostream>
#include <time.h>

//chech if gpu is available
#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

//Allocates chunk to be reandred by thread block
__global__ void render(float *fb, int max_x, int max_y) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if((i >= max_x) || (j >= max_y)) return;
    int pixel_index = j*max_x*3 + i*3;
    fb[pixel_index + 0] = float(i) / max_x;
    fb[pixel_index + 1] = float(j) / max_y;
    fb[pixel_index + 2] = 0.2;
}

ing main(){
    //size of frame
    int width = 1200; int height = 600; //resolution of the image

    //number of threads allocated to render block (frame buffer)
    int threadsX = 8; int threadsY = 8; //8*8 64 threads per render block

    std::cerr << "Rendering... \n";
    

    int total_pixels = width * height;
    size_t chunk_size = 3*total_pixels*sizeof(double);

    //allocate memeory for chunk (frame buffer)
    double *chunk;
    checkCudaErrors(cudaMallocManaged((void **)&chunk, chunk_size));

    clock_t t1, t2;
    t1 = clock();

    //Render our chunk (frame buffer)
    dim3 blocks(width/threadsX+1, height/threadsY+1);
    dim3 threads(tx, ty);
    render<<<blocks, threads>>>(chunk, width, height); //create threads

    checkCudaErrors(cudaGetLastError()); //check for errors
    checkCudaErrors(cudaDeviceSynchronize()); //syncronize threads

    t2 = clock();
    double timer_seconds = ((double)(t2 - t1))/ CLOCKS_PER_SEC;
    std::cerr << timer_seconds << "seconds \n";

    //Output chucks as immage
    for(int j = height-1; j >= 0; j--){
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j*3*nx + i*3;
            float r = fb[pixel_index + 0];
            float g = fb[pixel_index + 1];
            float b = fb[pixel_index + 2];
            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    checkCudaErrors(cudaFree(chuck)); //kill threads
}

