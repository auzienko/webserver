/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_array.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 10:20:35 by zcris             #+#    #+#             */
/*   Updated: 2021/12/28 08:08:32 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Z_ARRAY_H
# define Z_ARRAY_H

# include <stdlib.h>
# include <unistd.h>

# define Z_ARRAY_EXIT_IF_NULL 1

typedef struct z_array_tag
{
	char	**elem;
	size_t	size;
	int		status;
}	t_z_array;

int		z_array_init(t_z_array *a);
int		z_array_append(t_z_array *a, char *str);
int		z_array_remove(t_z_array *a, size_t index);
int		z_array_null_terminate(t_z_array *a);
int		z_array_free(t_z_array *a);
int		z_array_sort(t_z_array *a, int (f)(const char *, const char *));
void	*z_array_utls_ft_alloc(size_t size);
char	*z_array_utls_ft_strdup(char *s1);
void	z_array_re_init(t_z_array *a);

#endif
