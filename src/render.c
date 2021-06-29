#pragma GCC dependency "buffer.h"

#include "render.h"

#include "common.h"
#include "buffer.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @brief Draws vim-like tilde-prepended rows on the screen
 *
 * @todo set to customizable with lineno
 */
void draw_rows(struct extensible_buf *e_buffer) {
  int line;

  for (line = 0; line < T.screenrows; line++) {
    int filerow = line + T.rowoff;
    // is this part of the text buffer, or a row that exists after its end?
    if (filerow >= T.numrows) {
      // no file was opened; blank editor
      if (T.numrows == 0 && line == T.screenrows / 3) {
        // write branding
        char branding[80];

        int brandingLen = snprintf(
          branding,
          sizeof(branding),
          "%s -- v%s", APPNAME, APPVERSION
        );

        // truncate?
        if (brandingLen > T.screencols) brandingLen = T.screencols;

        // padding
        int padding = (T.screencols - brandingLen) / 2;
        if (padding) {
          buf_extend(e_buffer, "~", 1);
          padding--;
        }

        while (padding--) buf_extend(e_buffer, " ", 1);
        buf_extend(e_buffer, branding, brandingLen);
      } else {
        buf_extend(e_buffer, "~", 1);
      }
    } else {
      // subtract num of chars to left of the col offset from the row len
      int len = T.row[filerow].rsize - T.coloff;

      if (len < 0) len = 0; // correct horizontal pos
      if (len > T.screencols) len = T.screencols;

      buf_extend(e_buffer, &T.row[filerow].render[T.coloff], len);
    }

    // clear line
    buf_extend(e_buffer, "\x1b[K", 3);

    // mitigate missing line prefix on last line
    buf_extend(e_buffer, "\r\n", 2);
  }
}

/**
 * @brief Render the message bar to the viewport
 *
 * Times out after user input or five seconds
 *
 * @param e_buffer
 */
void draw_msg_bar(struct extensible_buf *e_buffer) {
  // clear message bar
  buf_extend(e_buffer, "\x1b[K", 3);

  int msglen = strlen(T.statusmsg);

  // ensure it fits
  if (msglen > T.screencols) msglen = T.screencols;

  // only display message if it is less than 5s old
  if (msglen && time(NULL) - T.statusmsg_time < 5) {
    buf_extend(e_buffer, T.statusmsg, msglen);
  }
}

/**
 * @brief Render the status bar
 * @param e_buffer
 *
 * @see https://vt100.net/docs/vt100-ug/chapter3.html#SGR
 */
void draw_stats_bar(struct extensible_buf *e_buffer) {
  // switch to inverted hues
  buf_extend(e_buffer, "\x1b[7m", 4);

  char status[80], rstatus[80];

  int len = snprintf(
    status,
    sizeof(status),
    "%.20s - %d lines %s",
    T.filename ? T.filename : "[No Name]",
    T.numrows,
		T.dirty ? "(modified)" : ""
  );

  // lineno
  int rlen = snprintf(
    rstatus,
    sizeof(rstatus),
    "%d/%d",
    // current line
    T.curs_y + 1,
    T.numrows
  );

  // truncate
  if (len > T.screencols) len = T.screencols;
  buf_extend(e_buffer, status, len);

  while (len < T.screencols) {
    // print spaces until we hit the second status str
    if (T.screencols - len == rlen) {
      buf_extend(e_buffer, rstatus, rlen);
      break;
    } else {
      buf_extend(e_buffer, " ", 1);
      len++;
    }
  }

  // clear formatting
  buf_extend(e_buffer, "\x1b[m", 3);

  // print NL after first status bar
  buf_extend(e_buffer, "\r\n", 2);
}

/**
 * @brief Set the Status Message object
 *
 * @param fmt
 * @param ... variadic
 */
void set_stats_msg(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  // va_arg(&fmt);

  // this calls `va_arg` for us
  vsnprintf(
    T.statusmsg,
    sizeof(T.statusmsg),
    fmt,
    ap
  );

  va_end(ap);

  // passing `NULL` sets UNIX timestamp for now
  T.statusmsg_time = time(NULL);
}
