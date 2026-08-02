/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:40:21 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/08 15:15:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (ft_strchr(set, s1[start]) && s1[start])
	{
		start++;
	}
	end = ft_strlen(s1);
	while (start < end && ft_strchr(set, s1[end - 1]))
	{
		end--;
	}
	trimmed = ft_substr(s1, start, end - start);
	return (trimmed);
}

/* int main()
{
	char	*cu;
	char	*set;
	char	*trimmed;

	cu = "$#%&   Amarante 2025&ˆ%&   &% $#";
	set = "#$%^ &ˆ";
	trimmed = ft_strtrim(cu, set);
	printf("%s ", trimmed);
}
 */