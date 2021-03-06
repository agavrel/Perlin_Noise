/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 20:32:54 by agrumbac          #+#    #+#             */
/*   Updated: 2017/02/04 03:44:16 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long long	ft_abs(long long n)
{
	if (n > 0)
		return (n);
	else if (n != -9223372036854775807)
		return (-n);
	return (-1);
}
