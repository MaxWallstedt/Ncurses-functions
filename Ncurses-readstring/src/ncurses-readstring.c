/************************************************************************/
/*                    Copyright © 2013 Max Wällstedt                    */
/*                                                                      */
/* This file is part of Ncurses-readstring.                             */
/*                                                                      */
/* Ncurses-readstring is free software: you can redistribute it and/or  */
/* modify it under the terms of the GNU General Public License as       */
/* published by the Free Software Foundation, either version 3 of the   */
/* License, or (at your option) any later version.                      */
/*                                                                      */
/* Ncurses-readstring is distributed in the hope that it will be        */
/* useful, but WITHOUT ANY WARRANTY; without even the implied warranty  */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Ncurses-readstring. If not, see                           */
/* <http://www.gnu.org/licenses/>.                                      */
/*                                                                      */
/************************************************************************/

#include <curses.h>

/* Reads a string from the keyboard and echoes what has been typed
   so far on specified coordinates. */
int
mvreadstr (int y, int x, char *str, int maxn, int curss)
{
    /* "i" is used in the foor-loop that erases the input field
        efter a keypress. */
    /* "count" is the number of characters typed so far. */
    /* "curi" is where the cursor currently is located. */
    /* "curi2" will be used to temporarily store "curi" to
       preserve its value. */
    /* "key" will contain the value returned by getch() */
    /* "buf" and "swap" will be used when several characters
       will be moved in the string. */
    /* "cx" takes the value of the originally entered coordinate. */
    /* "ins" indicates if you are in insert-mode. */
    int i, count = 0, curi = 0, curi2, key, buf, swap, cx = x, ins = 0;

    curs_set (1);
    *(str) = '\0';

    do      /* Loops until an 'Enter'-key is pressed */
    {
        move (y, x);
        key = getch();

        for (i = 0; i < maxn; i++)      /* Clears the input field */
            mvaddch (y, cx + i, ' ');

        /* Left-key */
        if (key == KEY_LEFT && curi > 0)
        {
            x--;
            curi--;
        }

        /* Right-key */
        else if (key == KEY_RIGHT && *(str + curi) != '\0')
        {
            x++;
            curi++;
        }

        /* Home-key */
        else if (key == KEY_HOME)
        {
            x = cx;
            curi = 0;
        }

        /* End-key */
        else if (key == KEY_END)
        {
            curi = count;
            x = cx + curi;
        }

        /* Backspace (8 on Windows systems) */
        else if ((key == KEY_BACKSPACE || key == 8) && curi != 0)
        {
            curi2 = --curi;

            do
            {
                buf = *(str + curi + 1);
                *(str + curi) = buf;
                curi++;
            }
            while (buf != '\0');

            curi = curi2;
            x--;
            count--;
        }

        /* Delete-key */
        else if (key == KEY_DC && *(str + curi) != '\0')
        {
            curi2 = curi;

            do                          /* Moves every character right */
            {                           /* of the cursor one step to   */
                buf = *(str + curi + 1);/* the left (until '\0' is     */
                *(str + curi) = buf;    /* encountered)                */
                curi++;
            }
            while (buf != '\0');

            curi = curi2;
            count--;
        }

        /* Insert-key */
        else if (key == 331)
        {
            if (ins == 0)
            {
                ins = 1;
                curs_set (2);
            }

            else
            {
                ins = 0;
                curs_set (1);
            }
        }

        /* Escape-key */
        else if (key == 27)
        {
            *(str) = '\0';  /* Reset string */
            return 1;
        }

        /* Any other key. To restrict which characters that can
           be entered, add more conditions below. */
        else if (((key >= 32 && key <= 126)
                 || (key >= 128 && key <= 255)))
        {
            if (ins == 0 && count < maxn)   /* If not instert-mode */
            {
                swap = key;
                curi2 = curi;

            do                          /* Moves every character right */
            {                           /* of the cursor one step to   */
                buf = *(str + curi);    /* the right (until '\0') and  */
                *(str + curi) = swap;   /* puts the entered key under  */
                swap = buf;             /* the cursor                  */
                curi++;
            }
            while (buf != '\0');

                *(str + curi) = '\0';
                curi = curi2;
                x++;
                count++;
                curi++;
            }

            else if (ins == 1)      /* If insert-mode */
            {
                if (*(str + curi) == '\0' && count < maxn)
                {                           /* If current character is */
                    *(str + curi++) = key;  /* the current last one    */
                    *(str + curi) = '\0';
                    count++;
                    x++;
                }

                else if (*(str + curi) != '\0') /* If current character */
                {                               /* is not last          */
                    *(str + curi++) = key;
                    x++;
                }
            }
        }

        mvprintw (y, cx, "%s", str);    /* Prints the string (since it */
    }                                   /* is cleared above)           */
    while (key != '\n' && key != '\r' && key != 459);

    curs_set (curss);                   /* Cursor visibility */

    return 0;
}
