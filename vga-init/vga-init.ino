#include <VGAX.h>

VGAX vga;

void setup() {
  vga.begin();
  vga.clear(11);
  for (int y=0; y!=VGAX_HEIGHT; y++) {
    for (int x=0; x!=VGAX_BWIDTH; x++) {
      vga.putpixel4(x, y, y%2 ? 27 : 228);
    }
  }
}
void loop() {
}