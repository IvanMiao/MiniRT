#include "minirt.h"

int	color_mult(int color, double factor)
{
	t_color	res;

	if (factor < 0)
		factor = 0;
	if (factor > 1)
		factor = 1;
	res.r = (int)(((color >> 16) & 0xFF) * factor);
	res.g = (int)(((color >> 8) & 0xFF) * factor);
	res.b = (int)((color & 0xFF) * factor);
	return ((res.r << 16) | (res.g << 8) | res.b);
}

void	test_sphere(void)
{
	t_minirt	rt;
	int			x;
	int			y;

	t_sphere	sp;
	sp.center = new_vector(0, 0, -5);
	sp.radius = 1.0;

	int	color_red;
	color_red = (255 << 16) | (0 << 8) | 0;

	t_light	light;
	light.position = new_vector(-5, 0, 0);
	light.brightness = 1.0;

	t_ray	r;
	r.origin = new_vector(0, 0, 0);

	rt.mlx = mlx_init();
	rt.mlx_win = mlx_new_window(rt.mlx, WIDTH, HEIGHT, "MiniRT");
	rt.data.img = mlx_new_image(rt.mlx, WIDTH, HEIGHT);
	rt.data.addr = mlx_get_data_addr(rt.data.img,
		&rt.data.bits_per_pixel, &rt.data.line_length, &rt.data.endian);

	y = 0;
	x = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			double u = (double)x / (WIDTH - 1);
			double v = (double)y / (HEIGHT - 1);
			r.direction = new_vector(u * 2.0 - 1.0, v * -2.0 + 1.0, -1.0);
			r.direction = vector_normalize(r.direction);

			double t = hit_sphere(&sp, &r);
			if (t > 0.0)
			{
				t_vector	hit_point = vector_add(r.origin, vector_mult(r.direction, t));
				t_vector	normal = sphere_normal_at(&sp, hit_point);
				t_vector	light_dir = vector_normalize(vector_sub(light.position, hit_point));

				double	light_intensity = fmax(0.0, vector_dot(normal, light_dir));

				double	ambient = 0.1;
				light_intensity = ambient + (1.0 - ambient) * light.brightness * light_intensity;

				put_pixel(&rt.data, x, y, color_mult(color_red, light_intensity));
			}
			else
				put_pixel(&rt.data, x, y, 0x000000);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt.mlx, rt.mlx_win, rt.data.img, 0, 0);
	mlx_loop(rt.mlx);
}
