/************************************************************************/
/*                    Copyright © 2013 Max Wällstedt                    */
/*                                                                      */
/* This file is part of Ncurses-menu.                                   */
/*                                                                      */
/* Ncurses-menu is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Ncurses-menu is distributed in the hope that it will be useful,      */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Ncurses-menu. If not, see <http://www.gnu.org/licenses/>. */
/*                                                                      */
/************************************************************************/

#include <curses.h>

/* Calculates the length of a "string" */
int
n_chars (char *str)
{
    int i = 0;

    while (1)                       /* Loops until the ith element */
    {                               /* is a string terminator.     */
        if (*(str + i++) == '\0')   /* i increases even if the     */
            return --i;             /* element is '\0', so i is    */
    }                               /* decreased by one.           */
}

/* Copies string "src" to string "dest" */
void
str_cp (char *dest, char *src)
{
    int i = 0;

    do                                 /* Loops until the ith  */
    {                                  /* element of src is    */
        if (*(src + i) != '\0')        /* '\0'. If the ith     */
        {                              /* element of src is    */
            *(dest + i) = *(src + i);  /* not '\0', copy it to */
            i++;                       /* the ith element of   */
        }                              /* dest, then increase  */
    }                                  /* i by one.            */
    while (*(src + i) != '\0');

    *(dest + i) = '\0'; /* Terminate dest by adding '\0' to its */
                        /* last element (now dest == src).      */
    return;
}

/* Prints a menu according to your arguments/parameters,
   see the README file for more details */
int
print_menu (int sty, int x, int alts, int width,
            char title[], char entries[][100], int start)
{
    /* "i" will be used for printing out a character several times
       in a row by using for-loops. Later it will also be used
       to point to elements in "temparray" in order to assign some
       spaces after the currently selected word, so that the
       entire menu cell will be highlighted. */
    /* "j" will be used once by a for-loop when "i" is used elsewhere. */
    /* "k" is used to point to the different "entries" strings. */
    /* "blankspace1" and 2 are used for formatting the strings
       in the menu cells. */
    /* "currow" contains the currently highlighted row in the menu. */
    /* "y" will be used to move the cursor in the y-axis. */
    /* "key" will hold the keycode of the last key you pressed,
       in order to later compare it for different actions. */
    /* "temparray" will as previously stated contain the currently
       selected word in order to highlight it. */
    int i, j, k, blankspace1, blankspace2, currow = start, y = sty, key;
    char temparray[100];

    if (n_chars (title) + 2 > width)    /* "width" cannot be less than */
        width = n_chars (title) + 2;    /* the width of the strings    */
                                        /* plus some space. First      */
    for (k = 0; k < alts; k++)          /* check the title, then the   */
    {                                   /* entries.                    */
        if (n_chars (&entries[k][0]) + 2 > width)
            width = n_chars (&entries[k][0]) + 2;
    }

    k = 0;
    move (y++, x);
    addch (ACS_ULCORNER);           /* Here the program starts to     */
                                    /* print the frame of the menu.   */
    for (i = 0; i < width; i++)     /* ULCORNER is upper left corner, */
        addch (ACS_HLINE);          /* HLINE is horizontal line.      */

    addch (ACS_URCORNER);
    printw ("\n");
    move (y++, x);

    if ((width - n_chars (title)) % 2 != 0) /* If it's not possible to */
    {                                       /* perfectly center the    */
        blankspace2 = (width - n_chars (title) + 1) / 2;
        blankspace1 = blankspace2 - 1;      /* menu title, it will be  */
    }                                       /* placed slightly left.   */

    else
    {
        blankspace1 = (width - n_chars (title)) / 2;
        blankspace2 = blankspace1;
    }

    addch (ACS_VLINE);

    for (i = 0; i < blankspace1; i++)
        printw (" ");

    printw ("%s", title);

    for (i = 0; i < blankspace2; i++)
        printw (" ");

    addch (ACS_VLINE);
    printw ("\n");
    move (y++, x);
    addch (ACS_LTEE);           /* LTEE is a left tilted 'T'-like     */
                                /* shape used to connect lines to the */
    for (i = 0; i < width; i++) /* right of, above and underneath it. */
        addch (ACS_HLINE);

    addch (ACS_RTEE);
    printw ("\n");
    move (y++, x);

    for (j = 0; j < alts - 1; j++)  /* Here, the loop will print all */
    {                               /* except the last entry for the */
        addch (ACS_VLINE);          /* menu.                         */
        printw (" %s", &entries[k][0]);
        blankspace1 = width - (n_chars (&entries[k][0]) + 1);
                                    /* The blankspace is after the */
        for (i = 0; i < blankspace1; i++)
            printw(" ");            /* string, since the strings   */
                                    /* will be left fixated.       */
        k++;
        addch (ACS_VLINE);
        printw ("\n");
        move (y++, x);
        addch (ACS_LTEE);

        for (i = 0; i < width; i++)
            addch (ACS_HLINE);

        addch (ACS_RTEE);
        printw ("\n");
        move (y++, x);
    }

    addch (ACS_VLINE);
    printw (" %s", &entries[k][0]);
    blankspace1 = width - (n_chars (&entries[k][0]) + 1);

    for (i = 0; i < blankspace1; i++)
        printw(" ");

    k++;
    addch (ACS_VLINE);
    printw ("\n");
    move (y++, x);
    addch (ACS_LLCORNER);       /* The last menu entry is outside the */
                                /* loop because the lower corners are */
    for (i = 0; i < width; i++) /* different. This last entry uses    */
        addch (ACS_HLINE);      /* regular box corners to fulfill the */
                                /* menu.                              */
    addch (ACS_LRCORNER);
    printw ("\n");

    do  /* This loop is terminated when you */
    {   /* select an entry in the menu.     */
        attron (A_STANDOUT);    /* This highlights the current row. */
        blankspace1 = width - (n_chars (&entries[currow - 1][0]) + 1);
        temparray[0] = ' ';     /* Also the blankspace after the */
        str_cp (&temparray[1], &entries[currow - 1][0]);
                                /* string will be highlighted.   */
        for (i = n_chars (&entries[currow - 1][0]) + 1; i < width; i++)
        {
            temparray[i] = ' ';
        }

        temparray[i] = '\0';    /* The highlighted entry will be  */
        mvprintw ((sty + 3) + (currow - 1) * 2, x + 1, "%s", temparray);
        attroff (A_STANDOUT);   /* printed over the corresponding */
        key = getch();          /* non-highlighted entry.         */

        if (key == KEY_UP)
        {
            mvprintw ((sty + 3) + (currow - 1) * 2, /* First print the */
                      x + 1, "%s", temparray);      /* non-highlighted */
                                                    /* entry over the  */
            if (currow == 1)                        /* highlighted one */
                currow = alts;

            else                /* Change the currently selected entry */
                currow--;       /* according to the direction given by */
        }                       /* the keypress. Going up from the top */
                                /* moves you to the bottom.            */
        else if (key == KEY_DOWN)   /* Works just like KEY_UP above,   */
        {                           /* just in the opposite direction. */
            mvprintw ((sty + 3) + (currow - 1) * 2,
                      x + 1, "%s", temparray);

            if (currow == alts)
                currow = 1;

            else
                currow++;
        }
    }
    while (key != '\n' && key != '\r' && key != 459);

    return currow;  /* The return is the row-number of the selected */
}                   /* entry. Can be 1 to "alts" (not 0).           */