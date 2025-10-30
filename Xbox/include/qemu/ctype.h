/*
 * QEMU TCG support
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef QEMU_CTYPE_H
#define QEMU_CTYPE_H

#ifndef _CTYPE_H
int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);
#endif

#define qemu_isalnum(c)         isalnum((unsigned char)(c))
#define qemu_isalpha(c)         isalpha((unsigned char)(c))
#define qemu_iscntrl(c)         iscntrl((unsigned char)(c))
#define qemu_isdigit(c)         isdigit((unsigned char)(c))
#define qemu_isgraph(c)         isgraph((unsigned char)(c))
#define qemu_islower(c)         islower((unsigned char)(c))
#define qemu_isprint(c)         isprint((unsigned char)(c))
#define qemu_ispunct(c)         ispunct((unsigned char)(c))
#define qemu_isspace(c)         isspace((unsigned char)(c))
#define qemu_isupper(c)         isupper((unsigned char)(c))
#define qemu_isxdigit(c)        isxdigit((unsigned char)(c))
#define qemu_tolower(c)         tolower((unsigned char)(c))
#define qemu_toupper(c)         toupper((unsigned char)(c))
#define qemu_isascii(c)         isascii((unsigned char)(c))
#define qemu_toascii(c)         toascii((unsigned char)(c))

#endif
