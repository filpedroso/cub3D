/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:27:28 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/05 19:19:26 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_s;
	unsigned int	i;
	size_t			len;

	len = ft_strlen(s);
	new_s = (char *)malloc(len + 1);
	if (!new_s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_s[i] = f(i, s[i]);
		i++;
	}
	new_s[len] = '\0';
	return (new_s);
}

/* #include <stdio.h>
#include "libft.h"
#include <ctype.h>
#include <stdlib.h>

// Example transformation functions

// Convert each character to uppercase
char	to_uppercase(unsigned int i, char c)
{
	(void)i; // Ignore the index if not needed
	return (char)toupper((unsigned char)c);
}

// Shift each character by its index value
char	shift_by_index(unsigned int i, char c)
{
	return (char)(c + i);
}

// Toggle case of each character
char	toggle_case(unsigned int i, char c)
{
	(void)i; // Ignore the index
	if (islower((unsigned char)c))
		return (char)toupper((unsigned char)c);
	else if (isupper((unsigned char)c))
		return (char)tolower((unsigned char)c);
	return (c);
}

// Main function to test ft_strmapi
int	main(void)
{
	const char *str = "Hello World!";
	char *result;

	// Test 1: Convert to uppercase
	result = ft_strmapi(str, to_uppercase);
	printf("Uppercase: %s\n", result);
	free(result);

	// Test 2: Shift each character by its index
	result = ft_strmapi(str, shift_by_index);
	printf("Shift by index: %s\n", result);
	free(result);

	// Test 3: Toggle case of each character
	result = ft_strmapi(str, toggle_case);
	printf("Toggle case: %s\n", result);
	free(result);

	return (0);
} */
