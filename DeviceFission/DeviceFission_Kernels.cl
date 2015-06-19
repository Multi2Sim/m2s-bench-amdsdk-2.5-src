/*
 * Copys the input to output
 */
__kernel
void 
copy(__global float* input, __global float* output)
{
    size_t xPos = get_global_id(0);
    output[xPos] = input[xPos];
}
