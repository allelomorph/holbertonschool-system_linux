#ifndef LIBASM_H
#define LIBASM_H

/* size_t */
#include <stddef.h>

/* task 0. strlen */
size_t asm_strlen(const char *str);

/* task 1. strcmp */
int asm_strcmp(const char *s1, const char *s2);

/* task 2. strncmp */
int asm_strncmp(const char *s1, const char *s2, size_t n);

/* task 3. strchr */
char *asm_strchr(const char *s, int c);

/* task 4. strstr */
char *asm_strstr(const char *haystack, const char *needle);

/* task 5. memcpy */
void *asm_memcpy(void *dest, const void *src, size_t n);

/* task 6. putc */
size_t asm_putc(int c);

/* task 7. puts */
size_t asm_puts(const char *str);

/* task 8. strcasecmp */
int asm_strcasecmp(const char *s1, const char *s2);

/* task 9. strncasecmp */
int asm_strncasecmp(const char *s1, const char *s2, size_t n);

/* task 10. strspn */
size_t asm_strspn(const char *s, const char *accept);

/* task 11. strcspn */
size_t asm_strcspn(const char *s, const char *reject);

/* task 12. strpbrk */
char *asm_strpbrk(const char *s, const char *accept);

/* task 13. puti */
size_t asm_puti(int n);

/* task 14. puti_base */
size_t asm_puti_base(int n, const char *base);

#endif /* LIBASM_H */
