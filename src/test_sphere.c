#include "minirt.h"

int	color_mult(int color, double factor)
{
	int r;
	int g;
	int b;

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
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	int		x;
	int		y;

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

	int		image_width = 800;
	int		image_height = 800;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, image_width, image_height, "MiniRT");
	img.img = mlx_new_image(mlx, image_width, image_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	y = 0;
	x = 0;
	while (y < image_height)
	{
		x = 0;
		while (x < image_width)
		{
			double u = (double)x / (image_width - 1);
			double v = (double)y / (image_height - 1);
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

				my_mlx_pixel_put(&img, x, y, color_mult(color_red, light_intensity));
			}
			else
				my_mlx_pixel_put(&img, x, y, 0x000000);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
