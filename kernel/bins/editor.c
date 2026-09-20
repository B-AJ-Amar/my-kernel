#include <asm.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/bins/editor.h>
#include <kernel/console.h>
#include <mm/heap/heap.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static size_t editor_length(const char *text) {
  size_t length = 0;

  while (text[length] != '\0')
    length++;
  return length;
}

static size_t editor_line_start(const char *text, size_t cursor) {
  while (cursor > 0 && text[cursor - 1] != '\n')
    cursor--;
  return cursor;
}

static size_t editor_line_column(const char *text, size_t cursor) {
  return cursor - editor_line_start(text, cursor);
}

static size_t editor_line_number(const char *text, size_t cursor) {
  size_t line = 0;

  for (size_t i = 0; i < cursor; i++) {
    if (text[i] == '\n')
      line++;
  }
  return line;
}

static size_t editor_line_offset(const char *text, size_t line) {
  size_t current_line = 0;
  size_t offset = 0;

  while (text[offset] != '\0' && current_line < line) {
    if (text[offset] == '\n')
      current_line++;
    offset++;
  }
  return offset;
}

static size_t editor_line_end(const char *text, size_t start) {
  while (text[start] != '\0' && text[start] != '\n')
    start++;
  return start;
}

static bool editor_grow(char **text, size_t *capacity, size_t required) {
  size_t new_capacity = *capacity;
  char *new_text;

  while (new_capacity < required)
    new_capacity *= 2;

  new_text = krealloc(*text, new_capacity);
  if (new_text == NULL)
    return false;

  *text = new_text;
  *capacity = new_capacity;
  return true;
}

static void editor_put_char(size_t row, size_t column, char value, int fg,
                            int bg) {
  console_set_cursor(row, column);
  console_set_color(fg, bg);
  console_putchar(value);
}

static void editor_fill_row(size_t row, int fg, int bg, char value) {
  console_set_color(fg, bg);
  console_set_cursor(row, 0);
  for (size_t column = 0; column < EDITOR_WIDTH; column++)
    console_putchar(value);
}

static void editor_write_at(size_t row, size_t column, const char *text, int fg,
                            int bg) {
  console_set_cursor(row, column);
  console_set_color(fg, bg);
  while (*text != '\0' && column < EDITOR_WIDTH) {
    console_putchar(*text++);
    column++;
  }
}

static void editor_render(const char *name, const char *text, size_t top_line,
                          size_t left_column) {
  size_t name_length = strlen((char *)name);
  size_t title_width = name_length + 2;
  size_t title_column =
      title_width < EDITOR_WIDTH ? (EDITOR_WIDTH - title_width) / 2 : 0;

  console_clear();

  editor_fill_row(0, EDITOR_FRAME_FG, EDITOR_FRAME_BG, '-');
  editor_put_char(0, title_column, '|', EDITOR_FRAME_FG, EDITOR_FRAME_BG);
  editor_write_at(0, title_column + 1, name, EDITOR_FRAME_FG, EDITOR_FRAME_BG);
  if (title_column + name_length + 1 < EDITOR_WIDTH)
    editor_put_char(0, title_column + name_length + 1, '|', EDITOR_FRAME_FG,
                    EDITOR_FRAME_BG);

  for (size_t screen_line = 0; screen_line < EDITOR_BODY_HEIGHT;
       screen_line++) {
    size_t line = top_line + screen_line;
    size_t start = editor_line_offset(text, line);
    size_t end = editor_line_end(text, start);

    editor_fill_row(EDITOR_BODY_TOP + screen_line, EDITOR_NORMAL_FG,
                    EDITOR_NORMAL_BG, ' ');
    for (size_t offset = left_column;
         start + offset < end && offset < left_column + EDITOR_WIDTH;
         offset++) {
      editor_put_char(EDITOR_BODY_TOP + screen_line, offset - left_column,
                      text[start + offset], EDITOR_NORMAL_FG, EDITOR_NORMAL_BG);
    }
  }

  editor_fill_row(EDITOR_SEPARATOR_ROW, EDITOR_FRAME_FG, EDITOR_FRAME_BG, '-');
  editor_write_at(EDITOR_FOOTER_ROW, 1, "^S Save   ^Q Quit   ^X Save & Exit",
                  EDITOR_FRAME_FG, EDITOR_FRAME_BG);

  console_set_color(EDITOR_NORMAL_FG, EDITOR_NORMAL_BG);
}

static bool editor_save(fs_node_t *file, const char *text, size_t length) {
  return fs_write(file, (const uint8_t *)text, length);
}

int editor_run(fs_node_t *file) {
  char *text;
  char *loaded;
  size_t length;
  size_t capacity = EDITOR_INITIAL_CAPACITY;
  size_t cursor = 0;
  size_t top_line = 0;
  size_t left_column = 0;
  bool running = true;
  bool save_failed = false;
  bool needs_render = true;

  if (file == NULL || file->type != FS_FILE)
    return -1;

  loaded = fs_read(file, file->size);
  if (loaded == NULL)
    return -1;

  length = editor_length(loaded);
  while (capacity <= length)
    capacity *= 2;
  text = kmalloc(capacity);
  if (text == NULL) {
    kfree(loaded);
    return -1;
  }
  memcpy(text, loaded, length + 1);
  kfree(loaded);

  console_set_cursor_visible(false);
  while (running) {
    keyboard_event_t *event;
    size_t cursor_line;
    size_t cursor_column;
    size_t previous_top_line = top_line;
    size_t previous_left_column = left_column;

    cursor_line = editor_line_number(text, cursor);
    cursor_column = editor_line_column(text, cursor);
    if (cursor_line < top_line)
      top_line = cursor_line;
    if (cursor_line >= top_line + EDITOR_BODY_HEIGHT)
      top_line = cursor_line - EDITOR_BODY_HEIGHT + 1;
    if (cursor_column < left_column)
      left_column = cursor_column;
    if (cursor_column >= left_column + EDITOR_WIDTH)
      left_column = cursor_column - EDITOR_WIDTH + 1;

    if (top_line != previous_top_line || left_column != previous_left_column)
      needs_render = true;

    if (needs_render) {
      editor_render(file->name, text, top_line, left_column);
      needs_render = false;
    }

    console_set_cursor_visible(true);
    console_set_cursor(EDITOR_BODY_TOP + cursor_line - top_line,
                       cursor_column - left_column);

    event = keyboard_pop_event();
    if (event == NULL) {
      hlt();
      continue;
    }
    if (event->type != KEY_PRESS)
      continue;

    if (keyboard_ctrl()) {
      if (event->key == S_KEY) {
        save_failed = !editor_save(file, text, length);
      } else if (event->key == X_KEY) {
        save_failed = !editor_save(file, text, length);
        running = false;
      } else if (event->key == Q_KEY) {
        running = false;
      }
      continue;
    }

    switch (event->key) {
    case LEFT_ARROW_KEY:
      if (cursor > 0)
        cursor--;
      break;
    case RIGHT_ARROW_KEY:
      if (cursor < length)
        cursor++;
      break;
    case UP_ARROW_KEY: {
      size_t column = editor_line_column(text, cursor);
      size_t line = editor_line_number(text, cursor);
      if (line > 0) {
        size_t start = editor_line_offset(text, line - 1);
        size_t end = editor_line_end(text, start);
        cursor = start + (column < end - start ? column : end - start);
      }
      break;
    }
    case DOWN_ARROW_KEY: {
      size_t column = editor_line_column(text, cursor);
      size_t line = editor_line_number(text, cursor);
      size_t start = editor_line_offset(text, line);
      size_t end = editor_line_end(text, start);
      if (text[end] == '\n') {
        start = end + 1;
        end = editor_line_end(text, start);
        cursor = start + (column < end - start ? column : end - start);
      }
      break;
    }
    case HOME_KEY:
      cursor = editor_line_start(text, cursor);
      break;
    case END_KEY:
      cursor = editor_line_end(text, editor_line_start(text, cursor));
      break;
    case PAGE_UP_KEY:
      top_line =
          top_line > EDITOR_BODY_HEIGHT ? top_line - EDITOR_BODY_HEIGHT : 0;
      cursor = editor_line_offset(text, top_line);
      needs_render = true;
      break;
    case PAGE_DOWN_KEY:
      top_line += EDITOR_BODY_HEIGHT;
      cursor = editor_line_offset(text, top_line);
      needs_render = true;
      break;
    case BACKSPACE_KEY:
      if (cursor > 0) {
        memmove(&text[cursor - 1], &text[cursor], length - cursor + 1);
        cursor--;
        length--;
        needs_render = true;
      }
      break;
    case ENTER_KEY:
      if (!editor_grow(&text, &capacity, length + 2))
        break;
      memmove(&text[cursor + 1], &text[cursor], length - cursor + 1);
      text[cursor] = '\n';
      cursor++;
      length++;
      needs_render = true;
      break;
    default:
      if (is_printable_key(event->key) && event->character != 0) {
        if (!editor_grow(&text, &capacity, length + 2))
          break;
        memmove(&text[cursor + 1], &text[cursor], length - cursor + 1);
        text[cursor] = (char)event->character;
        cursor++;
        length++;
        needs_render = true;
      }
      break;
    }
  }

  console_set_cursor_visible(true);
  console_clear();
  kfree(text);
  return save_failed ? -1 : 0;
}
