/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 20:23:14 by agrumbac          #+#    #+#             */
/*   Updated: 2016/11/29 15:27:21 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_sqrt(unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i * i < n)
		i++;
	return (i);
}
