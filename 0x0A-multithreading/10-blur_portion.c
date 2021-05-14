#include "multithreading.h"

#include <stdio.h>

void blurPixel(const img_t *img, img_t *img_blur, const kernel_t *kernel,
	       size_t px_x, size_t px_y)
{
	size_t i, j,/* x, y,*/ k_radius, px_i;
	float r_total = 0, g_total = 0, b_total = 0, weight = 0;
	ssize_t x, y;
/*
	float r_delta, g_delta, b_delta;
*/
	if (!img || !img_blur || !kernel)
		return;

	if (px_x > img->w || px_y > img->h)
		return;

	k_radius = kernel->size / 2;
	for (i = 0, y = (ssize_t)px_y - k_radius; i < kernel->size; i++, y++)
	{
		for (j = 0, x = (ssize_t)px_x - k_radius; j < kernel->size; j++, x++)
		{
/*
			printf("\ti:%lu y:%li j:%lu x:%li kernel->matrix[i][j]:%f\n", i, y, j, x, kernel->matrix[i][j]);
*/
			if ((x >= 0 && (size_t)x < img->w) &&
			    (y >= 0 && (size_t)y < img->h))
			{
				weight += kernel->matrix[i][j];
				px_i = (y * img->w) + x;
/*
				printf("\toriginal image px_i:%lu R:%i G:%i B:%i\n", px_i, (img->pixels + px_i)->r, (img->pixels + px_i)->g, (img->pixels + px_i)->b);
*/

				r_total += (img->pixels + px_i)->r * kernel->matrix[i][j];
				g_total += (img->pixels + px_i)->g * kernel->matrix[i][j];
				b_total += (img->pixels + px_i)->b * kernel->matrix[i][j];
/*
				r_delta = (img->pixels + px_i)->r * kernel->matrix[i][j];
				g_delta = (img->pixels + px_i)->g * kernel->matrix[i][j];
				b_delta = (img->pixels + px_i)->b * kernel->matrix[i][j];

				printf("\tmodified: R:%f G:%f B:%f\n", r_delta, g_delta, b_delta);

				r_total += r_delta;
				g_total += g_delta;
				b_total += b_delta;
*/
			}
		}
/*
		printf("\n");
*/
	}

	px_i = (px_y * img->w) + px_x;
/*
	printf("setting img_blur->pixels[%lu] R to r_total:%f / weight:%f = %f or %02x\n", px_i, r_total, weight, r_total / weight, (uint8_t)(r_total / weight));
	printf("setting img_blur->pixels[%lu] G to g_total:%f / weight:%f = %f or %02x\n", px_i, g_total, weight, g_total / weight, (uint8_t)(g_total / weight));
	printf("setting img_blur->pixels[%lu] B to b_total:%f / weight:%f = %f or %02x\n", px_i, b_total, weight, b_total / weight, (uint8_t)(b_total / weight));
	printf("\n");
	printf("\n");
*/
	(img_blur->pixels + px_i)->r = (uint8_t)(r_total / weight);
	(img_blur->pixels + px_i)->g = (uint8_t)(g_total / weight);
	(img_blur->pixels + px_i)->b = (uint8_t)(b_total / weight);
}


/**
 * blur_portion - blurs a portion of an image using a Gaussian Blur
 *
 * @portion: pointer to struct containing information needed to perform a blur
 *   operation on a portion of an image file
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t i, j, x_end, y_end;

	if (!portion || !portion->img || !portion->img_blur ||
	    !portion->kernel || !portion->kernel->matrix)
		return;

	if (!portion->w || !portion->h ||
	    !(portion->kernel->size > 1 && portion->kernel->size % 2))
		return;

	printf("img @ %p\n", (void *)portion->img);
	if (portion->img)
		printf("img->w:%lu img->h:%lu\n", portion->img->w, portion->img->h);

	printf("img_blur @ %p\n", (void *)portion->img_blur);
	if (portion->img_blur)
		printf("img_blur->w:%lu img_blur->h:%lu\n", portion->img_blur->w, portion->img_blur->h);

	printf("x:%lu y:%lu\n", portion->x, portion->y);
	printf("w:%lu h:%lu\n", portion->w, portion->h);

	printf("kernel @ %p\n", (void *)portion->kernel);
	if (portion->kernel)
	{
		printf("\tkernel->size:%lu\n", portion->kernel->size);

		for (i = 0; i < portion->kernel->size; i++)
		{
			putchar('\t');
			for (j = 0; j < portion->kernel->size; j++)
				printf("%#f ", portion->kernel->matrix[i][j]);
			putchar('\n');
		}
	}

	y_end = portion->y + portion->h;
	x_end = portion->x + portion->w;
	for (i = portion->y; i < y_end; i++)
	{
		for (j = portion->x; j < x_end; j++)
		{
			blurPixel(portion->img, portion->img_blur,
				  portion->kernel, j, i);
		}
	}
}
