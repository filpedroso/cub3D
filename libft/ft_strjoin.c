/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:51:15 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/04 18:52:18 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	size_t	slen1;
	size_t	slen2;

	slen1 = ft_strlen(s1);
	slen2 = ft_strlen(s2);
	s3 = malloc(slen1 + slen2 + 1);
	if (!s3)
		return (NULL);
	ft_memcpy(s3, s1, slen1);
	ft_memcpy(&s3[slen1], s2, slen2);
	s3[slen1 + slen2] = '\0';
	return (s3);
}
/* int main()
{
	const char *s1 = "paulo";
	const char *s2 = "emilio";
	ft_strjoin(s1, s2);
	printf("%s %s", s1, s2);
} */