#include "multithreading.h"
/* perror */
#include <stdio.h>
/* errno */
#include <errno.h>
/* pthread_kill SIGTERM */
#include <signal.h>
/* malloc free */
#include <stdlib.h>

#define THREAD_POOL_SZ 10


/**
 * blurPortionThreadEntry - thread entry function for applyinh a Gaussian Blur
 *   to a portion of a .ppm image
 *
 * @portion: pointer to profile of portion of .ppm image to be blurred
 * Return: always NULL, `void *` expected by pthread_create param prototype
 */
void *blurPortionThreadEntry(blur_portion_t *portion)
{
	if (!portion)
		pthread_exit(NULL);

	blur_portion(portion);

	pthread_exit(NULL);
}


/**
 * portionImage - divides pixel coordinate field of a .ppm image into
 *   horizontal strips and stores portion profiles in array of blur_portion_t
 *   structs
 *
 * @img_blur: pointer to blurred image
 * @img: pointer to source image
 * @kernel: pointer to the convolution kernel to use for Gaussian blur
 * @portion_ct: number of portions to divide pixel coordinates into
 * Return: array of portion profiles, or NULL on failure
 */
blur_portion_t *portionImage(img_t *img_blur, const img_t *img,
			     const kernel_t *kernel, size_t portion_ct)
{
	blur_portion_t *portions = NULL;
	size_t i, extra_rows, next_y;

	if (!img_blur || !img || !kernel ||
	    !img_blur->pixels || !img->pixels || !kernel->matrix)
		return (NULL);

	portions = malloc(sizeof(blur_portion_t) * portion_ct);
	if (!portions)
		return (NULL);

	/* assign portions by dividing image into horizontal strips */
	for (i = 0, next_y = 0, extra_rows = img->h % portion_ct;
	     i < portion_ct; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;

		portions[i].x = 0;
		portions[i].y = next_y;
		portions[i].w = img->w;
		portions[i].h = img->h / portion_ct;
		if (extra_rows)
		{
			(portions[i].h)++;
			extra_rows--;
		}

		portions[i].kernel = kernel;

		next_y += portions[i].h;
	}

	return (portions);
}


/**
 * blur_image - blurs the entirety of an image using a Gaussian Blur
 *
 * @img_blur: pointer to blurred image
 * @img: pointer to source image
 * @kernel: pointer to the convolution kernel to use
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t t_ids[THREAD_POOL_SZ] = {0};
	blur_portion_t *portions = NULL;
	pthread_attr_t attr;
	size_t i, j;

	if (!img_blur || !img || !kernel ||
	    !img_blur->pixels || !img->pixels || !kernel->matrix)
		return;
	pthread_attr_init(&attr); /* explicit attribute for portabiliity */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	portions = portionImage(img_blur, img, kernel, THREAD_POOL_SZ);
	if (!portions)
	{
		fprintf(stderr, "blur_image: portionImage failed\n");
		return;
	}
	for (i = 0; i < THREAD_POOL_SZ; i++)
	{
		if (pthread_create(t_ids + i, &attr,
				   (void *(*)(void *))blurPortionThreadEntry,
				   portions + i) != 0)
		{
			fprintf(stderr, "blur_image: pthread_create failed\n");
			for (j = 0; j < i; j++)
				pthread_kill(t_ids[j], SIGTERM);
			free(portions);
			return;
		}
	}
	pthread_attr_destroy(&attr);
	for (i = 0; i < THREAD_POOL_SZ; i++)
	{
		if (pthread_join(t_ids[i], NULL) != 0)
		{
			fprintf(stderr, "blur_image: pthread_join failed\n");
			free(portions);
			return;
		}
	}
	free(portions);
}
