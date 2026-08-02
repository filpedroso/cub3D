# Libft, a 42 school project #

Libft is a custom C library that replicates essential standard library functions and provides additional utility functions for working with strings, memory, file descriptors, and linked lists.

## Function Overview

### Character and String Checks
- **`ft_isalpha`**: Checks if a character is alphabetic.
- **`ft_isdigit`**: Checks if a character is a digit.
- **`ft_isalnum`**: Checks if a character is alphanumeric.
- **`ft_isascii`**: Checks if a character is an ASCII value.
- **`ft_isprint`**: Checks if a character is printable.

### String Manipulation
- **`ft_strlen`**: Calculates the length of a string.
- **`ft_strlcpy`**: Copies a string to a buffer with size limitation.
- **`ft_strlcat`**: Concatenates strings with buffer size control.
- **`ft_strchr`**: Finds the first occurrence of a character in a string.
- **`ft_strrchr`**: Finds the last occurrence of a character in a string.
- **`ft_strncmp`**: Compares two strings up to `n` characters.
- **`ft_strnstr`**: Locates a substring within a string up to a specified length.
- **`ft_strdup`**: Duplicates a string by allocating memory for it.
- **`ft_substr`**: Extracts a substring from a string.
- **`ft_strjoin`**: Joins two strings into a new string.
- **`ft_strtrim`**: Trims specified characters from both ends of a string.
- **`ft_split`**: Splits a string into an array of substrings based on a delimiter.

### Case Conversion
- **`ft_toupper`**: Converts a lowercase character to uppercase.
- **`ft_tolower`**: Converts an uppercase character to lowercase.

### Memory Manipulation
- **`ft_memset`**: Fills memory with a constant byte.
- **`ft_bzero`**: Sets all bytes in a memory area to zero.
- **`ft_memcpy`**: Copies memory from source to destination.
- **`ft_memmove`**: Moves memory safely between overlapping areas.
- **`ft_memchr`**: Locates a byte in a memory area.
- **`ft_memcmp`**: Compares two memory areas.

### Conversion
- **`ft_atoi`**: Converts a string to an integer.
- **`ft_itoa`**: Converts an integer to a string.

### Memory Allocation
- **`ft_calloc`**: Allocates and zero-initializes memory for an array.

### String Iteration and Mapping
- **`ft_strmapi`**: Applies a function to each character of a string, returning a new string.
- **`ft_striteri`**: Applies a function to each character of a string in-place.

### File Descriptor Output
- **`ft_putchar_fd`**: Writes a character to a specified file descriptor.
- **`ft_putstr_fd`**: Writes a string to a specified file descriptor.
- **`ft_putendl_fd`**: Writes a string followed by a newline to a file descriptor.
- **`ft_putnbr_fd`**: Writes an integer to a specified file descriptor.

### Linked List Utilities
- **`ft_lstnew`**: Creates a new linked list element.
- **`ft_lstadd_front`**: Adds an element to the start of the list.
- **`ft_lstsize`**: Returns the size of the list.
- **`ft_lstlast`**: Returns the last element of the list.
- **`ft_lstadd_back`**: Adds an element to the end of the list.
- **`ft_lstdelone`**: Deletes a single element from the list.
- **`ft_lstclear`**: Clears and frees all elements in the list.
- **`ft_lstiter`**: Applies a function to each element of the list.
- **`ft_lstmap`**: Applies a function to each element, creating a new list with the results.

---

## Usage

Libft is designed as a foundational library in C programming, enhancing the standard library functions and providing useful additional functionality. To use it, include `libft.h` and link the compiled library in your C project.

--- 

### License

Libft is created as part of the 42 School curriculum and is intended for educational use.
