/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:45:12 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/01 14:05:31 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	if (needle[0] == '\0')
	{
		return ((char *)haystack);
	}
	while (len && *haystack)
	{
		i = 0;
		while (needle[i] && (needle[i] == haystack[i]) && i < len)
		{
			i++;
		}
		if (needle[i] == '\0')
		{
			return ((char *)haystack);
		}
		haystack++;
		len--;
	}
	return (NULL);
}

/* int main()
{
	const char	*haystack;
	const char	*needle;
	size_t		len;
	const char	*expected;
	char		*result;

	// Test 1
	haystack = "hello world";
	needle = "world";
	len = 11;
	expected = "world";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 1 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 2
	haystack = "hello world";
	needle = "world";
	len = 5;
	expected = NULL;
	result = ft_strnstr(haystack, needle, len);
	printf("Test 2 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 3
	haystack = "hello";
	needle = "lo";
	len = 5;
	expected = "lo";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 3 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 4
	haystack = "hello";
	needle = "world";
	len = 5;
	expected = NULL;
	result = ft_strnstr(haystack, needle, len);
	printf("Test 4 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 5
	haystack = "hello";
	needle = "";
	len = 5;
	expected = "hello";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 5 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 6
	haystack = "";
	needle = "world";
	len = 5;
	expected = NULL;
	result = ft_strnstr(haystack, needle, len);
	printf("Test 6 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 7
	haystack = "";
	needle = "";
	len = 5;
	expected = "";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 7 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 8
	haystack = "haystackneedle";
	needle = "needle";
	len = 15;
	expected = "needle";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 8 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 9
	haystack = "haystackneedle";
	needle = "needle";
	len = 14;
	expected = "needle";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 9 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 10
	haystack = "abcde";
	needle = "c";
	len = 3;
	expected = "cde";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 10 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 11
	haystack = "abcdef";
	needle = "def";
	len = 6;
	expected = "def";
	result = ft_strnstr(haystack, needle, len);
	printf("Test 11 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	// Test 12
	haystack = "abcdef";
	needle = "defg";
	len = 7;
	expected = NULL;
	result = ft_strnstr(haystack, needle, len);
	printf("Test 12 - Searching for \"%s\" in \"%s\" with max length %zu:\n",
			needle, haystack, len);
	printf("Expected: %s\n", expected ? expected : "NULL");
	printf("Result:   %s\n\n", result ? result : "NULL");
	return (0);
}
 */