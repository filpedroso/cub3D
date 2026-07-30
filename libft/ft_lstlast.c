/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:59:40 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/07 16:12:20 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*ptr;

	if (lst == NULL)
		return (NULL);
	ptr = lst;
	while (ptr->next)
	{
		ptr = ptr->next;
	}
	return (ptr);
}

/* int	main(void)
{
	t_list	*lst;
	t_list	*new;
	t_list	*current;
	t_list	*next;
	int		*first_val;
	int		*j;
	int		i;
	int		first_value;
	int		last_value;

	i = 0;
	first_val = malloc(sizeof(int));
	*first_val = i;
	lst = ft_lstnew(first_val);
	while (i < 10)
	{
		printf("%i\n", *(int *)lst->content);
		j = malloc(sizeof(int));
		*j = i * 2;
		new = ft_lstnew(j);
		ft_lstadd_front(&lst, new);
		i++;
	}
	first_value = *(int *)(lst->content);
	last_value = *(int *)(ft_lstlast(lst))->content;
	printf("first: %i, last: %i\n", first_value, last_value);
	current = lst;
	while (current)
	{
		printf("%i\n", *(int *)current->content);
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	return (0);
} */
