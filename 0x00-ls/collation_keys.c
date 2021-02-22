#include "hls.h"

/* malloc free */
#include <stdlib.h>
#include <stdbool.h>


int _strcoll(const char *s1, const char *s2)
{
	coll_key_t *ckey_s1 = NULL, *ckey_s2 = NULL,
		*temp1 = NULL, *temp2 = NULL;
	int diff;

	/* build collation keys */
	ckey_s1 = buildCollKey(s1);
	ckey_s2 = buildCollKey(s2);

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


coll_key_t *buildCollKey(const char *s)
{
	coll_elem_t *elems = NULL;
	coll_key_t *key = NULL, *temp = NULL, *trim = NULL;
	int len, level, i;
	bool variable_elem_found = false;

	/* file name can't be "" */
	if (s == NULL || !s[0])
		return (NULL);

	printf("\tbuilding collation key for :%s\n", s);
	len = _strlen(s);
        elems = malloc(sizeof(coll_elem_t) * len);
	if (!elems)
		return (NULL);

	/* populate element arrays */
	for (i = 0; i < len; i++)
		setCollElem(elems + i, s[i]);

	/* build collation keys */
	for (level = 1; level < 4; level++)
	{
		if (level == 3 && !variable_elem_found)
			break;

		if (level != 1)
		{
			temp = addCollKeyNode(&key);
			temp->n = 0;
		}

		for (i = 0; i < len; i++)
		{
			if (level == 3)
			{
				temp = addCollKeyNode(&key);
				if (elems[i].variable)
					temp->n = elems[i].weights[0];
				else
					temp->n = 0xFF;
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

	/* trim trailing 0xFF key segments */
	temp = key;
	while (temp && temp->next)
		temp = temp->next;
	while (temp && temp->n == 0xFF)
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


void setCollElem(coll_elem_t *elem, char c)
{
	if (!elem)
		return;

	switch (c)
	{
		/* 0020  ; [*0209.0020.0002] # SPACE */
	case ' ':
		elem->weights[0] = 0x2F;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0021  ; [*0267.0020.0002] # EXCLAMATION MARK */
	case '!':
		elem->weights[0] = 0x3B;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0022  ; [*031D.0020.0002] # QUOTATION MARK */
	case '"':
		elem->weights[0] = 0x43;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0023  ; [*03AC.0020.0002] # NUMBER SIGN */
	case '#':
		elem->weights[0] = 0x5B;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0024  ; [.1F64.0020.0002] # DOLLAR SIGN */
	case '$':
		elem->weights[0] = 0x53;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0025  ; [*03AD.0020.0002] # PERCENT SIGN */
	case '%':
		elem->weights[0] = 0x5D;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0026  ; [*03A9.0020.0002] # AMPERSAND */
	case '&':
		elem->weights[0] = 0x59;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0027  ; [*0316.0020.0002] # APOSTROPHE */
	case '\'':
		elem->weights[0] = 0x41;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0028  ; [*0328.0020.0002] # LEFT PARENTHESIS */
	case '(':
		elem->weights[0] = 0x45;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0029  ; [*0329.0020.0002] # RIGHT PARENTHESIS */
	case ')':
		elem->weights[0] = 0x47;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002A  ; [*03A1.0020.0002] # ASTERISK */
	case '*':
		elem->weights[0] = 0x55;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002B  ; [*0666.0020.0002] # PLUS SIGN */
	case '+':
		elem->weights[0] = 0x5F;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002C  ; [*0223.0020.0002] # COMMA */
	case ',':
		elem->weights[0] = 0x35;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002D  ; [*020D.0020.0002] # HYPHEN-MINUS */
	case '-':
		elem->weights[0] = 0x33;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002E  ; [*027E.0020.0002] # FULL STOP */
	case '.':
		elem->weights[0] = 0x3F;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 002F  ; [*03A6.0020.0002] # SOLIDUS */
	case '/':
		/* ls cannot sort by this char as it is special in shell */
		break;
		/* 0030  ; [.1F98.0020.0002] # DIGIT ZERO */
	case '0':
		elem->weights[0] = 0x70;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0031  ; [.1F99.0020.0002] # DIGIT ONE */
	case '1':
		elem->weights[0] = 0x71;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0032  ; [.1F9A.0020.0002] # DIGIT TWO */
	case '2':
		elem->weights[0] = 0x72;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0033  ; [.1F9B.0020.0002] # DIGIT THREE */
	case '3':
		elem->weights[0] = 0x73;
		elem->weights[1] = 0x02;
		break;
		/* 0034  ; [.1F9C.0020.0002] # DIGIT FOUR */
	case '4':
		elem->weights[0] = 0x74;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0035  ; [.1F9D.0020.0002] # DIGIT FIVE */
	case '5':
		elem->weights[0] = 0x75;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0036  ; [.1F9E.0020.0002] # DIGIT SIX */
	case '6':
		elem->weights[0] = 0x76;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0037  ; [.1F9F.0020.0002] # DIGIT SEVEN */
	case '7':
		elem->weights[0] = 0x77;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0038  ; [.1FA0.0020.0002] # DIGIT EIGHT */
	case '8':
		elem->weights[0] = 0x78;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0039  ; [.1FA1.0020.0002] # DIGIT NINE */
	case '9':
		elem->weights[0] = 0x79;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 003A  ; [*0240.0020.0002] # COLON */
	case ':':
		elem->weights[0] = 0x39;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 003B  ; [*023A.0020.0002] # SEMICOLON */
	case ';':
		elem->weights[0] = 0x37;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 003C  ; [*066A.0020.0002] # LESS-THAN SIGN */
	case '<':
		elem->weights[0] = 0x27;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 003D  ; [*066B.0020.0002] # EQUALS SIGN */
	case '=':
		elem->weights[0] = 0x29;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 003E  ; [*066C.0020.0002] # GREATER-THAN SIGN */
	case '>':
		elem->weights[0] = 0x2B;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 003F  ; [*026D.0020.0002] # QUESTION MARK */
	case '?':
		elem->weights[0] = 0x3D;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0040  ; [*03A0.0020.0002] # COMMERCIAL AT */
	case '@':
		elem->weights[0] = 0x51;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0041  ; [.1FA2.0020.0008] # LATIN CAPITAL LETTER A */
	case 'A':
		elem->weights[0] = 0x80;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0042  ; [.1FBC.0020.0008] # LATIN CAPITAL LETTER B */
	case 'B':
		elem->weights[0] = 0x81;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0043  ; [.1FD6.0020.0008] # LATIN CAPITAL LETTER C */
	case 'C':
		elem->weights[0] = 0x82;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0044  ; [.1FEB.0020.0008] # LATIN CAPITAL LETTER D */
	case 'D':
		elem->weights[0] = 0x83;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0045  ; [.2007.0020.0008] # LATIN CAPITAL LETTER E */
	case 'E':
		elem->weights[0] = 0x84;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0046  ; [.2042.0020.0008] # LATIN CAPITAL LETTER F */
	case 'F':
		elem->weights[0] = 0x85;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0047  ; [.2051.0020.0008] # LATIN CAPITAL LETTER G */
	case 'G':
		elem->weights[0] = 0x86;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0048  ; [.2075.0020.0008] # LATIN CAPITAL LETTER H */
	case 'H':
		elem->weights[0] = 0x87;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0049  ; [.2090.0020.0008] # LATIN CAPITAL LETTER I */
	case 'I':
		elem->weights[0] = 0x88;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004A  ; [.20AB.0020.0008] # LATIN CAPITAL LETTER J */
	case 'J':
		elem->weights[0] = 0x89;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004B  ; [.20C4.0020.0008] # LATIN CAPITAL LETTER K */
	case 'K':
		elem->weights[0] = 0x8A;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004C  ; [.20D6.0020.0008] # LATIN CAPITAL LETTER L */
	case 'L':
		elem->weights[0] = 0x8B;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004D  ; [.2109.0020.0008] # LATIN CAPITAL LETTER M */
	case 'M':
		elem->weights[0] = 0x8C;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004E  ; [.2118.0020.0008] # LATIN CAPITAL LETTER N */
	case 'N':
		elem->weights[0] = 0x8D;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 004F  ; [.213C.0020.0008] # LATIN CAPITAL LETTER O */
	case 'O':
		elem->weights[0] = 0x8E;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0050  ; [.216B.0020.0008] # LATIN CAPITAL LETTER P */
	case 'P':
		elem->weights[0] = 0x8F;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0051  ; [.2180.0020.0008] # LATIN CAPITAL LETTER Q */
	case 'Q':
		elem->weights[0] = 0x90;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0052  ; [.2193.0020.0008] # LATIN CAPITAL LETTER R */
	case 'R':
		elem->weights[0] = 0x91;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0053  ; [.21D2.0020.0008] # LATIN CAPITAL LETTER S */
	case 'S':
		elem->weights[0] = 0x92;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0054  ; [.21F7.0020.0008] # LATIN CAPITAL LETTER T */
	case 'T':
		elem->weights[0] = 0x93;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0055  ; [.2217.0020.0008] # LATIN CAPITAL LETTER U */
	case 'U':
		elem->weights[0] = 0x94;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0056  ; [.2247.0020.0008] # LATIN CAPITAL LETTER V */
	case 'V':
		elem->weights[0] = 0x95;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0057  ; [.2259.0020.0008] # LATIN CAPITAL LETTER W */
	case 'W':
		elem->weights[0] = 0x96;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0058  ; [.2264.0020.0008] # LATIN CAPITAL LETTER X */
	case 'X':
		elem->weights[0] = 0x97;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 0059  ; [.2270.0020.0008] # LATIN CAPITAL LETTER Y */
	case 'Y':
		elem->weights[0] = 0x98;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 005A  ; [.2286.0020.0008] # LATIN CAPITAL LETTER Z */
	case 'Z':
		elem->weights[0] = 0x99;
		elem->weights[1] = 0x08;
		elem->variable = false;
		break;
		/* 005B  ; [*032A.0020.0002] # LEFT SQUARE BRACKET */
	case '[':
		elem->weights[0] = 0x49;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 005C  ; [*03A7.0020.0002] # REVERSE SOLIDUS */
	case '\\':
		elem->weights[0] = 0x57;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 005D  ; [*032B.0020.0002] # RIGHT SQUARE BRACKET */
	case ']':
		elem->weights[0] = 0x4B;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 005E  ; [*04B7.0020.0002] # CIRCUMFLEX ACCENT */
	case '^':
		elem->weights[0] = 0x23;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 005F  ; [*020B.0020.0002] # LOW LINE */
	case '_':
		elem->weights[0] = 0x31;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0060  ; [*04B4.0020.0002] # GRAVE ACCENT */
	case '`':
		elem->weights[0] = 0x21;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 0061  ; [.1FA2.0020.0002] # LATIN SMALL LETTER A */
	case 'a':
		elem->weights[0] = 0x80;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0062  ; [.1FBC.0020.0002] # LATIN SMALL LETTER B */
	case 'b':
		elem->weights[0] = 0x81;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0063  ; [.1FD6.0020.0002] # LATIN SMALL LETTER C */
	case 'c':
		elem->weights[0] = 0x82;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0064  ; [.1FEB.0020.0002] # LATIN SMALL LETTER D */
	case 'd':
		elem->weights[0] = 0x83;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0065  ; [.2007.0020.0002] # LATIN SMALL LETTER E */
	case 'e':
		elem->weights[0] = 0x84;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0066  ; [.2042.0020.0002] # LATIN SMALL LETTER F */
	case 'f':
		elem->weights[0] = 0x85;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0067  ; [.2051.0020.0002] # LATIN SMALL LETTER G */
	case 'g':
		elem->weights[0] = 0x86;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0068  ; [.2075.0020.0002] # LATIN SMALL LETTER H */
	case 'h':
		elem->weights[0] = 0x87;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0069  ; [.2090.0020.0002] # LATIN SMALL LETTER I */
	case 'i':
		elem->weights[0] = 0x88;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006A  ; [.20AB.0020.0002] # LATIN SMALL LETTER J */
	case 'j':
		elem->weights[0] = 0x89;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006B  ; [.20C4.0020.0002] # LATIN SMALL LETTER K */
	case 'k':
		elem->weights[0] = 0x8A;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006C  ; [.20D6.0020.0002] # LATIN SMALL LETTER L */
	case 'l':
		elem->weights[0] = 0x8B;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006D  ; [.2109.0020.0002] # LATIN SMALL LETTER M */
	case 'm':
		elem->weights[0] = 0x8C;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006E  ; [.2118.0020.0002] # LATIN SMALL LETTER N */
	case 'n':
		elem->weights[0] = 0x8D;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 006F  ; [.213C.0020.0002] # LATIN SMALL LETTER O */
	case 'o':
		elem->weights[0] = 0x8E;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0070  ; [.216B.0020.0002] # LATIN SMALL LETTER P */
	case 'p':
		elem->weights[0] = 0x8F;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0071  ; [.2180.0020.0002] # LATIN SMALL LETTER Q */
	case 'q':
		elem->weights[0] = 0x90;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0072  ; [.2193.0020.0002] # LATIN SMALL LETTER R */
	case 'r':
		elem->weights[0] = 0x91;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0073  ; [.21D2.0020.0002] # LATIN SMALL LETTER S */
	case 's':
		elem->weights[0] = 0x92;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0074  ; [.21F7.0020.0002] # LATIN SMALL LETTER T */
	case 't':
		elem->weights[0] = 0x93;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0075  ; [.2217.0020.0002] # LATIN SMALL LETTER U */
	case 'u':
		elem->weights[0] = 0x94;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0076  ; [.2247.0020.0002] # LATIN SMALL LETTER V */
	case 'v':
		elem->weights[0] = 0x95;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0077  ; [.2259.0020.0002] # LATIN SMALL LETTER W */
	case 'w':
		elem->weights[0] = 0x96;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0078  ; [.2264.0020.0002] # LATIN SMALL LETTER X */
	case 'x':
		elem->weights[0] = 0x97;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 0079  ; [.2270.0020.0002] # LATIN SMALL LETTER Y */
	case 'y':
		elem->weights[0] = 0x98;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 007A  ; [.2286.0020.0002] # LATIN SMALL LETTER Z */
	case 'z':
		elem->weights[0] = 0x99;
		elem->weights[1] = 0x02;
		elem->variable = false;
		break;
		/* 007B  ; [*032C.0020.0002] # LEFT CURLY BRACKET */
	case '{':
		elem->weights[0] = 0x4D;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 007C  ; [*066E.0020.0002] # VERTICAL LINE */
	case '|':
		elem->weights[0] = 0x2D;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 007D  ; [*032D.0020.0002] # RIGHT CURLY BRACKET */
	case '}':
		elem->weights[0] = 0x4F;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
		/* 007E  ; [*0670.0020.0002] # TILDE */
	case '~':
		elem->weights[0] = 0x25;
		elem->weights[1] = 0x02;
		elem->variable = true;
		break;
	default:
		elem->weights[0] = elem->weights[1] = 0x00;
		elem->variable = false;
		break;
	};
}
