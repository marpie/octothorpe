/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include <stdio.h>
#include <string.h>
#ifdef __GNUC__
#include <strings.h>
#endif
#include <stdbool.h>
#include <search.h>
#include <stdlib.h>

#include "fmt_utils.h"
#include "datatypes.h"
#include "oassert.h"
#include "ostrings.h"
#include "dmalloc.h"
#include "strbuf.h"

bool streq (char *s1, char *s2)
{
	return strcmp(s1, s2)==0 ? true : false;
};

bool strieq (char *s1, char *s2)
{
	return strcasecmp(s1, s2)==0 ? true : false;
};

char* str_trim_one_char_right (char *in)
{
	if (strlen(in)==0)
		return in;

	in[strlen(in)-1]=0;
	return in;
};

char str_last_char (const char *s)
{
	return s[strlen(s)-1];
};

char* str_trim_all_lf_cr_right (char *in)
{
	size_t slen=strlen(in);
	if (slen==0)
		return in;
	int last_char_pos=slen-1;

	if (in[last_char_pos]=='\n' || in[last_char_pos]=='\r')
	{
		in[last_char_pos]=0;
		str_trim_all_lf_cr_right(in);
	};

	return in;
};

char *remove_char_begin_end_if_present (char *s, char c)
{
	size_t slen=strlen(s);
	if (slen==0)
		return s;
	int last_char_pos=slen-1;
	if (s[0]==c && s[last_char_pos]==c)
	{
		s[last_char_pos]=0;
		strcpy (s, s+1);
	};
	return s;
};

size_t strtol_or_strtoll(const char *nptr, char **endptr, int base)
{
#if __WORDSIZE==64
	return strtoll(nptr, endptr, base);
#else
	return strtol(nptr, endptr, base);
#endif
};

const char *bool_to_string(bool b)
{
	if (b)
		return "true";
	return "false";
};

static int my_strcasecmp (const void *p1, const void *p2)
{
	//printf ("%s() p1=%s p2=%s\n", __FUNCTION__, (char*)p1, *(const char**)p2); // debug
	return strcasecmp (p1, *(const char**)p2);
};

static int my_strcmp (const void *p1, const void *p2)
{
	//printf ("%s() p1=%s p2=%s\n", __FUNCTION__, (char*)p1, *(const char**)p2); // debug
	return strcmp (p1, *(const char**)p2);
};

int find_string_in_array_of_strings(const char *s, const char **array, size_t array_size, 
	bool case_insensitive, bool sorted)
{
	void *found;
	
	if (sorted)
		found=bsearch (s, array, array_size, sizeof(char*), case_insensitive ? my_strcasecmp : my_strcmp);
	else
		found=lfind (s, array, &array_size, sizeof(char*), case_insensitive ? my_strcasecmp : my_strcmp);

	if (found)
		return (const char**)found-array;
	else
		return -1; // string not found
};
/*
FIXME: conflicting with smth
static const char *mon_name[12] = 
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
*/
bool string_is_ends_with (const char *s, const char *ending)
{
	return strcmp (s+strlen(s)-strlen(ending), ending)==0 ? true : false;
};

unsigned str_common_prefix_len (const char *s1, const char *s2)
{
	unsigned i=0;
	for (;s1[i] && s2[i] && s1[i]==s2[i];i++);
	return i;
};

// output: len of buffer including two terminating zeroes
byte* cvt_to_widestr_and_allocate (char *str, size_t *len)
{
	size_t str_len=strlen(str);
	size_t buf_size=(str_len+1)*2;
	byte* ustr=DCALLOC(byte, buf_size, "byte");
	for (size_t i=0; i<str_len; i++)
		ustr[i*2]=str[i];
	*len=buf_size;
	return ustr;
};

// TODO use memmove
// begin/end starts from 0th character
void string_remove_part (char *buf, int begin, int end)
{
	oassert(buf);
	oassert(strlen(buf)>end);
	oassert(strlen(buf)>begin);
	oassert(begin<=end);

	char *rest=DSTRDUP (buf+end+1,"rest");
	strcpy (buf+begin, rest);
	DFREE (rest);
};

// -> "aaaaaa ...(123 skipped)...bbbbbb"
void fprint_shrinked_string (char *s, size_t limit, FILE *f)
{
	strbuf sb=STRBUF_INIT;
	strbuf_addstr(&sb, s);
	strbuf_fprint_short (&sb, limit, f);
	strbuf_deinit(&sb);
};

// "while C99 allows str to be NULL in this case, and gives the return value (as always) as the number of characters that would have been written in case the output string has been large enough."
// http://linux.die.net/man/3/snprintf
char* dmalloc_and_snprintf (const char *fmt, ...)
{
	va_list va;

	va_start (va, fmt);
	size_t s=vsnprintf (NULL, 0, fmt, va);
	va_end (va);

	char *buf=DMALLOC (byte, s+1, "buf");
	va_start (va, fmt);
	vsnprintf (buf, s+1, fmt, va);
	va_end (va);

	return buf;
};

bool is_string_consists_only_of_Latin_letters (char *s)
{
	return strspn (s, "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm")==strlen(s);
};

bool is_string_consists_only_of_hex_digits (char *s)
{
	return strspn (s, "0123456789ABCDEFabcdef")==strlen(s);
};

bool is_string_has_only_one_character_repeating (char *s, char *output)
{
	if (strlen(s)==1)
		return true;
	char *begin=s;
	s++;
	for (; *s; s++)
		if (*s!=*begin)
			return false;

	if (output)
		*output=*begin;
	return true;
};

int my_strnicmp(const char* s1, const char* s2, size_t len)
{
	for (int i=0; i<len; i++)
	{
		if (s1[i]!=s2[i])
			return s1[i]-s2[i];
		if (s1[i]==0)
			return 0;
	};
	return 0;
};
