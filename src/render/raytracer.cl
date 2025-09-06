// **************************************************************************** //
//                                                                              //
//                                                         :::      ::::::::    //
//    raytracer.cl                                       :+:      :+:    :+:    //
//                                                     +:+ +:+         +:+      //
//    By: ymiao <ymiao@student.42.fr>                +#+  +:+       +#+         //
//                                                 +#+#+#+#+#+   +#+            //
//    Created: 2025/09/04 21:00:00 by ymiao             #+#    #+#              //
//    Updated: 2025/09/04 21:00:00 by ymiao            ###   ########.fr        //
//                                                                              //
// **************************************************************************** //

#define WIDTH 1600
#define HEIGHT 1200
#define EPSILON 1e-6f
#define MSAA_SAMPLES 256

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
} t_gpu_scene;

typedef struct s_gpu_object
{
	int		type;		// 0=sphere, 1=plane, 2=cylinder
	float	pos[3];		// center/point
	float	dir[3];		// normal/axis
	float	color[3];
	float	radius;		// for sphere/cylinder
	float	height;		// for cylinder
} t_gpu_object;

typedef struct s_ray
{
	float3	origin;
	float3	direction;
} t_ray;

typedef struct s_hit
{
	int		hit;
	float	t;
	float3	point;
	float3	normal;
	float3	color;
} t_hit;

// Vector operations
float3 vec_add(float3 a, float3 b) {
	return (float3)(a.x + b.x, a.y + b.y, a.z + b.z);
}

float3 vec_sub(float3 a, float3 b) {
	return (float3)(a.x - b.x, a.y - b.y, a.z - b.z);
}

float3 vec_mult(float3 v, float s) {
	return (float3)(v.x * s, v.y * s, v.z * s);
}

float vec_dot(float3 a, float3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 vec_normalize(float3 v) {
	float len = sqrt(vec_dot(v, v));
	if (len < EPSILON) return (float3)(0, 0, 0);
	return vec_mult(v, 1.0f / len);
}

// Improved random number generator for MSAA
uint wang_hash(uint seed) {
	seed = (seed ^ 61) ^ (seed >> 16);
	seed *= 9;
	seed = seed ^ (seed >> 4);
	seed *= 0x27d4eb2d;
	seed = seed ^ (seed >> 15);
	return seed;
}

// XorShift random number generator - better quality
uint xorshift32(uint *state) {
	uint x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return x;
}

float random_float(uint *seed) {
	return (float)xorshift32(seed) / (float)UINT_MAX;
}

float random_range(uint *seed, float min, float max) {
	return min + (max - min) * random_float(seed);
}

// Ray-sphere intersection
t_hit intersect_sphere(t_ray ray, __global t_gpu_object *obj) {
	t_hit hit = {0, 0.0f, (float3)(0,0,0), (float3)(0,0,0), (float3)(0,0,0)};
	
	float3 center = (float3)(obj->pos[0], obj->pos[1], obj->pos[2]);
	float3 oc = vec_sub(ray.origin, center);
	
	float a = vec_dot(ray.direction, ray.direction);
	float b = 2.0f * vec_dot(oc, ray.direction);
	float c = vec_dot(oc, oc) - obj->radius * obj->radius;
	
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) return hit;
	
	float t = (-b - sqrt(discriminant)) / (2.0f * a);
	if (t > EPSILON) {
		hit.hit = 1;
		hit.t = t;
		hit.point = vec_add(ray.origin, vec_mult(ray.direction, t));
		hit.normal = vec_normalize(vec_sub(hit.point, center));
		hit.color = (float3)(obj->color[0], obj->color[1], obj->color[2]);
	}
	
	return hit;
}

// Ray-plane intersection
t_hit intersect_plane(t_ray ray, __global t_gpu_object *obj) {
	t_hit hit = {0, 0.0f, (float3)(0,0,0), (float3)(0,0,0), (float3)(0,0,0)};
	
	float3 plane_point = (float3)(obj->pos[0], obj->pos[1], obj->pos[2]);
	float3 plane_normal = vec_normalize((float3)(obj->dir[0], obj->dir[1], obj->dir[2]));
	
	float denom = vec_dot(plane_normal, ray.direction);
	if (fabs(denom) < EPSILON) return hit;
	
	float t = vec_dot(vec_sub(plane_point, ray.origin), plane_normal) / denom;
	if (t > EPSILON) {
		hit.hit = 1;
		hit.t = t;
		hit.point = vec_add(ray.origin, vec_mult(ray.direction, t));
		hit.normal = plane_normal;
		hit.color = (float3)(obj->color[0], obj->color[1], obj->color[2]);
	}
	
	return hit;
}

// Complete cylinder intersection with caps
t_hit intersect_cylinder(t_ray ray, __global t_gpu_object *obj) {
	t_hit hit = {0, 0.0f, (float3)(0,0,0), (float3)(0,0,0), (float3)(0,0,0)};
	t_hit closest_hit = hit;
	float closest_t = INFINITY;
	
	float3 center = (float3)(obj->pos[0], obj->pos[1], obj->pos[2]);
	float3 axis = vec_normalize((float3)(obj->dir[0], obj->dir[1], obj->dir[2]));
	
	// Test cylinder body
	float3 oc = vec_sub(ray.origin, center);
	float3 d_cross_axis = vec_sub(ray.direction, vec_mult(axis, vec_dot(ray.direction, axis)));
	float3 oc_cross_axis = vec_sub(oc, vec_mult(axis, vec_dot(oc, axis)));
	
	float a = vec_dot(d_cross_axis, d_cross_axis);
	float b = 2.0f * vec_dot(oc_cross_axis, d_cross_axis);
	float c = vec_dot(oc_cross_axis, oc_cross_axis) - obj->radius * obj->radius;
	
	float discriminant = b * b - 4 * a * c;
	if (discriminant >= 0) {
		float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
		
		// Check both intersection points
		for (int i = 0; i < 2; i++) {
			float t = (i == 0) ? t1 : t2;
			if (t > EPSILON && t < closest_t) {
				float3 point = vec_add(ray.origin, vec_mult(ray.direction, t));
				float h = vec_dot(vec_sub(point, center), axis);
				
				if (h >= 0 && h <= obj->height) {
					closest_t = t;
					closest_hit.hit = 1;
					closest_hit.t = t;
					closest_hit.point = point;
					float3 axis_point = vec_add(center, vec_mult(axis, h));
					closest_hit.normal = vec_normalize(vec_sub(point, axis_point));
					closest_hit.color = (float3)(obj->color[0], obj->color[1], obj->color[2]);
				}
			}
		}
	}
	
	// Test bottom cap (at center)
	float denom = vec_dot(axis, ray.direction);
	if (fabs(denom) > EPSILON) {
		float t = vec_dot(vec_sub(center, ray.origin), axis) / denom;
		if (t > EPSILON && t < closest_t) {
			float3 point = vec_add(ray.origin, vec_mult(ray.direction, t));
			float3 to_point = vec_sub(point, center);
			float dist_sq = vec_dot(to_point, to_point) - pow(vec_dot(to_point, axis), 2);
			
			if (dist_sq <= obj->radius * obj->radius) {
				closest_t = t;
				closest_hit.hit = 1;
				closest_hit.t = t;
				closest_hit.point = point;
				closest_hit.normal = vec_mult(axis, -1.0f);
				closest_hit.color = (float3)(obj->color[0], obj->color[1], obj->color[2]);
			}
		}
	}
	
	// Test top cap (at center + height * axis)
	float3 top_center = vec_add(center, vec_mult(axis, obj->height));
	if (fabs(denom) > EPSILON) {
		float t = vec_dot(vec_sub(top_center, ray.origin), axis) / denom;
		if (t > EPSILON && t < closest_t) {
			float3 point = vec_add(ray.origin, vec_mult(ray.direction, t));
			float3 to_point = vec_sub(point, top_center);
			float dist_sq = vec_dot(to_point, to_point) - pow(vec_dot(to_point, axis), 2);
			
			if (dist_sq <= obj->radius * obj->radius) {
				closest_t = t;
				closest_hit.hit = 1;
				closest_hit.t = t;
				closest_hit.point = point;
				closest_hit.normal = axis;
				closest_hit.color = (float3)(obj->color[0], obj->color[1], obj->color[2]);
			}
		}
	}
	
	return closest_hit;
}

// Check if a point is in shadow
bool is_in_shadow(float3 point, float3 light_pos, __global t_gpu_object *objects, int num_objects) {
	float3 light_dir = vec_sub(light_pos, point);
	float light_distance = sqrt(vec_dot(light_dir, light_dir));
	light_dir = vec_normalize(light_dir);
	
	// Create shadow ray with appropriate offset to avoid self-intersection
	t_ray shadow_ray;
	shadow_ray.origin = vec_add(point, vec_mult(light_dir, 1e-4f)); // Use same offset as CPU
	shadow_ray.direction = light_dir;
	
	// Check for any intersection between point and light
	for (int i = 0; i < num_objects; i++) {
		t_hit hit;
		
		if (objects[i].type == 0) {  // sphere
			hit = intersect_sphere(shadow_ray, &objects[i]);
		} else if (objects[i].type == 1) {  // plane
			hit = intersect_plane(shadow_ray, &objects[i]);
		} else if (objects[i].type == 2) {  // cylinder
			hit = intersect_cylinder(shadow_ray, &objects[i]);
		} else {
			continue;
		}
		
		// If we hit something closer than the light, we're in shadow
		if (hit.hit && hit.t > 0 && hit.t < light_distance) {
			return true;
		}
	}
	
	return false;
}

// Find closest intersection
t_hit trace_ray(t_ray ray, __global t_gpu_object *objects, int num_objects) {
	t_hit closest_hit = {0, 0.0f, (float3)(0,0,0), (float3)(0,0,0), (float3)(0,0,0)};
	float closest_t = INFINITY;
	
	for (int i = 0; i < num_objects; i++) {
		t_hit hit;
		
		if (objects[i].type == 0) {  // sphere
			hit = intersect_sphere(ray, &objects[i]);
		} else if (objects[i].type == 1) {  // plane
			hit = intersect_plane(ray, &objects[i]);
		} else if (objects[i].type == 2) {  // cylinder
			hit = intersect_cylinder(ray, &objects[i]);
		} else {
			continue;
		}
		
		if (hit.hit && hit.t < closest_t) {
			closest_t = hit.t;
			closest_hit = hit;
		}
	}
	
	return closest_hit;
}

// Lighting calculation with shadows - matches CPU version
float3 calculate_lighting(__global t_gpu_scene *scene, t_hit hit, __global t_gpu_object *objects) {
	float3 light_pos = (float3)(scene->light_pos[0], scene->light_pos[1], scene->light_pos[2]);
	
	// Ambient component - always present
	float3 ambient = (float3)(scene->ambient_color[0], scene->ambient_color[1], scene->ambient_color[2]);
	ambient = vec_mult(ambient, scene->ambient_ratio);
	ambient.x *= hit.color.x;
	ambient.y *= hit.color.y;
	ambient.z *= hit.color.z;
	
	// Initialize final color with ambient
	float3 final_color = ambient;
	
	// Only add diffuse lighting if not in shadow
	if (!is_in_shadow(hit.point, light_pos, objects, scene->num_objects)) {
		float3 light_dir = vec_normalize(vec_sub(light_pos, hit.point));
		float diffuse_factor = fmax(0.0f, vec_dot(hit.normal, light_dir));
		diffuse_factor *= scene->light_ratio;
		
		float3 diffuse_color;
		diffuse_color.x = diffuse_factor * hit.color.x;
		diffuse_color.y = diffuse_factor * hit.color.y;
		diffuse_color.z = diffuse_factor * hit.color.z;
		
		final_color = vec_add(final_color, diffuse_color);
	}
	
	// Clamp to [0, 1] - same as CPU version
	final_color.x = fmin(1.0f, fmax(0.0f, final_color.x));
	final_color.y = fmin(1.0f, fmax(0.0f, final_color.y));
	final_color.z = fmin(1.0f, fmax(0.0f, final_color.z));
	
	return final_color;
}

// Main kernel with MSAA
__kernel void raytracer_kernel(__global int *output, 
							   __global t_gpu_scene *scene,
							   __global t_gpu_object *objects) {
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	if (x >= WIDTH || y >= HEIGHT) return;
	
	// Initialize random seed with better distribution
	uint seed = wang_hash((y * 1920 + x) * 747796405u + 2891336453u);
	
	float3 accumulated_color = (float3)(0.0f, 0.0f, 0.0f);
	
	// Multi-sampling for anti-aliasing
	for (int sample = 0; sample < MSAA_SAMPLES; sample++) {
		// Generate random offsets for this sample
		float offset_x = random_range(&seed, -0.5f, 0.5f);
		float offset_y = random_range(&seed, -0.5f, 0.5f);
		
		// Generate ray with offset - matches CPU version exactly
		float screen_x = (2.0f * (x + 0.5f + offset_x) / WIDTH - 1.0f) * scene->half_width;
		float screen_y = -(2.0f * (y + 0.5f + offset_y) / HEIGHT - 1.0f) * scene->half_height;
		
		float3 camera_pos = (float3)(scene->camera_pos[0], scene->camera_pos[1], scene->camera_pos[2]);
		float3 u = (float3)(scene->camera_u[0], scene->camera_u[1], scene->camera_u[2]);
		float3 v = (float3)(scene->camera_v[0], scene->camera_v[1], scene->camera_v[2]);
		float3 w = (float3)(scene->camera_w[0], scene->camera_w[1], scene->camera_w[2]);
		
		// Calculate screen point first, then direction - same as CPU
		float3 screen_point = camera_pos;
		screen_point = vec_add(screen_point, vec_mult(u, screen_x));
		screen_point = vec_add(screen_point, vec_mult(v, screen_y));
		screen_point = vec_add(screen_point, vec_mult(w, -1.0f));
		
		t_ray ray;
		ray.origin = camera_pos;
		ray.direction = vec_normalize(vec_sub(screen_point, camera_pos));
		
		// Trace ray
		t_hit hit = trace_ray(ray, objects, scene->num_objects);
		
		float3 sample_color = (float3)(0.0f, 0.0f, 0.0f);  // black background
		
		if (hit.hit) {
			sample_color = calculate_lighting(scene, hit, objects);
		}
		
		accumulated_color = vec_add(accumulated_color, sample_color);
	}
	
	// Average the samples
	accumulated_color = vec_mult(accumulated_color, 1.0f / (float)MSAA_SAMPLES);
	
	// Convert to integer color
	int r = (int)(fmin(1.0f, fmax(0.0f, accumulated_color.x)) * 255.0f);
	int g = (int)(fmin(1.0f, fmax(0.0f, accumulated_color.y)) * 255.0f);
	int b = (int)(fmin(1.0f, fmax(0.0f, accumulated_color.z)) * 255.0f);
	
	int color = (r << 16) | (g << 8) | b;
	
	output[y * WIDTH + x] = color;
}
