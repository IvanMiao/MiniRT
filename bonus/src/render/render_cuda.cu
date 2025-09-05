#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include "render_cuda.h"

#define WIDTH 1200
#define HEIGHT 900

# define SAMPLES_PER_PIXEL 256
# define SHININESS 64.0f

#define CUDA_CHECK(err) { \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA Error: %s at %s:%d\n", cudaGetErrorString(err), __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
}

// GPU version of light structure without 'next' pointer
typedef struct s_light_gpu
{
	t_vector	position;
	float		ratio;
	t_color		color;
}	t_light_gpu;

// 简单的线性同余伪随机数生成器，适用于 GPU
__device__ unsigned int cuda_rand(unsigned int *seed)
{
    *seed = (*seed * 1103515245U + 12345U) & 0x7fffffffU;
    return *seed;
}

// 生成 [0, 1) 范围内的随机浮点数
__device__ float cuda_randf(unsigned int *seed)
{
    return (float)cuda_rand(seed) / (float)0x80000000U;
}

__device__ t_vector	vector_init(float x, float y, float z)
{
	t_vector	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

__device__ t_vector	vector_add(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

__device__ t_vector	vector_sub(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}

__device__ t_vector	vector_mult(t_vector v, float scalar)
{
	t_vector	res;

	res.x = v.x * scalar;
	res.y = v.y * scalar;
	res.z = v.z * scalar;
	return (res);
}

__device__ t_vector	vector_div(t_vector v, float scalar)
{
	t_vector	res;

	res.x = v.x / scalar;
	res.y = v.y / scalar;
	res.z = v.z / scalar;
	return (res);
}

__device__ float	vector_length(t_vector v)
{
	float	res;

	res = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return (res);
}

__device__ float	vector_length_sq(t_vector v)
{
	float	res;

	res = v.x * v.x + v.y * v.y + v.z * v.z;
	return (res);
}

__device__ t_vector	vector_normalize(t_vector v)
{
	t_vector	res;
	float		v_length;

	v_length = vector_length(v);
	if (v_length == 0)
		return (vector_init(0, 0, 0));
	res = vector_div(v, v_length);
	return (res);
}

__device__ float	vector_dot(t_vector v1, t_vector v2)
{
	float	res;

	res = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return (res);
}

__device__ t_vector	vector_cross(t_vector v1, t_vector v2)
{
	t_vector	res;

	res.x = (v1.y * v2.z) - (v1.z * v2.y);
	res.y = (v1.z * v2.x) - (v1.x * v2.z);
	res.z = (v1.x * v2.y) - (v1.y * v2.x);
	return (res);
}

__device__ t_color	color_init(int r, int g, int b)
{
	t_color	res;

	res.r = (float)r / 255.0f;
	res.g = (float)g / 255.0f;
	res.b = (float)b / 255.0f;
	return (res);
}

__device__ t_color	color_init_d(float r, float g, float b)
{
	t_color	res;

	res.r = r;
	res.g = g;
	res.b = b;
	return (res);
}

__device__ t_color	color_add(t_color a, t_color b)
{
	t_color	res;

	res.r = a.r + b.r;
	res.g = a.g + b.g;
	res.b = a.b + b.b;
	return (res);
}

__device__ t_color	color_div(t_color a, float scalar)
{
	t_color	res;

	res.r = a.r / scalar;
	res.g = a.g / scalar;
	res.b = a.b / scalar;
	return (res);
}

__device__ int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(fminf(color.r, 1.0f) * 255.0f);
	g = (int)(fminf(color.g, 1.0f) * 255.0f);
	b = (int)(fminf(color.b, 1.0f) * 255.0f);
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	return (r << 16 | g << 8 | b);
}


__device__ float	hit_sphere_gpu(const t_sphere *sp, const t_ray *ray)
{
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	oc = vector_sub(ray->origin, sp->center);
	a = vector_dot(ray->direction, ray->direction);
	b = 2.0f * vector_dot(oc, ray->direction);
	c = vector_dot(oc, oc) - sp->radius * sp->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0f);
	else
	{
		float t = (-b - sqrtf(discriminant)) / (2.0f * a);
		if (t > EPSILON)
			return (t);
		t = (-b + sqrtf(discriminant)) / (2.0f * a);
		if (t > EPSILON)
			return (t);
		return (-1.0f);
	}
}

__device__ t_vector	sphere_normal_at_gpu(t_sphere *sp, t_vector p)
{
	t_vector	normal;

	normal = vector_sub(p, sp->center);
	return (vector_normalize(normal));
}

__device__ float	hit_plane_gpu(t_plane *plane, t_ray *ray)
{
	float		t;
	float		dn;
	t_vector	op;

	op = vector_sub(plane->point, ray->origin);
	dn = vector_dot(ray->direction, plane->normal);
	if (fabsf(dn) < EPSILON)
		return (-1.0f);
	t = vector_dot(op, plane->normal) / dn;
	if (t > EPSILON)
		return (t);
	return (-1.0f);
}


static __device__ float	hit_main(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	float	t1;
	float	t2;
	float	m1;
	float	m2;

	if (info->discri < 0)
		return (-1.0f);
	t1 = (-info->b - sqrtf(info->discri)) / (2 * info->a);
	t2 = (-info->b + sqrtf(info->discri)) / (2 * info->a);
	m1 = vector_dot(ray->direction, info->normal) * t1
		+ vector_dot(info->oc, info->normal);
	m2 = vector_dot(ray->direction, info->normal) * t2
		+ vector_dot(info->oc, info->normal);
	if (t1 > EPSILON && m1 >= 0 && m1 <= cy->height)
		return (t1);
	if (t2 > EPSILON && m2 >= 0 && m2 <= cy->height)
		return (t2);
	return (-1.0f);
}

static __device__ float	hit_caps2(t_plane *cap, t_ray *ray, t_cy_info *info)
{
	float		t;
	t_vector	p;
	float		radius_sq;

	radius_sq = info->radius * info->radius;
	t = hit_plane_gpu(cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		if (vector_length_sq(vector_sub(p, cap->point)) <= radius_sq)
			return (t);
	}
	return (-1.0f);
}

static __device__ float	hit_caps(t_cylinder *cy, t_ray *ray, t_cy_info *info)
{
	t_plane		cap;
	float		t1;
	float		t2;

	cap.point = vector_add(cy->center, vector_mult(info->normal, cy->height));
	cap.normal = info->normal;
	t1 = hit_caps2(&cap, ray, info);
	cap.point = cy->center;
	cap.normal = vector_mult(info->normal, -1);
	t2 = hit_caps2(&cap, ray, info);
	if (t1 > 0 && (t2 < 0 || t1 < t2))
		return (t1);
	return (t2);
}

__device__ float	hit_cylinder_gpu(t_cylinder *cy, t_ray *ray)
{
	t_cy_info	info;
	float		side_t;
	float		caps_t;

	info.normal = vector_normalize(cy->normal);
	info.radius = cy->diameter / 2.0f;
	info.oc = vector_sub(ray->origin, cy->center);
	info.a = vector_dot(ray->direction, ray->direction)
		- powf(vector_dot(ray->direction, info.normal), 2);
	info.b = 2 * (vector_dot(ray->direction, info.oc)
			- (vector_dot(ray->direction, info.normal)
				* vector_dot(info.oc, info.normal)));
	info.c = vector_dot(info.oc, info.oc)
		- powf(vector_dot(info.oc, info.normal), 2)
		- info.radius * info.radius;
	info.discri = info.b * info.b - 4 * info.a * info.c;
	side_t = hit_main(cy, ray, &info);
	caps_t = hit_caps(cy, ray, &info);
	if (side_t > 0 && (caps_t < 0 || side_t < caps_t))
		return (side_t);
	return (caps_t);
}

__device__ t_vector	cylinder_normal_at_gpu(t_cylinder *cy, t_vector point)
{
	t_vector	normal;
	t_vector	oc;
	float		m;

	normal = vector_normalize(cy->normal);
	oc = vector_sub(point, cy->center);
	m = vector_dot(oc, normal);
	if (m > cy->height - EPSILON)
		return (normal);
	if (m < EPSILON)
		return (vector_mult(normal, -1));
	return (vector_normalize(vector_sub(oc, vector_mult(normal, m))));
}

static __device__ void	fill_co_info_gpu(t_cone *co, t_ray *ray, t_co_info *info)
{
	float	cos2;

	cos2 = cosf(co->angle) * cosf(co->angle);
	info->a = info->dv * info->dv - cos2;
	info->b = 2 * (info->dv * info->ocv
			- cos2 * vector_dot(ray->direction, info->oc));
	info->c = info->ocv * info->ocv
		- cos2 * vector_dot(info->oc, info->oc);
	info->discr = info->b * info->b - 4 * info->a * info->c;
}

static __device__ float	hit_cone_body_gpu(t_cone *co, t_ray *ray)
{
	float		t1;
	float		t2;
	float		m1;
	float		m2;
	t_co_info	info;

	info.oc = vector_sub(ray->origin, co->center);
	info.dv = vector_dot(ray->direction, co->normal);
	info.ocv = vector_dot(info.oc, co->normal);
	fill_co_info_gpu(co, ray, &info);
	if (info.discr < 0)
		return (-1.0f);
	t1 = (-info.b - sqrtf(info.discr)) / (2 * info.a);
	m1 = info.ocv + t1 * info.dv;
	if (t1 <= EPSILON || m1 < 0 || m1 > co->height)
		t1 = -1.0f;
	t2 = (-info.b + sqrtf(info.discr)) / (2 * info.a);
	m2 = info.ocv + t2 * info.dv;
	if (t2 <= EPSILON || m2 < 0 || m2 > co->height)
		t2 = -1.0f;
	if (t1 > 0 && t2 > 0)
		return (fminf(t1, t2));
	if (t1 > 0)
		return (t1);
	return (t2);
}

static __device__ float	hit_cone_cap_gpu(t_cone *co, t_ray *ray)
{
	t_plane		cap;
	t_vector	base;
	t_vector	p;
	float		t;
	float		r;

	base = vector_add(co->center,
			vector_mult(vector_normalize(co->normal), co->height));
	cap.point = base;
	cap.normal = co->normal;
	t = hit_plane_gpu(&cap, ray);
	if (t > 0)
	{
		p = vector_add(ray->origin, vector_mult(ray->direction, t));
		r = tanf(co->angle) * co->height;
		if (vector_length_sq(vector_sub(p, base)) <= r * r)
			return (t);
	}
	return (-1.0f);
}

__device__ float	hit_cone_gpu(t_cone *co, t_ray *ray)
{
	float	t_body;
	float	t_cap;

	t_body = hit_cone_body_gpu(co, ray);
	t_cap = hit_cone_cap_gpu(co, ray);
	if (t_body > 0 && (t_cap < 0 || t_body < t_cap))
		return (t_body);
	return (t_cap);
}

static __device__ t_vector	cone_normal_2_gpu(t_cone *co, t_vector oc, t_vector v, float m)
{
	t_vector	radial;
	t_vector	unit_radial;
	float		cos_angle;
	float		sin_angle;
	t_vector	normal;

	radial = vector_sub(oc, vector_mult(v, m));
	if (vector_length_sq(radial) < EPSILON * EPSILON)
		return (vector_init(1, 0, 0));
	unit_radial = vector_normalize(radial);
	cos_angle = cosf(co->angle);
	sin_angle = sinf(co->angle);
	normal = vector_add(
		vector_mult(unit_radial, sin_angle),
		vector_mult(v, -cos_angle)
	);
	return (vector_normalize(normal));
}

__device__ t_vector	cone_normal_at_gpu(t_cone *co, t_vector point)
{
	t_vector	v;
	t_vector	oc;
	float		m;
	float		k;
	t_vector	normal;

	v = vector_normalize(co->normal);
	oc = vector_sub(point, co->center);
	m = vector_dot(oc, v);
	if (fabsf(m - co->height) < EPSILON)
		return (v);
	k = tanf(co->angle);
	if (m < EPSILON)
		return (cone_normal_2_gpu(co, oc, v, m));
	normal = vector_sub(oc, vector_mult(v, m * (1 + k * k)));
	return (vector_normalize(normal));
}

static __device__ void	cal_point_and_normal_gpu(t_hit_record *record, t_ray *ray, t_minirt_gpu *rt)
{
	record->point = vector_add(ray->origin,
			vector_mult(ray->direction, record->t));

	if (record->type == SPHERE)
	{
		t_sphere *sp = &rt->spheres[record->obj_idx];
		record->normal = sphere_normal_at_gpu(sp, record->point);
	}
	else if (record->type == PLANE)
	{
		t_plane *pl = &rt->planes[record->obj_idx];
		record->normal = pl->normal;
	}
	else if (record->type == CYLINDER)
	{
		t_cylinder *cy = &rt->cylinders[record->obj_idx];
		record->normal = cylinder_normal_at_gpu(cy, record->point);
	}
	else if (record->type == CONE)
	{
		t_cone *co = &rt->cones[record->obj_idx];
		record->normal = cone_normal_at_gpu(co, record->point);
	}
}

__device__ t_hit_record	trace_ray_gpu(t_minirt_gpu *rt, t_ray *ray)
{
	t_hit_record	record;
	float			t;

	record.hit = false;
	record.t = 1e30f;

	for (int i = 0; i < rt->num_sp; ++i)
	{
		t = hit_sphere_gpu(&rt->spheres[i], ray);
		if (t > EPSILON && t < record.t)
		{
			record.hit = true;
			record.t = t;
			record.obj_color = rt->spheres[i].color;
			record.type = SPHERE;
			record.obj_idx = i;
		}
	}
	for (int i = 0; i < rt->num_pl; ++i)
	{
		t = hit_plane_gpu(&rt->planes[i], ray);
		if (t > EPSILON && t < record.t)
		{
			record.hit = true;
			record.t = t;
			record.obj_color = rt->planes[i].color;
			record.type = PLANE;
			record.obj_idx = i;
		}
	}
	for (int i = 0; i < rt->num_cy; ++i)
	{
		t = hit_cylinder_gpu(&rt->cylinders[i], ray);
		if (t > EPSILON && t < record.t)
		{
			record.hit = true;
			record.t = t;
			record.obj_color = rt->cylinders[i].color;
			record.type = CYLINDER;
			record.obj_idx = i;
		}
	}
	for (int i = 0; i < rt->num_co; ++i)
	{
		t = hit_cone_gpu(&rt->cones[i], ray);
		if (t > EPSILON && t < record.t)
		{
			record.hit = true;
			record.t = t;
			record.obj_color = rt->cones[i].color;
			record.type = CONE;
			record.obj_idx = i;
		}
	}
	if (record.hit)
		cal_point_and_normal_gpu(&record, ray, rt);
	return (record);
}

// render_cuda/cuda_renderer.cu

__device__ bool is_in_shadow_gpu(const t_minirt_gpu *scene, t_vector hit_point, t_vector light_pos)
{
	t_ray	shadow_ray;
	float	dist_to_light;
	float	t;

    // 创建射向光源的阴影光线
	shadow_ray.direction = vector_normalize(vector_sub(light_pos, hit_point));
    // 将起点沿光线方向稍微偏移，避免自相交
	shadow_ray.origin = vector_add(hit_point, vector_mult(shadow_ray.direction, EPSILON));
	
    dist_to_light = vector_length(vector_sub(light_pos, hit_point));

	// 检查与所有对象的相交情况
	// 遍历球体
	for (int i = 0; i < scene->num_sp; ++i)
	{
		t = hit_sphere_gpu(&scene->spheres[i], &shadow_ray);
		if (t > 0.0f && t < dist_to_light)
			return (true); // 被遮挡
	}
	// 遍历平面
	for (int i = 0; i < scene->num_pl; ++i)
	{
		t = hit_plane_gpu(&scene->planes[i], &shadow_ray);
		if (t > 0.0f && t < dist_to_light)
			return (true); // 被遮挡
	}
	// 遍历圆柱体
	for (int i = 0; i < scene->num_cy; ++i)
	{
		t = hit_cylinder_gpu(&scene->cylinders[i], &shadow_ray);
		if (t > 0.0f && t < dist_to_light)
			return (true); // 被遮挡
	}
	// 遍历圆锥体
	for (int i = 0; i < scene->num_co; ++i)
	{
		t = hit_cone_gpu(&scene->cones[i], &shadow_ray);
		if (t > 0.0f && t < dist_to_light)
			return (true); // 被遮挡
	}

	return (false); // 未被遮挡
}

// render_cuda/cuda_renderer.cu

// 移植 calculate_ambient_light
__device__ static t_color calculate_ambient_light_gpu(t_ambient ambient, t_color obj_color)
{
	t_color	result;
	result.r = ambient.ratio * ambient.color.r * obj_color.r;
	result.g = ambient.ratio * ambient.color.g * obj_color.g;
	result.b = ambient.ratio * ambient.color.b * obj_color.b;
	return (result);
}

// 移植 calculate_diffuse_light
__device__ static t_color calculate_diffuse_light_gpu(t_light_gpu light, t_vector hit_point,
								t_vector normal, t_color obj_color)
{
	t_vector	light_dir;
	float		light_intensity;
	t_color		result;

	light_dir = vector_normalize(vector_sub(light.position, hit_point));
	light_intensity = fmaxf(0.0f, vector_dot(normal, light_dir)); // fmaxf确保不为负
	light_intensity *= light.ratio;
	
	result.r = light_intensity * obj_color.r * light.color.r;
	result.g = light_intensity * obj_color.g * light.color.g;
	result.b = light_intensity * obj_color.b * light.color.b;
	return (result);
}

// 添加镜面反射计算
__device__ static t_color calculate_specular_light_gpu(t_camera camera, t_light_gpu light, 
								t_vector hit_point, t_vector normal)
{
	t_vector	light_dir;
	t_vector	view_dir;
	t_vector	reflect_dir;
	float		spec_intensity;
	t_color		result;

	light_dir = vector_normalize(vector_sub(light.position, hit_point));
	view_dir = vector_normalize(vector_sub(camera.viewpoint, hit_point));
	reflect_dir = vector_sub(
			vector_mult(normal, 2.0f * vector_dot(normal, light_dir)), light_dir);
	spec_intensity = powf(fmaxf(vector_dot(view_dir, reflect_dir), 0.0f), SHININESS);
	
	result.r = light.ratio * spec_intensity * light.color.r;
	result.g = light.ratio * spec_intensity * light.color.g;
	result.b = light.ratio * spec_intensity * light.color.b;
	return (result);
}


__device__ t_color combine_light_gpu(const t_minirt_gpu *scene, const t_hit_record *hit)
{
	t_color	ambient_color;
	t_color	diffuse_color;
	t_color	specular_color;
	t_color	final_color;
	t_light_gpu *lights_array = (t_light_gpu *)scene->lights;

	// 计算环境光
	ambient_color = calculate_ambient_light_gpu(scene->ambient, hit->obj_color);
	final_color = ambient_color;

	// 遍历所有光源
	for (int i = 0; i < scene->num_lights; ++i)
	{
		// 检查阴影并计算漫反射光和镜面反射光
		if (!is_in_shadow_gpu(scene, hit->point, lights_array[i].position))
		{
			diffuse_color = calculate_diffuse_light_gpu(lights_array[i], hit->point,
					hit->normal, hit->obj_color);
			specular_color = calculate_specular_light_gpu(scene->camera, lights_array[i], 
					hit->point, hit->normal);
			final_color = color_add(final_color, diffuse_color);
			final_color = color_add(final_color, specular_color);
		}
	}

	// 颜色裁剪，防止超过1.0
	final_color.r = fminf(1.0f, final_color.r);
	final_color.g = fminf(1.0f, final_color.g);
	final_color.b = fminf(1.0f, final_color.b);

	return (final_color);
}


//global
// render_cuda/cuda_renderer.cu (续)

__global__ void render_kernel(int *pixel_buffer, t_minirt_gpu *scene)
{
    // 计算当前线程对应的像素坐标 (x, y)
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    // 确保坐标在图像范围内
    if (x >= WIDTH || y >= HEIGHT)
        return;

    // 为每个线程初始化随机种子
    unsigned int seed = ((y * WIDTH + x) * 1103515245U + 12345U) ^ 0xdeadbeef;

    // 实现多重采样抗锯齿 (MSAA)
    t_color pixel_color = {0.0f, 0.0f, 0.0f};

    for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
    {
        // 在像素内生成随机偏移，实现抗锯齿
        float rand_offset_x = cuda_randf(&seed);
        float rand_offset_y = cuda_randf(&seed);
        
        float screen_x = (2.0f * (x + rand_offset_x) / WIDTH - 1.0f) * scene->camera.half_width;
        float screen_y = -(2.0f * (y + rand_offset_y) / HEIGHT - 1.0f) * scene->camera.half_height;

        t_vector screen_point = scene->camera.viewpoint;
        screen_point = vector_add(screen_point, vector_mult(scene->camera.u, screen_x));
        screen_point = vector_add(screen_point, vector_mult(scene->camera.v, screen_y));
        screen_point = vector_add(screen_point, vector_mult(scene->camera.w, -1.0f));

        t_ray ray;
        ray.origin = scene->camera.viewpoint;
        ray.direction = vector_normalize(vector_sub(screen_point, scene->camera.viewpoint));

        // 追踪光线并计算颜色
        t_hit_record hit = trace_ray_gpu(scene, &ray);
		t_color final_color_for_sample = {0.0f, 0.0f, 0.0f};
        if (hit.hit) {
			cal_point_and_normal_gpu(&hit, &ray, scene);
			final_color_for_sample = combine_light_gpu(scene, &hit);
		}
		pixel_color = color_add(pixel_color, final_color_for_sample);
    }
    
    // 平均所有采样的颜色值
    pixel_color = color_div(pixel_color, SAMPLES_PER_PIXEL);
    
    // 将最终颜色写入输出缓冲区
    int color_int = color_to_int(pixel_color);
    pixel_buffer[y * WIDTH + x] = color_int;
}


// run in cpu
static void	prepare_gpu(t_minirt *rt, t_minirt_gpu *host, t_minirt_gpu *device)
{
	t_object	*curr;
	t_light		*light_curr;

	host->ambient = rt->ambient;
	host->camera = rt->camera;

	// 计算light数量
	host->num_lights = 0;
	light_curr = rt->light;
	while (light_curr)
	{
		host->num_lights++;
		light_curr = light_curr->next;
	}

	// 计算对象数量
	host->num_sp = 0;
	host->num_pl = 0;
	host->num_cy = 0;
	host->num_co = 0;

	curr = rt->object;
	while (curr)
	{
		if (curr->type == SPHERE) host->num_sp++;
		else if (curr->type == PLANE) host->num_pl++;
		else if (curr->type == CYLINDER) host->num_cy++;
		else if (curr->type == CONE) host->num_co++;
		curr = curr->next;
	}
	
	// 初始化指针为NULL
	device->spheres = NULL;
	device->planes = NULL;
	device->cylinders = NULL;
	device->cones = NULL;
	device->lights = NULL;
	
	// 分配GPU内存
	if (host->num_sp > 0)
        CUDA_CHECK(cudaMalloc(&device->spheres, sizeof(t_sphere) * host->num_sp));
    if (host->num_pl > 0)
        CUDA_CHECK(cudaMalloc(&device->planes, sizeof(t_plane) * host->num_pl));
    if (host->num_cy > 0)
        CUDA_CHECK(cudaMalloc(&device->cylinders, sizeof(t_cylinder) * host->num_cy));
    if (host->num_co > 0)
        CUDA_CHECK(cudaMalloc(&device->cones, sizeof(t_cone) * host->num_co));
    if (host->num_lights > 0)
        CUDA_CHECK(cudaMalloc(&device->lights, sizeof(t_light_gpu) * host->num_lights));

	// 为临时主机数组分配内存
	t_sphere* spheres_h = NULL;
	t_plane* planes_h = NULL;
	t_cylinder* cylinders_h = NULL;
	t_cone* cones_h = NULL;
	t_light_gpu* lights_h = NULL;
	
	if (host->num_sp > 0)
		spheres_h = (t_sphere*)malloc(sizeof(t_sphere) * host->num_sp);
	if (host->num_pl > 0)
		planes_h = (t_plane*)malloc(sizeof(t_plane) * host->num_pl);
	if (host->num_cy > 0)
		cylinders_h = (t_cylinder*)malloc(sizeof(t_cylinder) * host->num_cy);
	if (host->num_co > 0)
		cones_h = (t_cone*)malloc(sizeof(t_cone) * host->num_co);
	if (host->num_lights > 0)
		lights_h = (t_light_gpu*)malloc(sizeof(t_light_gpu) * host->num_lights);

	// 复制对象数据
	int sp_idx = 0, pl_idx = 0, cy_idx = 0, co_idx = 0;
	curr = rt->object;
	while (curr)
	{
		if (curr->type == SPHERE && spheres_h) 
			spheres_h[sp_idx++] = *(t_sphere*)curr->obj;
		if (curr->type == PLANE && planes_h) 
			planes_h[pl_idx++] = *(t_plane*)curr->obj;
		if (curr->type == CYLINDER && cylinders_h) 
			cylinders_h[cy_idx++] = *(t_cylinder*)curr->obj;
		if (curr->type == CONE && cones_h) 
			cones_h[co_idx++] = *(t_cone*)curr->obj;
		curr = curr->next;
	}

	// 复制light数据 - 转换为GPU格式（去掉next指针）
	int light_idx = 0;
	light_curr = rt->light;
	while (light_curr && lights_h)
	{
		lights_h[light_idx].position = light_curr->position;
		lights_h[light_idx].ratio = light_curr->ratio;
		lights_h[light_idx].color = light_curr->color;
		light_idx++;
		light_curr = light_curr->next;
	}
	
	// 将数据复制到GPU
	if (host->num_sp > 0 && spheres_h)
		CUDA_CHECK(cudaMemcpy(device->spheres, spheres_h, sizeof(t_sphere) * host->num_sp, cudaMemcpyHostToDevice));
	if (host->num_pl > 0 && planes_h)
		CUDA_CHECK(cudaMemcpy(device->planes, planes_h, sizeof(t_plane) * host->num_pl, cudaMemcpyHostToDevice));
	if (host->num_cy > 0 && cylinders_h)
		CUDA_CHECK(cudaMemcpy(device->cylinders, cylinders_h, sizeof(t_cylinder) * host->num_cy, cudaMemcpyHostToDevice));
	if (host->num_co > 0 && cones_h)
		CUDA_CHECK(cudaMemcpy(device->cones, cones_h, sizeof(t_cone) * host->num_co, cudaMemcpyHostToDevice));
	if (host->num_lights > 0 && lights_h)
		CUDA_CHECK(cudaMemcpy(device->lights, lights_h, sizeof(t_light_gpu) * host->num_lights, cudaMemcpyHostToDevice));

	// 释放临时主机内存
	if (spheres_h) free(spheres_h);
	if (planes_h) free(planes_h);
	if (cylinders_h) free(cylinders_h);
	if (cones_h) free(cones_h);
	if (lights_h) free(lights_h);
}

extern "C" void render_with_cuda(t_minirt *rt)
{
    printf("Starting CUDA render...\n");
    
    // 1. 准备GPU场景数据
    t_minirt_gpu host_scene;
    t_minirt_gpu device_scene; // 这个结构体本身在CPU，但其指针成员指向GPU内存
    
    printf("Preparing GPU data...\n");
    prepare_gpu(rt, &host_scene, &device_scene);
    
    printf("Objects count: sp=%d, pl=%d, cy=%d, co=%d, lights=%d\n", 
           host_scene.num_sp, host_scene.num_pl, host_scene.num_cy, 
           host_scene.num_co, host_scene.num_lights);
    
    // 复制数量信息到device_scene
    device_scene.num_sp = host_scene.num_sp;
    device_scene.num_pl = host_scene.num_pl;
    device_scene.num_cy = host_scene.num_cy;
    device_scene.num_co = host_scene.num_co;
    device_scene.num_lights = host_scene.num_lights;
    device_scene.ambient = host_scene.ambient;
    device_scene.camera = host_scene.camera;
    
    printf("Copying scene to GPU...\n");
    // 将包含数量信息的场景结构体复制到GPU
    t_minirt_gpu* d_scene;
    CUDA_CHECK(cudaMalloc(&d_scene, sizeof(t_minirt_gpu)));
    CUDA_CHECK(cudaMemcpy(d_scene, &device_scene, sizeof(t_minirt_gpu), cudaMemcpyHostToDevice));

    // 2. 在GPU上为输出图像分配内存
    int *d_pixel_buffer;
    CUDA_CHECK(cudaMalloc(&d_pixel_buffer, WIDTH * HEIGHT * sizeof(int)));

    // 3. 设置内核启动参数
    dim3 threads_per_block(16, 16);
    dim3 num_blocks((WIDTH + 15) / 16, (HEIGHT + 15) / 16);

    // 4. 启动CUDA内核
    printf("Launching CUDA kernel...\n");
    render_kernel<<<num_blocks, threads_per_block>>>(d_pixel_buffer, d_scene);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize()); // 等待内核执行完成
    printf("Kernel finished.\n");

    // 5. 将渲染结果从GPU复制回CPU
    int *h_pixel_buffer = (int *)malloc(WIDTH * HEIGHT * sizeof(int));
    CUDA_CHECK(cudaMemcpy(h_pixel_buffer, d_pixel_buffer, WIDTH * HEIGHT * sizeof(int), cudaMemcpyDeviceToHost));

    // 6. 将结果复制到MLX的图像缓冲区
    memcpy(rt->img.addr, h_pixel_buffer, WIDTH * HEIGHT * sizeof(int));

    // 7. 释放所有内存
    free(h_pixel_buffer);
    CUDA_CHECK(cudaFree(d_pixel_buffer));
    if (device_scene.spheres) CUDA_CHECK(cudaFree(device_scene.spheres));
    if (device_scene.planes) CUDA_CHECK(cudaFree(device_scene.planes));
    if (device_scene.cylinders) CUDA_CHECK(cudaFree(device_scene.cylinders));
    if (device_scene.cones) CUDA_CHECK(cudaFree(device_scene.cones));
    if (device_scene.lights) CUDA_CHECK(cudaFree(device_scene.lights));
    CUDA_CHECK(cudaFree(d_scene));
    
    printf("CUDA render completed.\n");
}

