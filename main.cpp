#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>

using namespace std;

int main() {
  Fl_Double_Window *win = new Fl_Double_Window( 100, 100, 380, 300, "PostleitdatenViewer");
  Fl_Input *pPlz = new Fl_Input( 60, 10, 50, 22, "PLZ: " );
  Fl_Input *pOrt = new Fl_Input( 160, 10, 200, 22, "Ort: " );
  Fl_Input *pStr = new Fl_Input( 60, 50, 300, 22, "Straße: " );

  Fl_Table *pStrTbl = new Fl_Table( 60, 90, 300, 150 );
  pStrTbl->end();

  Fl_Return_Button *pOkBtn = new Fl_Return_Button( 60, 255, 50, 22, "Go!" );

  Fl_Button *pResetBtn = new Fl_Button( 120, 255, 50, 22, "Reset" );

  // Show window
  win->end();

  win->show();
  return Fl::run();
}
