#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct coll_elem_s {
	unsigned short int weights[3];
	bool variable;
} coll_elem_t;

typedef struct coll_key_s {
	unsigned short int n;
	struct coll_key_s *prev;
	struct coll_key_s *next;
} coll_key_t;

bool isVarElem(unsigned short int elem_first4);
void setCollElem(coll_elem_t *elem, char c);
int _strcoll(const char *s1, const char *s2);
coll_key_t *addCollKeyNode(coll_key_t **head);
void freeCollKey(coll_key_t **head);
coll_key_t *buildCollKey(const char *s);


int _strcoll(const char *s1, const char *s2)
{
	coll_key_t *ckey_s1 = NULL, *ckey_s2 = NULL,
		*temp1 = NULL, *temp2 = NULL;
	int diff;

	/* build collation keys */
	ckey_s1 = buildCollKey(s1);
	ckey_s2 = buildCollKey(s2);
/*
	printf("\tafter trim:\n");
	printf("\tcolkey_s1: ");
	temp1 = ckey_s1;
	while (temp1)
	{
		printf("%04X ", temp1->n);
		temp1 = temp1->next;
	}
	putchar('\n');

	printf("\tcolkey_s2: ");
	temp1 = ckey_s2;
	while (temp1)
	{
		printf("%04X ", temp1->n);
		temp1 = temp1->next;
	}
	putchar('\n');
*/
	/* compare collation keys */
	temp1 = ckey_s1;
	temp2 = ckey_s2;
	while (temp1 && temp2 && temp1->n == temp2->n)
	{
		temp1 = temp1->next;
		temp2 = temp2->next;
	}

	if (temp1 && temp2)
		diff = (int)(temp1->n - temp2->n);
	else if (temp1)
		diff = 1;
	else if (temp2)
		diff = -1;
	else
		diff = 0;

	freeCollKey(&ckey_s1);
	freeCollKey(&ckey_s2);

	return (diff);
}


int main(void)
{
	char *strs[15] = {
		"+", "++", "+++", "0", "+0", "++0", "+0+",
		"0+", "0++", "00", "+00", "0+0", "00+", "000", NULL
	};
	char *strs2[] = {
		"death", "deluge", "de luge", "de-luge", "deLuge",
		"de Luge", "de-Luge", "demark", NULL
	};
	int i, diff;

	for (i = 0; strs[i + 1]; i++)
	{
		diff = _strcoll(strs[i], strs[i + 1]);
		printf("_strcoll(\"%s\", \"%s\"):%i\n",
		       strs[i], strs[i + 1], diff);
	}

	for (i = 0; strs2[i + 1]; i++)
	{
		diff = _strcoll(strs2[i], strs2[i + 1]);
		printf("_strcoll(\"%s\", \"%s\"):%i\n",
		       strs2[i], strs2[i + 1], diff);
	}

	return (0);

}

/*
  +    0000 0000 0000 0666
  ++   0000 0000 0000 0666 0666
  +++  0000 0000 0000 0666 0666 0666
  0    1F98 0000 0020 0000 0002
  +0   1F98 0000 0020 0000 0002 0000 0666 (trailing FFFF cut)
  ++0  1F98 0000 0020 0000 0002 0000 0666 0666 (trailing FFFF cut)
  +0+  1F98 0000 0020 0000 0002 0000 0666 FFFF 0666
  0+   1F98 0000 0020 0000 0002 0000 FFFF 0666
  0++  1F98 0000 0020 0000 0002 0000 FFFF 0666 0666
  00   1F98 1F98 0000 0020 0020 0000 0002 0002
  +00  1F98 1F98 0000 0020 0020 0000 0002 0002 0000 0666 (trailing FFFF FFFF cut)
  0+0  1F98 1F98 0000 0020 0020 0000 0002 0002 0000 FFFF 0666 (trailing FFFF cut)
  00+  1F98 1F98 0000 0020 0020 0000 0002 0002 0000 FFFF FFFF 0666
  000  1F98 1F98 1F98 0000 0020 0020 0020 0000 0002 0002 0002
*/

coll_key_t *buildCollKey(const char *s)
{
	coll_elem_t *elems = NULL;
	coll_key_t *key = NULL, *temp = NULL, *trim = NULL;
	int len, level, i;
	bool variable_elem_found = false;

	if (!s)
		return (NULL);

	len = strlen(s);
        elems = malloc(sizeof(coll_elem_t) * len);
	if (!elems)
		return (NULL);

	/* populate element arrays */
	for (i = 0; i < len; i++)
		setCollElem(elems + i, s[i]);

	/* build collation keys */
	for (level = 1; level < 5; level++)
	{
		if (level == 4 && !variable_elem_found)
			break;

		if (level != 1)
		{
			temp = addCollKeyNode(&key);
			temp->n = 0;
		}

		for (i = 0; i < len; i++)
		{
			if (level == 4)
			{
				temp = addCollKeyNode(&key);
				if (elems[i].variable)
					temp->n = elems[i].weights[0];
				else
					temp->n = 0xFFFF;
			}
			else
			{
				if (!elems[i].variable)
				{
					temp = addCollKeyNode(&key);
					temp->n = elems[i].weights[level - 1];
				}
				else if (!variable_elem_found)
					variable_elem_found = true;
			}
 		}
	}

	free(elems);

	/* trim trailing 0xFFFF key segments */
	temp = key;
	while (temp->next)
		temp = temp->next;
	while (temp->n == 0xFFFF)
	{
		trim = temp;
		temp = temp->prev;
		temp->next = NULL;
		free(trim);
	}

	return (key);
}


coll_key_t *addCollKeyNode(coll_key_t **head)
{
	coll_key_t *new = NULL, *temp = NULL;

	new = malloc(sizeof(coll_key_t));
	if (!new)
		return (NULL);

	if (!(*head))
	{
		new->prev = NULL;
		new->next = NULL;
		*head = new;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		new->prev = temp;
		new->next = NULL;
		temp->next = new;
	}

	return (new);
}

void freeCollKey(coll_key_t **head)
{
	coll_key_t *temp1 = NULL, *temp2 = NULL;

	temp1 = *head;

	while (temp1)
	{
		temp2 = temp1->next;
		free(temp1);
		temp1 = temp2;
	}

	*head = NULL;
}



bool isVarElem(unsigned short int elem_first4)
{
	/* Unicode variable weighted chars in ASCII range */
	/* 0020  ; [*0209.0020.0002] # SPACE */
	/* to */
	/* 007E  ; [*0670.0020.0002] # TILDE */
	/* 0024  ; [.1F64.0020.0002] # DOLLAR SIGN */
	if (elem_first4 >= 0x0209 && elem_first4 <= 0x0670)
		return (true);
	else
		return (false);
}


void setCollElem(coll_elem_t *elem, char c)
{
	if (!elem)
		return;

	switch (c)
	{
		/* 0020  ; [*0209.0020.0002] # SPACE */
	case ' ':
		elem->weights[0] = 0x0209;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0021  ; [*0267.0020.0002] # EXCLAMATION MARK */
	case '!':
		elem->weights[0] = 0x0267;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0022  ; [*031D.0020.0002] # QUOTATION MARK */
	case '"':
		elem->weights[0] = 0x031D;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0023  ; [*03AC.0020.0002] # NUMBER SIGN */
	case '#':
		elem->weights[0] = 0x03AC;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0024  ; [.1F64.0020.0002] # DOLLAR SIGN */
	case '$':
		elem->weights[0] = 0x1F64;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0025  ; [*03AD.0020.0002] # PERCENT SIGN */
	case '%':
		elem->weights[0] = 0x03AD;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0026  ; [*03A9.0020.0002] # AMPERSAND */
	case '&':
		elem->weights[0] = 0x03A9;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0027  ; [*0316.0020.0002] # APOSTROPHE */
	case '\'':
		elem->weights[0] = 0x0316;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0028  ; [*0328.0020.0002] # LEFT PARENTHESIS */
	case '(':
		elem->weights[0] = 0x0328;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0029  ; [*0329.0020.0002] # RIGHT PARENTHESIS */
	case ')':
		elem->weights[0] = 0x0329;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002A  ; [*03A1.0020.0002] # ASTERISK */
	case '*':
		elem->weights[0] = 0x03A1;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002B  ; [*0666.0020.0002] # PLUS SIGN */
	case '+':
		elem->weights[0] = 0x0666;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002C  ; [*0223.0020.0002] # COMMA */
	case ',':
		elem->weights[0] = 0x0223;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002D  ; [*020D.0020.0002] # HYPHEN-MINUS */
	case '-':
		elem->weights[0] = 0x020D;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002E  ; [*027E.0020.0002] # FULL STOP */
	case '.':
		elem->weights[0] = 0x027E;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 002F  ; [*03A6.0020.0002] # SOLIDUS */
	case '/':
		elem->weights[0] = 0x03A6;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0030  ; [.1F98.0020.0002] # DIGIT ZERO */
	case '0':
		elem->weights[0] = 0x1F98;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0031  ; [.1F99.0020.0002] # DIGIT ONE */
	case '1':
		elem->weights[0] = 0x1F99;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0032  ; [.1F9A.0020.0002] # DIGIT TWO */
	case '2':
		elem->weights[0] = 0x1F9A;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0033  ; [.1F9B.0020.0002] # DIGIT THREE */
	case '3':
		elem->weights[0] = 0x1F9B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		break;
		/* 0034  ; [.1F9C.0020.0002] # DIGIT FOUR */
	case '4':
		elem->weights[0] = 0x1F9C;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0035  ; [.1F9D.0020.0002] # DIGIT FIVE */
	case '5':
		elem->weights[0] = 0x1F9D;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0036  ; [.1F9E.0020.0002] # DIGIT SIX */
	case '6':
		elem->weights[0] = 0x1F9E;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0037  ; [.1F9F.0020.0002] # DIGIT SEVEN */
	case '7':
		elem->weights[0] = 0x1F9F;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0038  ; [.1FA0.0020.0002] # DIGIT EIGHT */
	case '8':
		elem->weights[0] = 0x1FA0;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0039  ; [.1FA1.0020.0002] # DIGIT NINE */
	case '9':
		elem->weights[0] = 0x1FA1;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 003A  ; [*0240.0020.0002] # COLON */
	case ':':
		elem->weights[0] = 0x0240;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 003B  ; [*023A.0020.0002] # SEMICOLON */
	case ';':
		elem->weights[0] = 0x023A;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 003C  ; [*066A.0020.0002] # LESS-THAN SIGN */
	case '<':
		elem->weights[0] = 0x066A;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 003D  ; [*066B.0020.0002] # EQUALS SIGN */
	case '=':
		elem->weights[0] = 0x066B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 003E  ; [*066C.0020.0002] # GREATER-THAN SIGN */
	case '>':
		elem->weights[0] = 0x066C;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 003F  ; [*026D.0020.0002] # QUESTION MARK */
	case '?':
		elem->weights[0] = 0x026D;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0040  ; [*03A0.0020.0002] # COMMERCIAL AT */
	case '@':
		elem->weights[0] = 0x03A0;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0041  ; [.1FA2.0020.0008] # LATIN CAPITAL LETTER A */
	case 'A':
		elem->weights[0] = 0x1FA2;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0042  ; [.1FBC.0020.0008] # LATIN CAPITAL LETTER B */
	case 'B':
		elem->weights[0] = 0x1FBC;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0043  ; [.1FD6.0020.0008] # LATIN CAPITAL LETTER C */
	case 'C':
		elem->weights[0] = 0x1FD6;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0044  ; [.1FEB.0020.0008] # LATIN CAPITAL LETTER D */
	case 'D':
		elem->weights[0] = 0x1FEB;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0045  ; [.2007.0020.0008] # LATIN CAPITAL LETTER E */
	case 'E':
		elem->weights[0] = 0x2007;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0046  ; [.2042.0020.0008] # LATIN CAPITAL LETTER F */
	case 'F':
		elem->weights[0] = 0x2042;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0047  ; [.2051.0020.0008] # LATIN CAPITAL LETTER G */
	case 'G':
		elem->weights[0] = 0x2051;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0048  ; [.2075.0020.0008] # LATIN CAPITAL LETTER H */
	case 'H':
		elem->weights[0] = 0x2075;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0049  ; [.2090.0020.0008] # LATIN CAPITAL LETTER I */
	case 'I':
		elem->weights[0] = 0x2090;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004A  ; [.20AB.0020.0008] # LATIN CAPITAL LETTER J */
	case 'J':
		elem->weights[0] = 0x20AB;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004B  ; [.20C4.0020.0008] # LATIN CAPITAL LETTER K */
	case 'K':
		elem->weights[0] = 0x20C4;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004C  ; [.20D6.0020.0008] # LATIN CAPITAL LETTER L */
	case 'L':
		elem->weights[0] = 0x20D6;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004D  ; [.2109.0020.0008] # LATIN CAPITAL LETTER M */
	case 'M':
		elem->weights[0] = 0x2109;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004E  ; [.2118.0020.0008] # LATIN CAPITAL LETTER N */
	case 'N':
		elem->weights[0] = 0x2118;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 004F  ; [.213C.0020.0008] # LATIN CAPITAL LETTER O */
	case 'O':
		elem->weights[0] = 0x213C;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0050  ; [.216B.0020.0008] # LATIN CAPITAL LETTER P */
	case 'P':
		elem->weights[0] = 0x216B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0051  ; [.2180.0020.0008] # LATIN CAPITAL LETTER Q */
	case 'Q':
		elem->weights[0] = 0x2180;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0052  ; [.2193.0020.0008] # LATIN CAPITAL LETTER R */
	case 'R':
		elem->weights[0] = 0x2193;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0053  ; [.21D2.0020.0008] # LATIN CAPITAL LETTER S */
	case 'S':
		elem->weights[0] = 0x21D2;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0054  ; [.21F7.0020.0008] # LATIN CAPITAL LETTER T */
	case 'T':
		elem->weights[0] = 0x21F7;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0055  ; [.2217.0020.0008] # LATIN CAPITAL LETTER U */
	case 'U':
		elem->weights[0] = 0x2217;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0056  ; [.2247.0020.0008] # LATIN CAPITAL LETTER V */
	case 'V':
		elem->weights[0] = 0x2247;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0057  ; [.2259.0020.0008] # LATIN CAPITAL LETTER W */
	case 'W':
		elem->weights[0] = 0x2259;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0058  ; [.2264.0020.0008] # LATIN CAPITAL LETTER X */
	case 'X':
		elem->weights[0] = 0x2264;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 0059  ; [.2270.0020.0008] # LATIN CAPITAL LETTER Y */
	case 'Y':
		elem->weights[0] = 0x2270;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 005A  ; [.2286.0020.0008] # LATIN CAPITAL LETTER Z */
	case 'Z':
		elem->weights[0] = 0x2286;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0008;
		elem->variable = false;
		break;
		/* 005B  ; [*032A.0020.0002] # LEFT SQUARE BRACKET */
	case '[':
		elem->weights[0] = 0x032A;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 005C  ; [*03A7.0020.0002] # REVERSE SOLIDUS */
	case '\\':
		elem->weights[0] = 0x03A7;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 005D  ; [*032B.0020.0002] # RIGHT SQUARE BRACKET */
	case ']':
		elem->weights[0] = 0x032B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 005E  ; [*04B7.0020.0002] # CIRCUMFLEX ACCENT */
	case '^':
		elem->weights[0] = 0x04B7;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 005F  ; [*020B.0020.0002] # LOW LINE */
	case '_':
		elem->weights[0] = 0x020B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0060  ; [*04B4.0020.0002] # GRAVE ACCENT */
	case '`':
		elem->weights[0] = 0x04B4;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 0061  ; [.1FA2.0020.0002] # LATIN SMALL LETTER A */
	case 'a':
		elem->weights[0] = 0x1FA2;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0062  ; [.1FBC.0020.0002] # LATIN SMALL LETTER B */
	case 'b':
		elem->weights[0] = 0x1FBC;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0063  ; [.1FD6.0020.0002] # LATIN SMALL LETTER C */
	case 'c':
		elem->weights[0] = 0x1FD6;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0064  ; [.1FEB.0020.0002] # LATIN SMALL LETTER D */
	case 'd':
		elem->weights[0] = 0x1FEB;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0065  ; [.2007.0020.0002] # LATIN SMALL LETTER E */
	case 'e':
		elem->weights[0] = 0x2007;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0066  ; [.2042.0020.0002] # LATIN SMALL LETTER F */
	case 'f':
		elem->weights[0] = 0x2042;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0067  ; [.2051.0020.0002] # LATIN SMALL LETTER G */
	case 'g':
		elem->weights[0] = 0x2051;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0068  ; [.2075.0020.0002] # LATIN SMALL LETTER H */
	case 'h':
		elem->weights[0] = 0x2075;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0069  ; [.2090.0020.0002] # LATIN SMALL LETTER I */
	case 'i':
		elem->weights[0] = 0x2090;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006A  ; [.20AB.0020.0002] # LATIN SMALL LETTER J */
	case 'j':
		elem->weights[0] = 0x20AB;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006B  ; [.20C4.0020.0002] # LATIN SMALL LETTER K */
	case 'k':
		elem->weights[0] = 0x20C4;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006C  ; [.20D6.0020.0002] # LATIN SMALL LETTER L */
	case 'l':
		elem->weights[0] = 0x20D6;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006D  ; [.2109.0020.0002] # LATIN SMALL LETTER M */
	case 'm':
		elem->weights[0] = 0x2109;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006E  ; [.2118.0020.0002] # LATIN SMALL LETTER N */
	case 'n':
		elem->weights[0] = 0x2118;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 006F  ; [.213C.0020.0002] # LATIN SMALL LETTER O */
	case 'o':
		elem->weights[0] = 0x213C;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0070  ; [.216B.0020.0002] # LATIN SMALL LETTER P */
	case 'p':
		elem->weights[0] = 0x216B;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0071  ; [.2180.0020.0002] # LATIN SMALL LETTER Q */
	case 'q':
		elem->weights[0] = 0x2180;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0072  ; [.2193.0020.0002] # LATIN SMALL LETTER R */
	case 'r':
		elem->weights[0] = 0x2193;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0073  ; [.21D2.0020.0002] # LATIN SMALL LETTER S */
	case 's':
		elem->weights[0] = 0x21D2;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0074  ; [.21F7.0020.0002] # LATIN SMALL LETTER T */
	case 't':
		elem->weights[0] = 0x21F7;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		/* 0075  ; [.2217.0020.0002] # LATIN SMALL LETTER U */
	break;
	case 'u':
		elem->weights[0] = 0x2217;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0076  ; [.2247.0020.0002] # LATIN SMALL LETTER V */
	case 'v':
		elem->weights[0] = 0x2247;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0077  ; [.2259.0020.0002] # LATIN SMALL LETTER W */
	case 'w':
		elem->weights[0] = 0x2259;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0078  ; [.2264.0020.0002] # LATIN SMALL LETTER X */
	case 'x':
		elem->weights[0] = 0x2264;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 0079  ; [.2270.0020.0002] # LATIN SMALL LETTER Y */
	case 'y':
		elem->weights[0] = 0x2270;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 007A  ; [.2286.0020.0002] # LATIN SMALL LETTER Z */
	case 'z':
		elem->weights[0] = 0x2286;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = false;
		break;
		/* 007B  ; [*032C.0020.0002] # LEFT CURLY BRACKET */
	case '{':
		elem->weights[0] = 0x032C;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 007C  ; [*066E.0020.0002] # VERTICAL LINE */
	case '|':
		elem->weights[0] = 0x066E;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 007D  ; [*032D.0020.0002] # RIGHT CURLY BRACKET */
	case '}':
		elem->weights[0] = 0x032D;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
		/* 007E  ; [*0670.0020.0002] # TILDE */
	case '~':
		elem->weights[0] = 0x0670;
		elem->weights[1] = 0x0020;
		elem->weights[2] = 0x0002;
		elem->variable = true;
		break;
	default:
		elem->weights[0] = elem->weights[1] = elem->weights[2] = 0;
		elem->variable = false;
		break;
	};
/*
	printf("\t\tsetCollElem(%c) elem @ %p: %04X.%04X.%04X variable:%s\n", c, (void *)elem, elem->weights[0], elem->weights[1], elem->weights[2], elem->variable ? "true" : "false");
*/
/*
	printf("\t\tplus sign elem: %04X.%04X.%04X variable:%s\n", 0x0666, 0x0020, 0x0002, "true");
*/
}


/*
0020  ; [*0209.0020.0002] # SPACE		' '
005F  ; [*020B.0020.0002] # LOW LINE		'_'
002D  ; [*020D.0020.0002] # HYPHEN-MINUS	'-'
002C  ; [*0223.0020.0002] # COMMA		','
003B  ; [*023A.0020.0002] # SEMICOLON		';'
003A  ; [*0240.0020.0002] # COLON		':'
0021  ; [*0267.0020.0002] # EXCLAMATION MARK	'!'
003F  ; [*026D.0020.0002] # QUESTION MARK	'?'
002E  ; [*027E.0020.0002] # FULL STOP		'.'
0027  ; [*0316.0020.0002] # APOSTROPHE		'''
0022  ; [*031D.0020.0002] # QUOTATION MARK	'"'
0028  ; [*0328.0020.0002] # LEFT PARENTHESIS	'('
0029  ; [*0329.0020.0002] # RIGHT PARENTHESIS	')'
005B  ; [*032A.0020.0002] # LEFT SQUARE BRACKET	'['
005D  ; [*032B.0020.0002] # RIGHT SQUARE BRACKET']'
007B  ; [*032C.0020.0002] # LEFT CURLY BRACKET	'{'
007D  ; [*032D.0020.0002] # RIGHT CURLY BRACKET	'}'
002A  ; [*03A1.0020.0002] # ASTERISK		'*'
005C  ; [*03A7.0020.0002] # REVERSE SOLIDUS	'\'
0026  ; [*03A9.0020.0002] # AMPERSAND		'&'
0023  ; [*03AC.0020.0002] # NUMBER SIGN		'#'
0025  ; [*03AD.0020.0002] # PERCENT SIGN	'%'
0040  ; [*03A0.0020.0002] # COMMERCIAL AT	'@'
002F  ; [*03A6.0020.0002] # SOLIDUS    		'/'
0060  ; [*04B4.0020.0002] # GRAVE ACCENT	'`'
005E  ; [*04B7.0020.0002] # CIRCUMFLEX ACCENT	'^'
002B  ; [*0666.0020.0002] # PLUS SIGN  		'+'
003C  ; [*066A.0020.0002] # LESS-THAN SIGN	'<'
003D  ; [*066B.0020.0002] # EQUALS SIGN		'='
003E  ; [*066C.0020.0002] # GREATER-THAN SIGN	'>'
007C  ; [*066E.0020.0002] # VERTICAL LINE	'|'
007E  ; [*0670.0020.0002] # TILDE    		'~'
0024  ; [.1F64.0020.0002] # DOLLAR SIGN		'$'
0030  ; [.1F98.0020.0002] # DIGIT ZERO
0031  ; [.1F99.0020.0002] # DIGIT ONE
0032  ; [.1F9A.0020.0002] # DIGIT TWO
0033  ; [.1F9B.0020.0002] # DIGIT THREE
0034  ; [.1F9C.0020.0002] # DIGIT FOUR
0035  ; [.1F9D.0020.0002] # DIGIT FIVE
0036  ; [.1F9E.0020.0002] # DIGIT SIX
0037  ; [.1F9F.0020.0002] # DIGIT SEVEN
0038  ; [.1FA0.0020.0002] # DIGIT EIGHT
0039  ; [.1FA1.0020.0002] # DIGIT NINE
0061  ; [.1FA2.0020.0002] # LATIN SMALL LETTER A
0041  ; [.1FA2.0020.0008] # LATIN CAPITAL LETTER A
0062  ; [.1FBC.0020.0002] # LATIN SMALL LETTER B
0042  ; [.1FBC.0020.0008] # LATIN CAPITAL LETTER B
0063  ; [.1FD6.0020.0002] # LATIN SMALL LETTER C
0043  ; [.1FD6.0020.0008] # LATIN CAPITAL LETTER C
0064  ; [.1FEB.0020.0002] # LATIN SMALL LETTER D
0044  ; [.1FEB.0020.0008] # LATIN CAPITAL LETTER D
0065  ; [.2007.0020.0002] # LATIN SMALL LETTER E
0045  ; [.2007.0020.0008] # LATIN CAPITAL LETTER E
0066  ; [.2042.0020.0002] # LATIN SMALL LETTER F
0046  ; [.2042.0020.0008] # LATIN CAPITAL LETTER F
0067  ; [.2051.0020.0002] # LATIN SMALL LETTER G
0047  ; [.2051.0020.0008] # LATIN CAPITAL LETTER G
0068  ; [.2075.0020.0002] # LATIN SMALL LETTER H
0048  ; [.2075.0020.0008] # LATIN CAPITAL LETTER H
0069  ; [.2090.0020.0002] # LATIN SMALL LETTER I
0049  ; [.2090.0020.0008] # LATIN CAPITAL LETTER I
006A  ; [.20AB.0020.0002] # LATIN SMALL LETTER J
004A  ; [.20AB.0020.0008] # LATIN CAPITAL LETTER J
006B  ; [.20C4.0020.0002] # LATIN SMALL LETTER K
004B  ; [.20C4.0020.0008] # LATIN CAPITAL LETTER K
006C  ; [.20D6.0020.0002] # LATIN SMALL LETTER L
004C  ; [.20D6.0020.0008] # LATIN CAPITAL LETTER L
006D  ; [.2109.0020.0002] # LATIN SMALL LETTER M
004D  ; [.2109.0020.0008] # LATIN CAPITAL LETTER M
006E  ; [.2118.0020.0002] # LATIN SMALL LETTER N
004E  ; [.2118.0020.0008] # LATIN CAPITAL LETTER N
006F  ; [.213C.0020.0002] # LATIN SMALL LETTER O
004F  ; [.213C.0020.0008] # LATIN CAPITAL LETTER O
0070  ; [.216B.0020.0002] # LATIN SMALL LETTER P
0050  ; [.216B.0020.0008] # LATIN CAPITAL LETTER P
0071  ; [.2180.0020.0002] # LATIN SMALL LETTER Q
0051  ; [.2180.0020.0008] # LATIN CAPITAL LETTER Q
0072  ; [.2193.0020.0002] # LATIN SMALL LETTER R
0052  ; [.2193.0020.0008] # LATIN CAPITAL LETTER R
0073  ; [.21D2.0020.0002] # LATIN SMALL LETTER S
0053  ; [.21D2.0020.0008] # LATIN CAPITAL LETTER S
0074  ; [.21F7.0020.0002] # LATIN SMALL LETTER T
0054  ; [.21F7.0020.0008] # LATIN CAPITAL LETTER T
0075  ; [.2217.0020.0002] # LATIN SMALL LETTER U
0055  ; [.2217.0020.0008] # LATIN CAPITAL LETTER U
0076  ; [.2247.0020.0002] # LATIN SMALL LETTER V
0056  ; [.2247.0020.0008] # LATIN CAPITAL LETTER V
0077  ; [.2259.0020.0002] # LATIN SMALL LETTER W
0057  ; [.2259.0020.0008] # LATIN CAPITAL LETTER W
0078  ; [.2264.0020.0002] # LATIN SMALL LETTER X
0058  ; [.2264.0020.0008] # LATIN CAPITAL LETTER X
0079  ; [.2270.0020.0002] # LATIN SMALL LETTER Y
0059  ; [.2270.0020.0008] # LATIN CAPITAL LETTER Y
007A  ; [.2286.0020.0002] # LATIN SMALL LETTER Z
005A  ; [.2286.0020.0008] # LATIN CAPITAL LETTER Z
*/

/*
0020  ; [*0209.0020.0002] # SPACE
0021  ; [*0267.0020.0002] # EXCLAMATION MARK
0022  ; [*031D.0020.0002] # QUOTATION MARK
0023  ; [*03AC.0020.0002] # NUMBER SIGN
0024  ; [.1F64.0020.0002] # DOLLAR SIGN
0025  ; [*03AD.0020.0002] # PERCENT SIGN
0026  ; [*03A9.0020.0002] # AMPERSAND
0027  ; [*0316.0020.0002] # APOSTROPHE
0028  ; [*0328.0020.0002] # LEFT PARENTHESIS
0029  ; [*0329.0020.0002] # RIGHT PARENTHESIS
002A  ; [*03A1.0020.0002] # ASTERISK
002B  ; [*0666.0020.0002] # PLUS SIGN
002C  ; [*0223.0020.0002] # COMMA
002D  ; [*020D.0020.0002] # HYPHEN-MINUS
002E  ; [*027E.0020.0002] # FULL STOP
002F  ; [*03A6.0020.0002] # SOLIDUS
0030  ; [.1F98.0020.0002] # DIGIT ZERO
0031  ; [.1F99.0020.0002] # DIGIT ONE
0032  ; [.1F9A.0020.0002] # DIGIT TWO
0033  ; [.1F9B.0020.0002] # DIGIT THREE
0034  ; [.1F9C.0020.0002] # DIGIT FOUR
0035  ; [.1F9D.0020.0002] # DIGIT FIVE
0036  ; [.1F9E.0020.0002] # DIGIT SIX
0037  ; [.1F9F.0020.0002] # DIGIT SEVEN
0038  ; [.1FA0.0020.0002] # DIGIT EIGHT
0039  ; [.1FA1.0020.0002] # DIGIT NINE
003A  ; [*0240.0020.0002] # COLON
003B  ; [*023A.0020.0002] # SEMICOLON
003C  ; [*066A.0020.0002] # LESS-THAN SIGN
003D  ; [*066B.0020.0002] # EQUALS SIGN
003E  ; [*066C.0020.0002] # GREATER-THAN SIGN
003F  ; [*026D.0020.0002] # QUESTION MARK
0040  ; [*03A0.0020.0002] # COMMERCIAL AT
0041  ; [.1FA2.0020.0008] # LATIN CAPITAL LETTER A
0042  ; [.1FBC.0020.0008] # LATIN CAPITAL LETTER B
0043  ; [.1FD6.0020.0008] # LATIN CAPITAL LETTER C
0044  ; [.1FEB.0020.0008] # LATIN CAPITAL LETTER D
0045  ; [.2007.0020.0008] # LATIN CAPITAL LETTER E
0046  ; [.2042.0020.0008] # LATIN CAPITAL LETTER F
0047  ; [.2051.0020.0008] # LATIN CAPITAL LETTER G
0048  ; [.2075.0020.0008] # LATIN CAPITAL LETTER H
0049  ; [.2090.0020.0008] # LATIN CAPITAL LETTER I
004A  ; [.20AB.0020.0008] # LATIN CAPITAL LETTER J
004B  ; [.20C4.0020.0008] # LATIN CAPITAL LETTER K
004C  ; [.20D6.0020.0008] # LATIN CAPITAL LETTER L
004D  ; [.2109.0020.0008] # LATIN CAPITAL LETTER M
004E  ; [.2118.0020.0008] # LATIN CAPITAL LETTER N
004F  ; [.213C.0020.0008] # LATIN CAPITAL LETTER O
0050  ; [.216B.0020.0008] # LATIN CAPITAL LETTER P
0051  ; [.2180.0020.0008] # LATIN CAPITAL LETTER Q
0052  ; [.2193.0020.0008] # LATIN CAPITAL LETTER R
0053  ; [.21D2.0020.0008] # LATIN CAPITAL LETTER S
0054  ; [.21F7.0020.0008] # LATIN CAPITAL LETTER T
0055  ; [.2217.0020.0008] # LATIN CAPITAL LETTER U
0056  ; [.2247.0020.0008] # LATIN CAPITAL LETTER V
0057  ; [.2259.0020.0008] # LATIN CAPITAL LETTER W
0058  ; [.2264.0020.0008] # LATIN CAPITAL LETTER X
0059  ; [.2270.0020.0008] # LATIN CAPITAL LETTER Y
005A  ; [.2286.0020.0008] # LATIN CAPITAL LETTER Z
005B  ; [*032A.0020.0002] # LEFT SQUARE BRACKET
005C  ; [*03A7.0020.0002] # REVERSE SOLIDUS
005D  ; [*032B.0020.0002] # RIGHT SQUARE BRACKET
005E  ; [*04B7.0020.0002] # CIRCUMFLEX ACCENT
005F  ; [*020B.0020.0002] # LOW LINE
0060  ; [*04B4.0020.0002] # GRAVE ACCENT
0061  ; [.1FA2.0020.0002] # LATIN SMALL LETTER A
0062  ; [.1FBC.0020.0002] # LATIN SMALL LETTER B
0063  ; [.1FD6.0020.0002] # LATIN SMALL LETTER C
0064  ; [.1FEB.0020.0002] # LATIN SMALL LETTER D
0065  ; [.2007.0020.0002] # LATIN SMALL LETTER E
0066  ; [.2042.0020.0002] # LATIN SMALL LETTER F
0067  ; [.2051.0020.0002] # LATIN SMALL LETTER G
0068  ; [.2075.0020.0002] # LATIN SMALL LETTER H
0069  ; [.2090.0020.0002] # LATIN SMALL LETTER I
006A  ; [.20AB.0020.0002] # LATIN SMALL LETTER J
006B  ; [.20C4.0020.0002] # LATIN SMALL LETTER K
006C  ; [.20D6.0020.0002] # LATIN SMALL LETTER L
006D  ; [.2109.0020.0002] # LATIN SMALL LETTER M
006E  ; [.2118.0020.0002] # LATIN SMALL LETTER N
006F  ; [.213C.0020.0002] # LATIN SMALL LETTER O
0070  ; [.216B.0020.0002] # LATIN SMALL LETTER P
0071  ; [.2180.0020.0002] # LATIN SMALL LETTER Q
0072  ; [.2193.0020.0002] # LATIN SMALL LETTER R
0073  ; [.21D2.0020.0002] # LATIN SMALL LETTER S
0074  ; [.21F7.0020.0002] # LATIN SMALL LETTER T
0075  ; [.2217.0020.0002] # LATIN SMALL LETTER U
0076  ; [.2247.0020.0002] # LATIN SMALL LETTER V
0077  ; [.2259.0020.0002] # LATIN SMALL LETTER W
0078  ; [.2264.0020.0002] # LATIN SMALL LETTER X
0079  ; [.2270.0020.0002] # LATIN SMALL LETTER Y
007A  ; [.2286.0020.0002] # LATIN SMALL LETTER Z
007B  ; [*032C.0020.0002] # LEFT CURLY BRACKET
007C  ; [*066E.0020.0002] # VERTICAL LINE
007D  ; [*032D.0020.0002] # RIGHT CURLY BRACKET
007E  ; [*0670.0020.0002] # TILDE
*/
