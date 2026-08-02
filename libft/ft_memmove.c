/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:17:11 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/29 21:23:40 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (src > dst)
	{
		ft_memcpy(dst, src, len);
		return (dst);
	}
	else if (src < dst)
	{
		d = (unsigned char *)dst + len;
		s = (const unsigned char *)src + len;
		while (len--)
		{
			*(--d) = *(--s);
		}
		return (dst);
	}
	return (dst);
}

/* int	main(void)
{
	char	str[] = "Hello, World!";

	printf("Before ft_memmove: %s\n", str);
	// Perform memmove operation
	ft_memmove(str + 7, str, 5);
	printf("After ft_memmove: %s\n", str); // Output: "Hello, Hello!"
	return (0);
} */
