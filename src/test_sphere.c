#include "minirt.h"

int	color_mult(int color, double factor)
{
	int	r;
	int	g;
	int	b;

	if (factor < 0)
		factor = 0;
	if (factor > 1)
		factor = 1;
	r = (int)(((color >> 16) & 0xFF) * factor);
	g = (int)(((color >> 8) & 0xFF) * factor);
	b = (int)((color & 0xFF) * factor);
	return ((r << 16) | (g << 8) | b);
}

void	test_sphere(void)
{
	t_minirt	rt;
	int			x;
	int			y;

	t_sphere	sp;
	sp.center = vector_init(0, 0, -5);
	sp.radius = 1.0;

	int	color_red;
	color_red = (255 << 16) | (0 << 8) | 0;

	t_light	light;
	light.position = vector_init(-5, 0, 0);
	light.ratio = 1.0;

	t_ray	r;
	r.origin = vector_init(0, 0, 0);

	rt.mlx = mlx_init();
	rt.mlx_win = mlx_new_window(rt.mlx, WIDTH, HEIGHT, "MiniRT");
	rt.img.img = mlx_new_image(rt.mlx, WIDTH, HEIGHT);
	rt.img.addr = mlx_get_data_addr(rt.img.img,
		&rt.img.bits_per_pixel, &rt.img.line_length, &rt.img.endian);

	y = 0;
	x = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			double u = (double)x / (WIDTH - 1);
			double v = (double)y / (HEIGHT - 1);
			r.direction = vector_init(u * 2.0 - 1.0, v * -2.0 + 1.0, -1.0);
			r.direction = vector_normalize(r.direction);

			double t = hit_sphere(&sp, &r);
			if (t > 0.0)
			{
				t_vector	hit_point = vector_add(r.origin, vector_mult(r.direction, t));
				t_vector	normal = sphere_normal_at(&sp, hit_point);
				t_vector	light_dir = vector_normalize(vector_sub(light.position, hit_point));

				double	light_intensity = fmax(0.0, vector_dot(normal, light_dir));

				double	ambient = 0.1;
				light_intensity = ambient + (1.0 - ambient) * light.ratio * light_intensity;

				put_pixel(&rt.img, x, y, color_mult(color_red, light_intensity));
			}
			else
				put_pixel(&rt.img, x, y, 0x000000);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt.mlx, rt.mlx_win, rt.img.img, 0, 0);
	mlx_loop(rt.mlx);
}
