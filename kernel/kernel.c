void kernel(void) {
  *(char *)0xB8000 = 'K';

  while (1) {
  }
}