#include "ncurses_display.h"
#include "system.h"

int main() {
  int test = 3;
  System system;
  NCursesDisplay::Display(system);
}