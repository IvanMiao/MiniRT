#ifndef RENDER_CUDA_H
# define RENDER_CUDA_H

# include "../minirt.h"

# ifdef __cplusplus
  extern "C" {
# endif

/*
 * This function serves as the entry point for the CUDA-accelerated rendering
 * process. It is defined in render_cuda.cu and will be called from main.c
 * when the --cuda flag is specified.
*/
void	render_with_cuda(t_minirt *rt);


# ifdef __cplusplus
  }
# endif

#endif
