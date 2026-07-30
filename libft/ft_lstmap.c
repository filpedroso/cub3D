/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:58:32 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/08 15:16:36 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlist;
	t_list	*newnode;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	newlist = NULL;
	while (lst)
	{
		content = f(lst->content);
		if (!content)
		{
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		newnode = ft_lstnew(content);
		if (!newnode)
		{
			ft_lstclear(&newlist, del);
			return (NULL);
		}
		ft_lstadd_back(&newlist, newnode);
		lst = lst->next;
	}
	return (newlist);
}

/* void *to_uppercase(void *content)
{
    char *str = (char *)content;
    char *new_str = malloc(strlen(str) + 1);

    if (!new_str)
        return NULL;

    for (int i = 0; str[i]; i++)
        new_str[i] = toupper(str[i]);
    new_str[strlen(str)] = '\0';  // Null-terminate the new string

    return new_str;
}

// Sample function to delete the content of a node (free the string)
void del(void *content)
{
    free(content);
}

// Helper function to print a list
void print_list(t_list *lst)
{
    while (lst)
    {
        printf("%s\n", (char *)lst->content);
        lst = lst->next;
    }
}

int main()
{
    // Create the original list with some test data
    t_list *original = ft_lstnew(strdup("hello"));
    ft_lstadd_back(&original, ft_lstnew(strdup("world")));
    ft_lstadd_back(&original, ft_lstnew(strdup("libft")));

    printf("Original list:\n");
    print_list(original);
	printf("test_point0");

    // Apply ft_lstmap to transform the list
	printf("test_point1");
    t_list *mapped = NULL;
	printf("test_point2");
	mapped = ft_lstmap(original, &to_uppercase, &del);
	printf("test_point3");

    printf("\nMapped list (after to_uppercase):\n");
    print_list(mapped);

    // Clean up memory
    ft_lstclear(&original, del);  // Clear the original list
    ft_lstclear(&mapped, del);    // Clear the mapped list

    return 0;
} */