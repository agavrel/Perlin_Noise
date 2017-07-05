/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <gmonein@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 15:59:27 by gmonein           #+#    #+#             */
/*   Updated: 2017/07/05 17:49:34 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"


static void	ft_swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/****************/

/*
void perlin_init(int *p, float *gx, float *gy, int size, unsigned seed) {
    int i;
    srand(seed);

	i = -1;
	while (++i < size)
        p[i] = i;
    // Randomly swap values
	i = -1;
    while (++i < size)
	{
        int j = rand() % size;
		ft_swap(&p[i], &p[j]);
    }
    // Precompute table of gradients
	i = -1;
    while (++i < size)
	{
        gx[i] = rand() / (float)RAND_MAX;
        gy[i] = rand() / (float)RAND_MAX;
    }
}

static inline int perlin_grab_q(int *p, int x, int y)
{
    return (p[(y + p[x & 0xff]) & 0xff]); // now need to protect against seg fault by insuring in range of table
}

float perlin_get(int *p, float *gx, float *gy, int size, float x, float y) {
    int qx0 = (int)floorf(x);
    int qx1 = qx0 + 1;
    int qy0 = (int)floorf(y);
    int qy1 = qy0 + 1;

    // Grab the precomputed, random values from the integer plans surrounding this point.

    int q00 = perlin_grab_q(p, qx0, qy0);
    int q01 = perlin_grab_q(p, qx1, qy0);
    int q10 = perlin_grab_q(p, qx0, qy1);
    int q11 = perlin_grab_q(p, qx1, qy1);

    float tx0 = x - floorf(x);
    float tx1 = tx0 - 1;
    float ty0 = y - floorf(y);
    float ty1 = ty0 - 1;

    float v00 = gx[q00]*tx0 + gy[q00]*ty0;
    float v01 = gx[q01]*tx1 + gy[q01]*ty0;
    float v10 = gx[q10]*tx0 + gy[q10]*ty1;
    float v11 = gx[q11]*tx1 + gy[q11]*ty1;

    float wx = (3 - 2*tx0)*tx0*tx0;
    float v0 = v00 - wx*(v00 - v01);
    float v1 = v10 - wx*(v10 - v11);
    float wy = (3 - 2*ty0)*ty0*ty0;

    return (v0 - wy * (v0 - v1));
}

*/

/****************/

void		free_for_all(t_env *env)
{
	int		i;

	i = -1;
	free(env->map[0]);
	free(env->map);
	free(env->pixels);
	SDL_DestroyRenderer(env->render);
	SDL_DestroyWindow(env->win);
	SDL_DestroyTexture(env->texture);
	free(env->args);
	while (++i < env->thread_cnt)
		free(env->thread[i]);
	free(env->thread);
}

static void	put_pix(int *pixels, int clr, int x, int y)
{
	pixels[(int)(WIN_W) * y + x] = clr;
}

/*
static void perlin(t_env *env)
{
    int tx = 2;
    int ty = 1; // standard perlin values for tx and ty
	float f;
    static int initted = 0;
    static float gx[256];
    static float gy[256];
    static int p[256];
	int	color;
	int		x;
	int		y;

	if (!initted) {
        perlin_init(p, gx, gy, 256, 1);
        initted = 1;
    }
	y = -1;
	while (++y < WIN_H)
	{
		x = -1;
		while (++x < WIN_W)
		{
			float x_field = (x + (tx / 2.0)) / 128;
			float y_field = (y + (ty / 2.0)) / 128;
	//		write(1, "a\n", 2);
			f = perlin_get(p, gx, gy, 256, x_field, y_field);
			f = sinf(f * 64) * 128 + 128;
		//	color = (((int)f << 16) | ((int)f << 8) | (int)f);
			color = ((f * 0x10000) + (f * 0x100) + f);
			put_pix(env->pixels, color, x, y);
		//	pixels[y*screen->w+x] = px;


		//	col = ((int)((x / WIN_W) * 0xff0000)) & 0xff0000;
//			put_pix(env->pixels, 0x00FFFF | col, x, y);
		}
	}
//	SDL_Flip(screen);
}
*/

/*
** fade is used to smooth the shapes
** this formula is the original Ken Perlin one's
*/

static double fade(double t)
{
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

/*
** linear interpolation
*/

static double lerp(double t, double a, double b)
{
	return (a + t * (b - a));
}

typedef struct s_vector
{
	double		x;
	double		y;
	double		z;
}				t_vector;

/*
** handles color
** returns originally equals to ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
*/

static double grad(int hash, t_vector v, t_vector u)
{
	const int h = hash & 0xf;		// CONVERT LO 4 BITS OF HASH CODE

	u.x = h < 8 ? v.x : v.y;			// INTO 12 GRADIENT DIRECTIONS.
	if (h < 4)
		u.y = v.y;
	else
		u.y = (h == 12 || h == 14) ? v.x : v.z;
	return (((!(h & 1) << 1) - 1) * u.x + ((!(h & 2) << 1) - 1) * u.y);
}



  static const int p[512] = { 151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
  };

  static const int permutation[512] = { 151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
  };



double noise(t_vector v)
{
	const int tx = (int)floorf(v.x) & 0xff;                  // FIND UNIT CUBE THAT
	const int ty = (int)floorf(v.y) & 0xff;                  // CONTAINS POINT.
	const int tz = (int)floorf(v.z) & 0xff;
	t_vector u;
	int i;

	i = -1;
	while (++i < 256)
		p[256 + i] = p[i] = permutation[i];
	v.x -= floorf(v.x);                                // FIND RELATIVE X,Y,Z
	v.y -= floorf(v.y);                                // OF POINT IN CUBE.
	v.z -= floorf(v.z);

	u.x = fade(v.x),                                // COMPUTE FADE CURVES
	u.y = fade(v.y),                                // FOR EACH OF X,Y,Z.
	u.z = fade(v.z);

	int a = p[tx] + ty;
	int aa = p[a] + tz;
	int ab = p[a + 1] + tz;      // HASH COORDINATES OF
	int b = p[tx + 1] + ty;
	int ba = p[b] + tz;
	int bb = p[a + 1] + tz;      // THE 8 CUBE CORNERS,

	return (lerp(u.z, lerp(u.y, lerp(u.x, grad(p[aa], v, u),  // AND ADD
                 grad(p[ba], (t_vector){v.x - 1, v.y, v.z}, u)), // BLENDED
         lerp(u.x, grad(p[ab], (t_vector){v.x, v.y - 1, v.z}, u),  // RESULTS
                 grad(p[bb], (t_vector){v.x - 1, v.y - 1, v.z}, u))),// FROM  8
 lerp(u.y, lerp(u.x, grad(p[aa + 1], (t_vector){v.x, v.y, v.z - 1}, u),  // CORNERS
                 grad(p[ba + 1], (t_vector){v.x - 1, v.y, v.z - 1}, u)), // OF CUBE
         lerp(u.x, grad(p[ab + 1], (t_vector){v.x, v.y - 1, v.z - 1}, u),
                 grad(p[bb + 1], (t_vector){v.x - 1, v.y - 1, v.z - 1}, u)))));
}


static void perlin(t_env *env)
{
	int y;
	int x;
	int color;
	t_vector v;

	y = -1;
	while (++y < WIN_H)
	{
		x = -1;
		while (++x < WIN_W)
		{
			v.x = x;
			v.y = y;
			v.z = 1;
			color = noise(v);
			put_pix(env->pixels, color, x, y);
		}
	}
}

static int	global_loop(t_env *env)
{
	while (env->run)
	{
		SDL_PollEvent(&env->event);
		if (env->event.type == 256)
			env->run = 0;
		if (env->event.window.type == SDL_WINDOWEVENT_CLOSE
				|| env->event.key.keysym.sym == SDLK_ESCAPE
				|| env->event.type == SDL_QUIT)
			env->run = 0;
		handle_events(env);
		perlin(env);
		SDL_UpdateTexture(env->texture, NULL, env->pixels, (int)WIN_W << 2);
		SDL_RenderCopy(env->render, env->texture, NULL, NULL);
		SDL_RenderPresent(env->render);
		env->key = SDL_GetKeyboardState(NULL);
	}
	free_for_all(env);
	exit(1);
	return (0);
}

int			init(t_env *env)
{
	env->cam.pos_x = 3.5;
	env->cam.pos_y = 3.5;
	env->dir_x = 1;
	env->dir_y = 0;
	env->plane_x = 0;
	env->plane_y = -0.66f;
	env->x = 0;
	env->y = 0;
	env->run = 1;
	env->pal = 0;
	env->blur = 1;
	env->color[0][0] = 0xDEE9ED;
	env->color[0][1] = 0xC6C7CB;
	env->color[0][2] = 0x808387;
	env->color[0][3] = 0x71EBAF;
	env->color[0][4] = 0x42A857;
	env->color[0][5] = 0x332F3B;
	env->thread_cnt = 16;
	if (!(env->args = (t_args *)malloc(sizeof(t_args) * (env->thread_cnt + 1))))
		return (0);
	env->thread = malloc_thread(env->thread_cnt, env->args, env);
	if (!(env->key = SDL_GetKeyboardState(NULL)))
		return (0);
	return (1);
}

int			init_sdl(t_env *env)
{
	if (SDL_Init(SDL_INIT_EVENTS) == -1)
		return (0);
	if (!(env->win = SDL_CreateWindow("Wolf3D", 0, 0, WIN_W, WIN_H, 0)))
		return (0);
	env->render = SDL_CreateRenderer(env->win, 0,
			SDL_RENDERER_TARGETTEXTURE
			| SDL_RENDERER_SOFTWARE
			| SDL_RENDERER_ACCELERATED);
		//	| SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(env->render, 0xC0, 0, 0, 255);
	SDL_RenderClear(env->render);
	SDL_RenderPresent(env->render);
	if (!(env->pixels = (int *)malloc(sizeof(int) * (WIN_H * WIN_W))))
		return (0);
	if (!(env->texture = SDL_CreateTexture(env->render,
					SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
					WIN_W, WIN_H)))
		return (0);
	SDL_SetRenderTarget(env->render, env->texture);
	return (1);
}

int			main(int argc, char **argv)
{
	t_env	env;
	int		**map;
	int		*pixels;

	if (!(env.map = parsing()))
		return (0);
	if (!init(&env))
		return (0);
	if (!init_sdl(&env))
		return (0);
	return (global_loop(&env));
}
