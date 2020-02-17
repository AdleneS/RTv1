#include "rtv1.h"

void		obj_push(int type, t_obj **list, void *data, t_tex tex)
{
	t_obj *new;

	if (!(new = malloc(sizeof(t_obj))))
		SDL_Error(-1);
	new->type = type;
	new->tex = tex;
	new->oc = (t_vec3df){-1, -1, -1};
	if (new->type == 1)
	{
		new->data = malloc(sizeof(t_sphere));
		*(t_sphere*)new->data = *(t_sphere*)data;
	}
	if (new->type == 2)
	{
		new->data = malloc(sizeof(t_plane));
		*(t_plane*)new->data = *(t_plane*)data;
	}
	if (new->type == 3)
	{
		new->data = malloc(sizeof(t_cylinder));
		*(t_cylinder*)new->data = *(t_cylinder*)data;
	}
	if (new->type == 4)
	{
		new->data = malloc(sizeof(t_cone));
		*(t_cone*)new->data = *(t_cone*)data;
	}
	new->next = !*list ? NULL : *list;
	*list = new;
}

void		light_push(t_light **list, t_light light, t_param *p)
{
	t_light *tmp;
	t_light *new;

	if (!(new = malloc(sizeof(t_light))))
		SDL_Error(-1);
	new->pos = light.pos;
	new->color = light.color;
	new->intensity = light.intensity;
	new->type = light.type;
	new->next = !*list ? NULL : *list;
	*list = new;
}