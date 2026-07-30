/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:53:44 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/31 15:15:26 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	while (n--)
	{
		if (*ptr1 != *ptr2)
		{
			return (*ptr1 - *ptr2);
		}
		ptr1++;
		ptr2++;
	}
	return (0);
}

/* int main(void)
{
    unsigned char arr1[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    unsigned char arr2[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    unsigned char arr3[] = {0x01, 0x02, 0x03, 0xFF, 0x05};
    unsigned char arr4[] = {0x01, 0x02, 0x03, 0x04, 0x06};
    size_t n;
    int result;

    // Test 1: Compare identical arrays
    n = 5;
    result = ft_memcmp(arr1, arr2, n);
    printf("Test 1: %d\n", result); // Expected: 0

    // Test 2: Compare different arrays within the first n bytes
    n = 5;
    result = ft_memcmp(arr1, arr3, n);
    printf("Test 2: %d\n", result); // Expected: non-zero

    // Test 3: Compare different arrays with n = 3
    n = 3;
    result = ft_memcmp(arr1, arr3, n);
    printf("Test 3: %d\n", result); // Expected: 0

    // Test 4: Compare different arrays with n = 4
    n = 4;
    result = ft_memcmp(arr3, arr4, n);
    printf("Test 4: %d\n", result); // Expected: non-zero

    // Test 5: Compare different arrays with n = 5
    n = 5;
    result = ft_memcmp(arr1, arr4, n);
    printf("Test 5: %d\n", result); // Expected: non-zero

    // Test 6: Compare with n = 0
    n = 0;
    result = ft_memcmp(arr1, arr3, n);
    printf("Test 6: %d\n", result); // Expected: 0

    return (0);
} */