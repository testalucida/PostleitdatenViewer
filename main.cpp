#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

using namespace std;

int main() {
  Fl_Double_Window *win = new Fl_Double_Window(862, 322, "PostleitdatenViewer");

  // Show window
  win->end();

  win->show();
  return Fl::run();
}
