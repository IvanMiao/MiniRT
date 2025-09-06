/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_render.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:00:00 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/04 21:24:29 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_RENDER_H
# define OPENCL_RENDER_H

# include <stddef.h>
# include <stdint.h>

// Forward declarations
typedef struct s_minirt t_minirt;

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
// Forward declarations for OpenCL types to avoid header dependency
typedef struct _cl_platform_id *    cl_platform_id;
typedef struct _cl_device_id *      cl_device_id;
typedef struct _cl_context *        cl_context;
typedef struct _cl_command_queue *  cl_command_queue;
typedef struct _cl_mem *            cl_mem;
typedef struct _cl_program *        cl_program;
typedef struct _cl_kernel *         cl_kernel;
typedef int                         cl_int;

// OpenCL constants
# define CL_SUCCESS                   0
# define CL_DEVICE_TYPE_GPU           (1 << 2)
# define CL_DEVICE_TYPE_CPU           (1 << 1)
# define CL_MEM_READ_ONLY             (1 << 2)
# define CL_MEM_WRITE_ONLY            (1 << 1)
# define CL_MEM_COPY_HOST_PTR         (1 << 5)
# define CL_TRUE                      1
# define CL_PROGRAM_BUILD_LOG         0x1183

// OpenCL function declarations
extern cl_int clGetPlatformIDs(cl_int, cl_platform_id *, cl_int *);
extern cl_int clGetDeviceIDs(cl_platform_id, cl_int, cl_int, cl_device_id *, cl_int *);
extern cl_context clCreateContext(const intptr_t *, cl_int, const cl_device_id *, void (*)(const char *, const void *, size_t, void *), void *, cl_int *);
extern cl_command_queue clCreateCommandQueue(cl_context, cl_device_id, intptr_t, cl_int *);
extern cl_program clCreateProgramWithSource(cl_context, cl_int, const char **, const size_t *, cl_int *);
extern cl_int clBuildProgram(cl_program, cl_int, const cl_device_id *, const char *, void (*)(cl_program, void *), void *);
extern cl_kernel clCreateKernel(cl_program, const char *, cl_int *);
extern cl_mem clCreateBuffer(cl_context, intptr_t, size_t, void *, cl_int *);
extern cl_int clSetKernelArg(cl_kernel, cl_int, size_t, const void *);
extern cl_int clEnqueueNDRangeKernel(cl_command_queue, cl_kernel, cl_int, const size_t *, const size_t *, const size_t *, cl_int, const cl_mem *, cl_mem *);
extern cl_int clEnqueueReadBuffer(cl_command_queue, cl_mem, cl_int, size_t, size_t, void *, cl_int, const cl_mem *, cl_mem *);
extern cl_int clGetProgramBuildInfo(cl_program, cl_device_id, cl_int, size_t, void *, size_t *);
extern cl_int clReleaseMemObject(cl_mem);
extern cl_int clReleaseKernel(cl_kernel);
extern cl_int clReleaseProgram(cl_program);
extern cl_int clReleaseCommandQueue(cl_command_queue);
extern cl_int clReleaseContext(cl_context);
# endif

// OpenCL context structure
typedef struct s_opencl_ctx
{
	cl_platform_id		platform;
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				output_buffer;
	cl_mem				scene_buffer;
	cl_mem				objects_buffer;
	size_t				global_size[2];
	size_t				local_size[2];
	int					initialized;
}	t_opencl_ctx;

// Simplified scene data for GPU
typedef struct s_gpu_scene
{
	float	camera_pos[3];
	float	camera_dir[3];
	float	camera_u[3];
	float	camera_v[3];
	float	camera_w[3];
	float	half_width;
	float	half_height;
	float	ambient_ratio;
	float	ambient_color[3];
	float	light_pos[3];
	float	light_ratio;
	int		num_objects;
}	t_gpu_scene;

// GPU object data
typedef struct s_gpu_object
{
	int		type;		// 0=sphere, 1=plane, 2=cylinder
	float	pos[3];		// center/point
	float	dir[3];		// normal/axis
	float	color[3];
	float	radius;		// for sphere/cylinder
	float	height;		// for cylinder
}	t_gpu_object;

// Function declarations
int		init_opencl(t_opencl_ctx *ctx);
void	cleanup_opencl(t_opencl_ctx *ctx);
int		render_opencl(t_minirt *rt, t_opencl_ctx *ctx);
void	prepare_scene_data(t_minirt *rt, t_gpu_scene *scene, t_gpu_object **objects, int *num_objects);

#endif
