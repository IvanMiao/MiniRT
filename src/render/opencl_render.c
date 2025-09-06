/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 21:00:00 by ymiao             #+#    #+#             */
/*   Updated: 2025/09/04 21:24:29 by ymiao            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opencl_render.h"
#include "../minirt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static char *read_kernel_source(const char *filename)
{
	FILE *file;
	char *source;
	size_t filesize;

	file = fopen(filename, "r");
	if (!file)
		return (NULL);
	
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	
	source = malloc(filesize + 1);
	if (!source) {
		fclose(file);
		return (NULL);
	}
	
	fread(source, 1, filesize, file);
	source[filesize] = '\0';
	fclose(file);
	
	return (source);
}

int init_opencl(t_opencl_ctx *ctx)
{
	cl_int err;
	char *kernel_source;
	size_t source_size;

	// Initialize structure
	memset(ctx, 0, sizeof(t_opencl_ctx));

	// Get platform
	err = clGetPlatformIDs(1, &ctx->platform, NULL);
	if (err != CL_SUCCESS) {
		printf("Error getting platform: %d\n", err);
		return (0);
	}

	// Get device
	err = clGetDeviceIDs(ctx->platform, CL_DEVICE_TYPE_GPU, 1, &ctx->device, NULL);
	if (err != CL_SUCCESS) {
		// Fallback to CPU
		err = clGetDeviceIDs(ctx->platform, CL_DEVICE_TYPE_CPU, 1, &ctx->device, NULL);
		if (err != CL_SUCCESS) {
			printf("Error getting device: %d\n", err);
			return (0);
		}
	}

	// Create context
	ctx->context = clCreateContext(NULL, 1, &ctx->device, NULL, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("Error creating context: %d\n", err);
		return (0);
	}

	// Create command queue
	ctx->queue = clCreateCommandQueue(ctx->context, ctx->device, 0, &err);
	if (err != CL_SUCCESS) {
		printf("Error creating command queue: %d\n", err);
		cleanup_opencl(ctx);
		return (0);
	}

	// Read kernel source
	kernel_source = read_kernel_source("src/render/raytracer.cl");
	if (!kernel_source) {
		printf("Error reading kernel source\n");
		cleanup_opencl(ctx);
		return (0);
	}

	source_size = strlen(kernel_source);

	// Create program
	ctx->program = clCreateProgramWithSource(ctx->context, 1, 
		(const char **)&kernel_source, &source_size, &err);
	free(kernel_source);
	
	if (err != CL_SUCCESS) {
		printf("Error creating program: %d\n", err);
		cleanup_opencl(ctx);
		return (0);
	}

	// Build program
	err = clBuildProgram(ctx->program, 1, &ctx->device, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error building program: %d\n", err);
		
		// Get build log
		size_t log_size;
		clGetProgramBuildInfo(ctx->program, ctx->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = malloc(log_size);
		clGetProgramBuildInfo(ctx->program, ctx->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build log:\n%s\n", log);
		free(log);
		
		cleanup_opencl(ctx);
		return (0);
	}

	// Create kernel
	ctx->kernel = clCreateKernel(ctx->program, "raytracer_kernel", &err);
	if (err != CL_SUCCESS) {
		printf("Error creating kernel: %d\n", err);
		cleanup_opencl(ctx);
		return (0);
	}

	// Create output buffer
	ctx->output_buffer = clCreateBuffer(ctx->context, CL_MEM_WRITE_ONLY, 
		WIDTH * HEIGHT * sizeof(int), NULL, &err);
	if (err != CL_SUCCESS) {
		printf("Error creating output buffer: %d\n", err);
		cleanup_opencl(ctx);
		return (0);
	}

	// Set work group sizes
	ctx->global_size[0] = WIDTH;
	ctx->global_size[1] = HEIGHT;
	ctx->local_size[0] = 8;
	ctx->local_size[1] = 8;

	ctx->initialized = 1;
	printf("OpenCL initialized successfully\n");
	return (1);
}

void cleanup_opencl(t_opencl_ctx *ctx)
{
	if (ctx->output_buffer) clReleaseMemObject(ctx->output_buffer);
	if (ctx->scene_buffer) clReleaseMemObject(ctx->scene_buffer);
	if (ctx->objects_buffer) clReleaseMemObject(ctx->objects_buffer);
	if (ctx->kernel) clReleaseKernel(ctx->kernel);
	if (ctx->program) clReleaseProgram(ctx->program);
	if (ctx->queue) clReleaseCommandQueue(ctx->queue);
	if (ctx->context) clReleaseContext(ctx->context);
	
	memset(ctx, 0, sizeof(t_opencl_ctx));
}

void prepare_scene_data(t_minirt *rt, t_gpu_scene *scene, t_gpu_object **objects, int *num_objects)
{
	t_object *obj;
	int count = 0;

	// Count objects
	obj = rt->object;
	while (obj) {
		count++;
		obj = obj->next;
	}

	*num_objects = count;
	*objects = malloc(count * sizeof(t_gpu_object));

	// Fill scene data
	scene->camera_pos[0] = rt->camera.viewpoint.x;
	scene->camera_pos[1] = rt->camera.viewpoint.y;
	scene->camera_pos[2] = rt->camera.viewpoint.z;

	scene->camera_dir[0] = rt->camera.direction.x;
	scene->camera_dir[1] = rt->camera.direction.y;
	scene->camera_dir[2] = rt->camera.direction.z;

	scene->camera_u[0] = rt->camera.u.x;
	scene->camera_u[1] = rt->camera.u.y;
	scene->camera_u[2] = rt->camera.u.z;

	scene->camera_v[0] = rt->camera.v.x;
	scene->camera_v[1] = rt->camera.v.y;
	scene->camera_v[2] = rt->camera.v.z;

	scene->camera_w[0] = rt->camera.w.x;
	scene->camera_w[1] = rt->camera.w.y;
	scene->camera_w[2] = rt->camera.w.z;

	scene->half_width = rt->camera.half_width;
	scene->half_height = rt->camera.half_height;

	scene->ambient_ratio = rt->ambient.ratio;
	scene->ambient_color[0] = rt->ambient.color.r;
	scene->ambient_color[1] = rt->ambient.color.g;
	scene->ambient_color[2] = rt->ambient.color.b;

	scene->light_pos[0] = rt->light.position.x;
	scene->light_pos[1] = rt->light.position.y;
	scene->light_pos[2] = rt->light.position.z;
	scene->light_ratio = rt->light.ratio;

	scene->num_objects = count;

	// Fill object data
	obj = rt->object;
	for (int i = 0; i < count; i++) {
		if (obj->type == SPHERE) {
			t_sphere *sphere = (t_sphere *)obj->obj;
			(*objects)[i].type = 0;
			(*objects)[i].pos[0] = sphere->center.x;
			(*objects)[i].pos[1] = sphere->center.y;
			(*objects)[i].pos[2] = sphere->center.z;
			(*objects)[i].radius = sphere->radius;
		} else if (obj->type == PLANE) {
			t_plane *plane = (t_plane *)obj->obj;
			(*objects)[i].type = 1;
			(*objects)[i].pos[0] = plane->point.x;
			(*objects)[i].pos[1] = plane->point.y;
			(*objects)[i].pos[2] = plane->point.z;
			(*objects)[i].dir[0] = plane->normal.x;
			(*objects)[i].dir[1] = plane->normal.y;
			(*objects)[i].dir[2] = plane->normal.z;
		} else if (obj->type == CYLINDER) {
			t_cylinder *cylinder = (t_cylinder *)obj->obj;
			(*objects)[i].type = 2;
			(*objects)[i].pos[0] = cylinder->center.x;
			(*objects)[i].pos[1] = cylinder->center.y;
			(*objects)[i].pos[2] = cylinder->center.z;
			(*objects)[i].dir[0] = cylinder->normal.x;
			(*objects)[i].dir[1] = cylinder->normal.y;
			(*objects)[i].dir[2] = cylinder->normal.z;
			(*objects)[i].radius = cylinder->diameter / 2.0;
			(*objects)[i].height = cylinder->height;
		}

		(*objects)[i].color[0] = obj->color.r;
		(*objects)[i].color[1] = obj->color.g;
		(*objects)[i].color[2] = obj->color.b;

		obj = obj->next;
	}
}

int render_opencl(t_minirt *rt, t_opencl_ctx *ctx)
{
	cl_int err;
	t_gpu_scene scene;
	t_gpu_object *objects;
	int num_objects;
	int *output_data;

	if (!ctx->initialized) {
		printf("OpenCL not initialized\n");
		return (0);
	}

	// Prepare scene data
	prepare_scene_data(rt, &scene, &objects, &num_objects);

	// Create scene buffer
	if (ctx->scene_buffer) clReleaseMemObject(ctx->scene_buffer);
	ctx->scene_buffer = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(t_gpu_scene), &scene, &err);
	if (err != CL_SUCCESS) {
		printf("Error creating scene buffer: %d\n", err);
		free(objects);
		return (0);
	}

	// Create objects buffer
	if (ctx->objects_buffer) clReleaseMemObject(ctx->objects_buffer);
	ctx->objects_buffer = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		num_objects * sizeof(t_gpu_object), objects, &err);
	if (err != CL_SUCCESS) {
		printf("Error creating objects buffer: %d\n", err);
		free(objects);
		return (0);
	}

	// Set kernel arguments
	err = clSetKernelArg(ctx->kernel, 0, sizeof(cl_mem), &ctx->output_buffer);
	err |= clSetKernelArg(ctx->kernel, 1, sizeof(cl_mem), &ctx->scene_buffer);
	err |= clSetKernelArg(ctx->kernel, 2, sizeof(cl_mem), &ctx->objects_buffer);
	
	if (err != CL_SUCCESS) {
		printf("Error setting kernel arguments: %d\n", err);
		free(objects);
		return (0);
	}

	// Execute kernel
	err = clEnqueueNDRangeKernel(ctx->queue, ctx->kernel, 2, NULL, 
		ctx->global_size, NULL, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error executing kernel: %d\n", err);
		free(objects);
		return (0);
	}

	// Read output
	output_data = malloc(WIDTH * HEIGHT * sizeof(int));
	err = clEnqueueReadBuffer(ctx->queue, ctx->output_buffer, CL_TRUE, 0,
		WIDTH * HEIGHT * sizeof(int), output_data, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("Error reading output: %d\n", err);
		free(objects);
		free(output_data);
		return (0);
	}

	// Copy to image buffer
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			char *dst = rt->img.addr + (y * rt->img.line_length + x * (rt->img.bits_per_pixel / 8));
			*(unsigned int *)dst = output_data[y * WIDTH + x];
		}
	}

	free(objects);
	free(output_data);
	
	printf("OpenCL rendering completed\n");
	return (1);
}
