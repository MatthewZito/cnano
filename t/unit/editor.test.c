#include "editor.h"

#include "tests.h"

static char buf[128];

static char*
get_line (int lineno) {
  memset(buf, 0, 128);
  render_state_get_line(editor.r, lineno, buf);
  return buf;
}

static void
setup (void) {
  editor.r    = render_state_init(NULL);
  editor.curs = DEFAULT_CURSOR_STATE;
}

static void
teardown (void) {
  render_state_free(editor.r);
  editor.curs = DEFAULT_CURSOR_STATE;
}

static void
test_editor_insert_char (void) {
  render_state_insert(editor.r, 0, 0, "hello", cursor_create_copy());

  SET_CURSOR(5, 0);

  editor_insert_char('w');

  is(get_line(0), "hellow", "inserts a new char at the end");
  ok(editor.r->num_lines == 1, "still has one line");
  ok(editor.curs.x == 6, "increments the x cursor coord");
  ok(editor.curs.y == 0, "maintains the y cursor coord at zero");

  SET_CURSOR(0, 0);

  editor_insert_char('w');

  is(get_line(0), "whellow", "inserts a char at the beginning");
  ok(editor.r->num_lines == 1, "still has one line");
  ok(editor.curs.x == 1, "increments x by one");
  ok(editor.curs.y == 0, "leaves y as-is ");

  SET_CURSOR(1, 0);

  editor_insert_newline();
  editor_insert_char('w');

  is(get_line(0), "w", "keeps the first char where it was");
  is(get_line(1), "whellow", "pushes the subsequent chars to the new line and inserts the new char");
  ok(editor.r->num_lines == 2, "now has two lines");
  ok(editor.curs.x == 1, "x is incremented once");
  ok(editor.curs.y == 1, "y is incremented once");

  SET_CURSOR(3, 1);
  editor_insert_char('x');
  is(get_line(1), "whexllow", "inserts the new char in the middle of the line");
}

static void
test_editor_insert_newline (void) {
  render_state_insert(editor.r, 0, 0, "hello", cursor_create_copy());

  is(get_line(0), "hello", "sanity check");
  ok(editor.r->num_lines == 1, "sanity check");
  ok(editor.curs.x == 0, "sanity check");
  ok(editor.curs.y == 0, "sanity check");

  SET_CURSOR(5, 0);

  editor_insert_newline();

  is(get_line(0), "hello", "maintains the first line as-is");
  is(get_line(1), "", "inserts an empty line");
  ok(editor.r->num_lines == 2, "now has two lines");
  ok(editor.curs.x == 0, "x is at zero");
  ok(editor.curs.y == 1, "y incremented to new line");

  editor_insert_newline();

  is(get_line(0), "hello", "maintains the first line as-is");
  is(get_line(1), "", "maintains the second line");
  is(get_line(2), "", "adds a third line");
  ok(editor.r->num_lines == 3, "yep - has three lines");
  ok(editor.curs.x == 0, "x coord is at zero");
  ok(editor.curs.y == 2, "y coord incremented to the third line");
}

static void
test_editor_insert_newline_middle_word (void) {
  render_state_insert(editor.r, 0, 0, "hello", cursor_create_copy());

  char buf[128];

  is(get_line(0), "hello", "sanity check");
  ok(editor.r->num_lines == 1, "sanity check");
  ok(editor.curs.x == 0, "sanity check");
  ok(editor.curs.y == 0, "sanity check");

  SET_CURSOR(2, 0);

  editor_insert_newline();

  is(get_line(0), "he", "breaks the word where inserted");
  is(get_line(1), "llo", "pushes the subsequent chars to the new line");
  ok(editor.r->num_lines == 2, "has two lines now");
  ok(editor.curs.x == 0, "x is at zero");
  ok(editor.curs.y == 1, "y is incremented");

  SET_CURSOR(0, 0);

  editor_insert_newline();

  is(get_line(0), "", "inserts a new line and pushes the subsequent ones");
  is(get_line(1), "he", "first line is now second");
  is(get_line(2), "llo", "second is now third");
  ok(editor.r->num_lines == 3, "has three lines");
  ok(editor.curs.x == 0, "x is zero");
  ok(editor.curs.y == 1, "y is 1 because we inserted the new line at the first line");
}

static void
test_editor_delete_char (void) {
  render_state_insert(editor.r, 0, 0, "hello", cursor_create_copy());

  SET_CURSOR(5, 0);
  editor_delete_char();
  is(get_line(0), "hell", "deletes a char");
  ok(editor.r->num_lines == 1, "has one line");
  ok(editor.curs.x == 4, "moves the x cursor coord back");
  ok(editor.curs.y == 0, "doesn't touch the y cursor coord");

  SET_CURSOR(0, 0);
  editor_delete_char();
  is(get_line(0), "hell", "no-ops");
  ok(editor.r->num_lines == 1, "still has one line");
  ok(editor.curs.x == 0, "keeps the x cursor coord at zero");
  ok(editor.curs.y == 0, "doesn't touch the y cursor coord");

  SET_CURSOR(4, 0);
  editor_insert_newline();
  editor_insert_char('x');
  is(get_line(0), "hell", "doesn't touch the first line");
  is(get_line(1), "x", "inserts a char at the beginning of the newline");
  ok(editor.r->num_lines == 2, "has two lines now");
  ok(editor.curs.x == 1, "moves the x cursor coord to 1");
  ok(editor.curs.y == 1, "increments the y cursor coord");

  editor_delete_char();
  is(get_line(0), "hell", "doesn't touch the first line");
  is(get_line(1), "", "removes the only char on the second line");
  ok(editor.r->num_lines == 2, "still has two lines doe");
  ok(editor.curs.x == 0, "puts the x cursor coord at the beginning");
  ok(editor.curs.y == 1, "...of the second line");

  editor_delete_char();
  is(get_line(0), "hell", "deletes the new line");
  ok(editor.r->num_lines == 1, "now has a single line again");
  ok(editor.curs.x == 4, "wraps the x cursor coord back to the end of the first line");
  ok(editor.curs.y == 0, "is on the first line");

  editor_delete_char();
  editor_delete_char();
  editor_delete_char();
  editor_delete_char();
  editor_delete_char();
  editor_delete_char();
  is(get_line(0), "", "clears the line");
  ok(editor.r->num_lines == 1, "still has one line because you cannot delete the first one");
  ok(editor.curs.x == 0, "stops the cursor at zero");
  ok(editor.curs.y == 0, "stops the cursor at zero");
}

static void
test_editor_delete_line_before_x (void) {
  render_state_insert(editor.r, 0, 0, "hello goodbye world", cursor_create_copy());

  SET_CURSOR(14, 0);
  editor_delete_line_before_x();
  is(get_line(0), "world", "deletes the full line preceding the x cursor coord");
  ok(editor.curs.x == 0, "x moves to the beginning of the line");
  ok(editor.curs.y == 0, "y remains at zero");

  SET_CURSOR(0, 0);
  editor_delete_line_before_x();
  is(get_line(0), "world", "no-ops because we're at 0:0");
  ok(editor.curs.x == 0, "moves x to zero");
  ok(editor.curs.y == 0, "y remains at zero");

  SET_CURSOR(3, 0);
  editor_delete_line_before_x();
  is(get_line(0), "ld", "deletes the chars before the x cursor coord");
  ok(editor.curs.x == 0, "moves x to zero");
  ok(editor.curs.y == 0, "y remains at zero");
}

void
run_editor_tests (void) {
  void (*functions[])() = {
    test_editor_insert_char,
    test_editor_insert_newline,
    test_editor_insert_newline_middle_word,
    test_editor_delete_char,
    test_editor_delete_line_before_x,
  };

  for (unsigned int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
    setup();
    functions[i]();
    teardown();
  }
}
