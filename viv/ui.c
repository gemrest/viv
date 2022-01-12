/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

/* NOTE: This source file contains *lots* of filler and testing code! */

#include "viv/ui.h"

#include <menu.h>
#include <stdlib.h>
#include <string.h>

void UI_initialise(char *content[], int content_length) {
  WINDOW *window;
  ITEM **window_items;
  MENU *menu;
  int character;
  int max_y;

  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);

  window_items = (ITEM **)calloc(content_length, sizeof(ITEM *));
  max_y = getmaxy(stdscr);

  for (int i = 0; i < content_length; ++i) {
    window_items[i] = new_item(content[i], content[i]);
  }

  menu = new_menu((ITEM **)window_items);

  window = newwin(max_y, 60, 0, 0);
  keypad(window, TRUE);

  set_menu_win(menu, window);
  set_menu_sub(menu, derwin(window, max_y - 2, 60, 0, 0));
  set_menu_format(menu, max_y - 2, 1);
  set_menu_mark(menu, "");

  /* box(window, 0, 0);
  UI_print_in_middle(window, 1, 0, 40, "viv", COLOR_PAIR(1));
  mvwaddch(window, 2, 0, ACS_LTEE);
  mvwhline(window, 2, 1, ACS_HLINE, 38);
  mvwaddch(window, 2, 39, ACS_RTEE); */

  post_menu(menu);
  wrefresh(window);

  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "exit: q");
  mvprintw(LINES - 1, 0, "movement: hjkl");
  attroff(COLOR_PAIR(2));
  refresh();

  while ((character = wgetch(window)) != 'q') {
    switch (character) {
      case 'k': {
        menu_driver(menu, REQ_UP_ITEM);
      } break;
      case 'j': {
        menu_driver(menu, REQ_DOWN_ITEM);
      } break;
      case 'l': {
        menu_driver(menu, REQ_SCR_DPAGE);
      } break;
      case 'h': {
        menu_driver(menu, REQ_SCR_UPAGE);
      } break;
      /* case KEY_RESIZE | 'r': {

      } break; */
      default: {
        /* ignore */
      } break;
    }
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < content_length; ++i) {
    free_item(window_items[i]);
  }
  delwin(window);
  clear();
  refresh();
}

/* void UI_print_in_middle(
  WINDOW *window,
  int start_y,
  int start_x,
  int width,
  char *string,
  chtype colour
) {
  int length, x, y;
  float temporary;

  if (window == NULL) { window = stdscr; }

  getyx(window, y, x);

  */ /* if (start_x != 0) { x = start_x; } */ /*
  if (start_y != 0) { y = start_y; }
  if (width != 0) { width = 80; }

  length = (int)strlen(string);
  temporary = (float)(width - length) / 2;
  x = start_x + (int)temporary;
  wattron(window, colour);
  mvwprintw(window, y, x, "%s", string);
  wattroff(window, colour);
  refresh();
} */
