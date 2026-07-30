/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:11:32 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/08 19:38:14 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*word_dup(const char *s, size_t len);
static int	count_words(const char *s, char c);
static void	free_all(char **arr, int i);
static int	add_word(char **result, const char *s, char c, int *i);

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		step;

	if (!s)
		return (NULL);
	result = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			step = add_word(result, s, c, &i);
			if (!step)
				return (NULL);
			s += step;
		}
		else
			s++;
	}
	result[i] = NULL;
	return (result);
}

static int	add_word(char **result, const char *s, char c, int *i)
{
	int	word_len;

	word_len = 0;
	while (s[word_len] && s[word_len] != c)
		word_len++;
	result[*i] = word_dup(s, word_len);
	if (!result[*i])
		return (free_all(result, *i), 0);
	(*i)++;
	return (word_len);
}

static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*word_dup(const char *s, size_t len)
{
	char	*word;

	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, s, len);
	word[len] = '\0';
	return (word);
}

static void	free_all(char **arr, int i)
{
	while (i >= 0)
		free(arr[i--]);
	free(arr);
}

/* int	main(void)
{
	const char	*stri = "amanheceu  peguei a   viola  ";
	char		**strings;

	strings = ft_split(stri, ' ');
	for (char **s = strings; *s; s++)
	{
		printf("%s\n", *s);
		free(*s);
	}
	free(strings);
	printf("the end");
} */
