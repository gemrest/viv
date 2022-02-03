/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

/* NOTE: This source file contains *lots* of filler and testing code! */

#include "viv/ui.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#include <menu.h>
#include <stdlib.h>
#include <string.h>

char *choices[] = {
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  "mix",
  "pop",
  (char *)NULL
};

void UI_initialise(void) {
  int height, width, i;
  WINDOW *window;
  ITEM **window_items;
  MENU *menu;
  int character;
  int choices_size;

  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);

  choices_size = ARRAY_SIZE(choices);
  window_items = (ITEM **)calloc(choices_size, sizeof(ITEM *));

  for (int i = 0; i < choices_size; ++i) {
    window_items[i] = new_item(choices[i], choices[i]);
  }

  menu = new_menu((ITEM **)window_items);

  window = newwin(10, 40, 4, 4);
  keypad(window, TRUE);

  set_menu_win(menu, window);
  set_menu_sub(menu, derwin(window, 6, 38, 3, 1));
  set_menu_format(menu, 5, 1);

  set_menu_mark(menu, " * ");

  box(window, 0, 0);
  UI_print_in_middle(window, 1, 0, 40, "viv", COLOR_PAIR(1));
  mvwaddch(window, 2, 0, ACS_LTEE);
  mvwhline(window, 2, 1, ACS_HLINE, 38);
  mvwaddch(window, 2, 39, ACS_RTEE);

  post_menu(menu);
  wrefresh(window);

  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scroll down or up a page of items");
  mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
  attroff(COLOR_PAIR(2));
  refresh();

  while ((character = wgetch(window)) != KEY_F(1)) {
    switch (character) {
      case KEY_UP: {
        menu_driver(menu, REQ_UP_ITEM);
      } break;
      case KEY_DOWN: {
        menu_driver(menu, REQ_DOWN_ITEM);
      } break;
      case KEY_RIGHT: {
        menu_driver(menu, REQ_SCR_DPAGE);
      } break;
      case KEY_LEFT: {
        menu_driver(menu, REQ_SCR_UPAGE);
      } break;
    }
  }

  unpost_menu(menu);
  free_menu(menu);
  for (int i = 0; i < choices_size; ++i) {
    free_item(window_items[i]);
  }
  delwin(window);
  clear();
  refresh();
}

void UI_print_in_middle(
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

  /* if (start_x != 0) { x = start_x; } */
  if (start_y != 0) { y = start_y; }
  if (width != 0) { width = 80; }

  length = (int)strlen(string);
  temporary = (float)(width - length) / 2;
  x = start_x + (int)temporary;
  wattron(window, colour);
  mvwprintw(window, y, x, "%s", string);
  wattroff(window, colour);
  refresh();
}
