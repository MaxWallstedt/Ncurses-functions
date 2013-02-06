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
#include <locale.h>
#include "ncurses-readstring.h"

int
main (int argc, char *argv[])
{
    /* "i" is used in for-loops to keep track of the x coordinate */
    /* "key" is used for storing the value returned by getch()    */
    int i, key;
    char string1[20], string2[20];  /* The two string variables */

    setlocale (LC_CTYPE, "");

    initscr();                  /* Most of the below initialisers are */
    noecho();                   /* not necessary for this example.    */
    keypad (stdscr, TRUE);      /* It's just a template for a         */
    meta (stdscr, TRUE);        /* hypothetical program that might    */
    nodelay (stdscr, FALSE);    /* need them.                         */
    notimeout (stdscr, TRUE);
    raw();
    curs_set (0);

    mvaddch (5, 10, ACS_ULCORNER);  /* Starts to print the first frame */

    for (i = 0; i < 20; i++)
        mvaddch (5, 10 + i + 1, ACS_HLINE);

    mvaddch (5, 10 + i + 1, ACS_URCORNER);
    mvaddch (6, 10, ACS_VLINE);

    for (i = 0; i < 20; i++)
        mvaddch (6, 10 + i + 1, ' ');

    mvaddch (6, 10 + i + 1, ACS_VLINE);
    mvaddch (7, 10, ACS_LLCORNER);

    for (i = 0; i < 20; i++)
        mvaddch (7, 10 + i + 1, ACS_HLINE);

    mvaddch (7, 10 + i + 1, ACS_LRCORNER);
    mvreadstr (6, 12, string1, 18, 0);  /* readstring is called here. */

    mvaddch (5, 35, ACS_ULCORNER);  /* Starts to print the second frame */

    for (i = 0; i < 20; i++)
        mvaddch (5, 35 + i + 1, ACS_HLINE);

    mvaddch (5, 35 + i + 1, ACS_URCORNER);
    mvaddch (6, 35, ACS_VLINE);

    for (i = 0; i < 20; i++)
        mvaddch (6, 35 + i + 1, ' ');

    mvaddch (6, 35 + i + 1, ACS_VLINE);
    mvaddch (7, 35, ACS_LLCORNER);

    for (i = 0; i < 20; i++)
        mvaddch (7, 35 + i + 1, ACS_HLINE);

    mvaddch (7, 35 + i + 1, ACS_LRCORNER);
    mvreadstr (6, 37, string2, 18, 0);  /* readstring is called here */
                                        /* once again.               */
    mvprintw (9, 10, "You wrote:");
    mvprintw (10, 10, "\"%s\" and \"%s\"", string1, string2);
                                            /* The two entered strings */
    mvprintw (12, 10, "Press 'q' to quit"); /* are both printed.       */

    do                      /* Loops until you press 'q' */
    {
        key = getch();
    }
    while (key != 'q');

    endwin();
    return 0;
}