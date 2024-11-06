#include "const.h"
#include "keypress.h"
#include "tests.h"
#include "window.h"

#define RESET_BUFFERS()   \
  memset(slice, 0, 1024); \
  buffer_free(buf);       \
  buf = buffer_init(NULL)

static char  buf[128];
static char *file_buffer;

static char *
get_line (int lineno) {
  memset(buf, 0, 128);
  render_state_get_line(editor.r, lineno, buf);
  return buf;
}

static void
setup (void) {
  editor.r              = render_state_init(NULL);
  editor.curs           = DEFAULT_CURSOR_STATE;
  editor.win.cols       = 0;
  editor.win.rows       = 0;

  editor.conf.ln_prefix = DEFAULT_LINE_PREFIX;

  line_pad              = DEFAULT_LNPAD;

  editor.win.rows       = 40;
  editor.win.cols       = 50;
  FILE *fd              = fopen("./t/fixtures/file.txt", "r");
  if (!fd) {
    perror("fopen");
    exit(1);
  }

  file_buffer         = malloc(68300);
  size_t          sz  = sizeof(file_buffer);
  read_all_result ret = read_all(fd, &file_buffer, &sz);
  render_state_insert(editor.r, editor.curs.x, editor.curs.y, file_buffer, NULL);
}

static void
teardown (void) {
  free(editor.r);
  free(file_buffer);
}

/* clang-format off */
static void
test_basic_draw (void) {
  ok(editor.r->num_lines == 38, "sanity check");

  buffer_t *buf = buffer_init(NULL);
  window_draw_rows(buf);

  ok(editor.curs.x == 0, "cursor at cell zero");
  ok(editor.curs.y == 0, "cursor at cell zero");
  is(
    buffer_state(buf),
    ESC_SEQ_COLOR(3) "  1 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) "amateuros                                     "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  2 bash" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  3 bolt" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  4 ccan" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  5 CINEWORLD-NextJS" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  6 clib" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  7 coreutils" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  8 coroutine" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  9 dagger" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 10 dcron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 11 endlessh" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 12 entr" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 13 Fiwix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 14 grub2" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 15 knex" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 16 k-os" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 17 libuv" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 18 linux" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 19 minix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 20 Mirai-Source-Code" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 21 neovim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 22 nodemon" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 23 node-v0.x-archive" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 SQLite-2.5.0-for-code-reading" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 v8" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 29 vim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 30 vite" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 31 vitest" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 32 vixiecron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 33 vscode-textbuffer" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 34 xi-editor" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 35 xstate" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 36 xv6-annotated" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 37 xv6-public" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 38 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF DEFAULT_LINE_PREFIX
    ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "each line is rendered correctly"
  );

  buffer_free(buf);
}

static void
test_basic_insert (void) {
  buffer_t *buf = buffer_init(NULL);

  CALL_N_TIMES(3, cursor_move_right());
  editor_insert_char('x');
  CALL_N_TIMES(13, cursor_move_right());
  editor_insert_char('x');
  CALL_N_TIMES(4, cursor_move_right_word());
  CALL_N_TIMES(2, cursor_move_left());
  editor_delete_char();
  editor_insert_newline();
  CALL_N_TIMES(3, editor_insert_char('x'));
  CALL_N_TIMES(5, cursor_move_left());
  editor_insert_newline();
  CALL_N_TIMES(22, cursor_move_down());
  CALL_N_TIMES(3, editor_insert_char('y'));
  CALL_N_TIMES(3, cursor_move_down());
  cursor_move_end();
  editor_insert_char('_');
  CALL_N_TIMES(3, editor_insert_char('z'));
  cursor_move_begin();
  CALL_N_TIMES(3, editor_insert_char('z'));
  editor_insert_char('_');

  window_draw_rows(buf);

  ok(editor.curs.x == 4, "cursor at cell zero");
  ok(editor.curs.y == 29, "cursor at cell zero");
  is(
    buffer_state(buf),
    "  1 amaxteuros" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  2 bash" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  3 bxolt" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  4 c" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  5 c" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  6 xxxn" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  7 CINEWORLD-NextJS" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  8 clib" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  9 coreutils" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 10 coroutine" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 11 dagger" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 12 dcron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 13 endlessh" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 14 entr" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 15 Fiwix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 16 grub2" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 17 knex" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 18 k-os" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 19 libuv" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 20 linux" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 21 minix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 22 Mirai-Source-Code" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 23 neovim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 nodemon" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 node-v0.x-archive" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 yyyredis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 SQLite-2.5.0-for-code-reading" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 29 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 30 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) "zzz_v8_zzz                                    "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 31 vim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 32 vite" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 33 vitest" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 34 vixiecron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 35 vscode-textbuffer" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 36 xi-editor" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 37 xstate" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 38 xv6-annotated" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 39 xv6-public" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 40 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after various edits and cursor changes"
  );

  buffer_free(buf);
}

static void
test_basic_horizontal_viewport_shift (void) {
  buffer_t *buf = buffer_init(NULL);

  SET_CURSOR(4, 27);
  CALL_N_TIMES(2, cursor_move_up());
  cursor_move_end();
  CALL_N_TIMES(20, editor_insert_char('x'));
  window_scroll();
  CALL_N_TIMES(33, editor_insert_char('y'));
  window_scroll();
  CALL_N_TIMES(3, editor_insert_char('y'));
  window_scroll();

  window_draw_rows(buf);

  ok(editor.curs.x == 85, "cursor at cell zero");
  ok(editor.curs.y == 25, "cursor at cell zero");
  is(
    buffer_state(buf),
    "  1 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  2 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  3 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  4 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  5 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  6 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  7 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  8 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  9 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 10 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 11 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 12 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 13 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 14 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 15 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 16 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 17 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 18 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 19 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 20 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 21 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 22 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 23 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) "xxxxxxxxxyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 29 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 30 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 31 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 32 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 33 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 34 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 35 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 36 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 37 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 38 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after moving off the main window view"
  );

  buffer_free(buf);
}

static void
test_basic_shift_back_viewport (void) {
  buffer_t *buf = buffer_init(NULL);

  SET_CURSOR(4, 27);
  CALL_N_TIMES(2, cursor_move_up());
  cursor_move_end();
  CALL_N_TIMES(20, editor_insert_char('x'));
  window_scroll();
  CALL_N_TIMES(33, editor_insert_char('y'));
  window_scroll();
  CALL_N_TIMES(3, editor_insert_char('y'));
  window_scroll();
  cursor_move_begin();
  window_scroll();
  editor_insert_char('p');

  window_draw_rows(buf);

  ok(editor.curs.x == 1, "cursor at cell zero");
  ok(editor.curs.y == 25, "cursor at cell zero");

  is(
    buffer_state(buf),
    "  1 amateuros" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  2 bash" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  3 bolt" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  4 ccan" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  5 CINEWORLD-NextJS" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  6 clib" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  7 coreutils" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  8 coroutine" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  9 dagger" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 10 dcron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 11 endlessh" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 12 entr" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 13 Fiwix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 14 grub2" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 15 knex" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 16 k-os" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 17 libuv" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 18 linux" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 19 minix" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 20 Mirai-Source-Code" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 21 neovim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 22 nodemon" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 23 node-v0.x-archive" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) "pSQLite-2.5.0-for-code-readingxxxxxxxxxxxxxxxx"
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 v8" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 29 vim" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 30 vite" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 31 vitest" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 32 vixiecron" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 33 vscode-textbuffer" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 34 xi-editor" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 35 xstate" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 36 xv6-annotated" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 37 xv6-public" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 38 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after jumping back to the main viewport and inserting a char"
  );

  buffer_free(buf);
}

static void
test_basic_select (void) {
  buffer_t *buf = buffer_init(NULL);

  SET_CURSOR(0, 27);
  CALL_N_TIMES(2, cursor_move_up());
  cursor_move_end();
  CALL_N_TIMES(20, editor_insert_char('x'));
  CALL_N_TIMES(33, editor_insert_char('y'));
  window_scroll();
  CALL_N_TIMES(3, editor_insert_char('y'));
  cursor_move_begin();
  window_scroll();
  editor_insert_char('p');
  CALL_N_TIMES(3, cursor_select_right());

  window_draw_rows(buf);

  char slice[1024];
  buffer_slice(buf, 393, 535, slice);

  ok(editor.curs.x == 4, "cursor at cell zero");
  ok(editor.curs.y == 25, "cursor at cell zero");
  is(
    slice,
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "p" ESC_SEQ_BG_COLOR(218) "SQL" ESC_SEQ_BG_COLOR(238) "ite-2.5.0-for-code-readingxxxxxxxxxxxxxxxx"
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 v8" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after highlighting some text"
  );

  RESET_BUFFERS();

  cursor_select_down();

  window_draw_rows(buf);
  buffer_slice(buf, 393, 578, slice);

  ok(editor.curs.x == 4, "cursor at cell zero");
  ok(editor.curs.y == 26, "cursor at cell zero");
  is(
    slice,
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 p" ESC_SEQ_BG_COLOR(218) "SQLite-2.5.0-for-code-readingxxxxxxxxxxxxxxxx"
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF ESC_SEQ_COLOR(3)
    " 27 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) ESC_SEQ_BG_COLOR(218) "trax" ESC_SEQ_BG_COLOR(238)
    "xx                                        " ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after extending the highlight across multiple lines"
  );

  RESET_BUFFERS();

  CALL_N_TIMES(5, cursor_select_left());
  window_scroll();

  window_draw_rows(buf);

  ok(editor.curs.x == 86, "cursor at cell zero");
  ok(editor.curs.y == 25, "cursor at cell zero");
  is(
    buffer_state(buf),
    "  1 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  2 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  3 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  4 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  5 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  6 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  7 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  8 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "  9 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 10 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 11 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 12 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 13 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 14 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 15 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 16 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 17 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 18 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 19 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 20 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 21 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 22 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 23 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) ESC_SEQ_BG_COLOR(218)
    "xxxxxxxxxyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" ESC_SEQ_BG_COLOR(238) " "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 29 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 30 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 31 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 32 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 33 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 34 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 35 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 36 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 37 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 38 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    DEFAULT_LINE_PREFIX ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after moving the highlight selection outside the viewport"
  );

  RESET_BUFFERS();

  CALL_N_TIMES(86, cursor_select_left());
  // Simulate refresh rate - normally we hit the scroll fn up every movement
  window_scroll();
  CALL_N_TIMES(2, cursor_select_left());
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 393, 564, slice);

  ok(editor.curs.x == 4, "cursor at cell zero");
  ok(editor.curs.y == 24, "cursor at cell zero");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 25 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) "redi" ESC_SEQ_BG_COLOR(218) "s"
    ESC_SEQ_BG_COLOR(238) "                                         "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 " ESC_SEQ_BG_COLOR(218) "p" ESC_SEQ_NORM_COLOR "SQLite-2.5.0-for-code-readingxxxxxxxxxxxxxxxx"
    ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "expected state after extending back into the viewport and across multiple "
    "lines"
  );

  buffer_free(buf);
}

// Edge case/bugfix: if the anchor line selection starts AFTER the subsequent
// line length, then we select down, we erroneously selected the entire next
// line (beyond the line length).
static void
test_edge_case_1_select (void) {
  buffer_t *buf = buffer_init(NULL);

  cursor_select_clear();

  SET_CURSOR(0, 27);
  CALL_N_TIMES(2, cursor_move_up());
  cursor_move_end();
  CALL_N_TIMES(20, editor_insert_char('x'));
  CALL_N_TIMES(33, editor_insert_char('y'));
  window_scroll();
  CALL_N_TIMES(3, editor_insert_char('y'));
  cursor_move_begin();
  window_scroll();
  SET_CURSOR(6, 25);
  cursor_select_right();
  window_scroll();
  cursor_select_down();
  window_scroll();

  window_draw_rows(buf);

  char slice[1024];
  buffer_slice(buf, 393, 578, slice);

  ok(editor.curs.x == 7, "cursor at cell zero");
  ok(editor.curs.y == 26, "cursor at cell zero");
  is(
    slice,
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 SQLite" ESC_SEQ_BG_COLOR(218) "-2.5.0-for-code-readingxxxxxxxxxxxxxxxxx"
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 27 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238) ESC_SEQ_BG_COLOR(218) "traxxx"
    ESC_SEQ_BG_COLOR(238) "                                        "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "selects across lines properly despite varying line lengths"
  );

  buffer_free(buf);
}

static void
test_basic_undo (void) {
  buffer_t *buf = buffer_init(NULL);

  cursor_select_clear();
  SET_CURSOR(0, 26);
  cursor_move_begin();
  CALL_N_TIMES(3, editor_insert_char('x'));
  editor_undo();
  CALL_N_TIMES(3, editor_insert_char('z'));
  cursor_move_up();
  CALL_N_TIMES(3, cursor_move_right());
  CALL_N_TIMES(3, editor_delete_char());
  cursor_move_up();
  CALL_N_TIMES(3, cursor_move_left());
  cursor_move_up();
  CALL_N_TIMES(3, editor_insert_char('z'));

  window_draw_rows(buf);

  char slice[1024];
  buffer_slice(buf, 369, 516, slice);

  ok(editor.curs.x == 3, "cursor at cell zero");
  ok(editor.curs.y == 23, "cursor at cell zero");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 24 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "zzzputlockertv.one                            "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 SQL-2.5.0-for-code-reading" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 zzztraxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "baseline for undo tests"
  );

  RESET_BUFFERS();

  editor_undo();

  window_draw_rows(buf);
  buffer_slice(buf, 369, 516, slice);

  ok(editor.curs.x == 0, "cursor at cell zero");
  ok(editor.curs.y == 23, "cursor at cell zero");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 24 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "putlockertv.one                               "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 SQL-2.5.0-for-code-reading" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 zzztraxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "undo works"
  );

  RESET_BUFFERS();

  editor_undo();

  window_draw_rows(buf);
  buffer_slice(buf, 369, 505, slice);

  ok(editor.curs.x == 6, "cursor at cell zero");
  ok(editor.curs.y == 25, "cursor at cell zero");
  is(
    slice,
    " 24 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 redis" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "SQLite-2.5.0-for-code-reading                 "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 zzztraxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "undo works"
  );

  buffer_free(buf);
}

void
test_complex_undo (void) {
  buffer_t *buf = buffer_init(NULL);

  SET_CURSOR(0, 25);

  CALL_N_TIMES(3, editor_insert_char('x'));
  cursor_move_end();
  CALL_N_TIMES(3, editor_insert_char('x'));
  CALL_N_TIMES(10, cursor_move_left());
  editor_delete_char();
  CALL_N_TIMES(4, cursor_move_left());
  editor_delete_char();
  CALL_N_TIMES(3, cursor_move_left());
  editor_delete_char();
  CALL_N_TIMES(5, cursor_move_left());
  editor_delete_char();
  cursor_move_up();
  cursor_move_end();
  CALL_N_TIMES(50, editor_insert_char('z'));
  cursor_move_begin();
  cursor_move_down();
  editor_delete_char();
  editor_delete_char();
  cursor_move_end();
  window_scroll();

  window_draw_rows(buf);

  char slice[1024];
  buffer_slice(buf, 198, 305, slice);

  ok(editor.curs.x == 85, "correct cursor pos");
  ok(editor.curs.y == 24, "correct cursor pos");
  is(
    slice,
    " 23 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 24 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 25 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "zzzzzzzzzzzzzzxxxSQLite2.5.0forcodereadingxxx "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct baseline"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 369, 543, slice);

  ok(editor.curs.x == 0, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    " 24 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 25 rediszzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite2.5.0forcodereadingxxx               "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 369, 539, slice);

  ok(editor.curs.x == 5, "correct cursor pos");
  ok(editor.curs.y == 24, "correct cursor pos");
  is(
    slice,
    " 24 putlockertv.one" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    ESC_SEQ_COLOR(3) " 25 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "redis                                         "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 26 xxxSQLite2.5.0forcodereadingxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 28 v8" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 10, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite-2.5.0forcodereadingxxx              "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 16, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite-2.5.0-forcodereadingxxx             "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 20, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite-2.5.0-for-codereadingxxx            "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 25, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite-2.5.0-for-code-readingxxx           "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 32, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "xxxSQLite-2.5.0-for-code-reading              "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 407, 502, slice);

  ok(editor.curs.x == 0, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    ESC_SEQ_COLOR(3) " 26 " ESC_SEQ_NORM_COLOR ESC_SEQ_BG_COLOR(238)
    "SQLite-2.5.0-for-code-reading                 "
    ESC_SEQ_NORM_COLOR ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    " 27 traxxx" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "correct undo"
  );

  RESET_BUFFERS();

  editor_undo();
  window_scroll();

  window_draw_rows(buf);
  buffer_slice(buf, 0, 20, slice);

  ok(editor.curs.x == 0, "correct cursor pos");
  ok(editor.curs.y == 25, "correct cursor pos");
  is(
    slice,
    "  1 " ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "~" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF
    "~" ESC_SEQ_ERASE_LN_RIGHT_OF_CURSOR CRLF,
    "empty now because this is the final undo"
  );
}

/* clang-format on */

void
run_regression_tests (void) {
  void (*functions[])() = {
    test_basic_draw,
    test_basic_insert,
    test_basic_horizontal_viewport_shift,
    test_basic_shift_back_viewport,
    test_basic_select,
    test_edge_case_1_select,
    test_basic_undo,
    test_complex_undo,
    // TODO: move word tests
  };

  for (unsigned int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
    setup();
    functions[i]();
    teardown();
  }
}
