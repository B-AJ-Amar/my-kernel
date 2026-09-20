#ifndef KERNEL_EDITOR_H
#define KERNEL_EDITOR_H

#include <fs/fs.h>
#define EDITOR_WIDTH 80
#define EDITOR_HEIGHT 25
#define EDITOR_BODY_TOP 1
#define EDITOR_BODY_HEIGHT 22
#define EDITOR_SEPARATOR_ROW 23
#define EDITOR_FOOTER_ROW 24
#define EDITOR_INITIAL_CAPACITY 1024

#define EDITOR_NORMAL_FG 15
#define EDITOR_NORMAL_BG 0
#define EDITOR_FRAME_FG 2
#define EDITOR_FRAME_BG 0

int editor_run(fs_node_t *file);

#endif
