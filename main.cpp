#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include "PostleitdatenManager.h"
#include "PostleitData.h"

using namespace std;

string _pldatenDir = "/home/martin/Postleitdaten/B1206154.DAT";
PostleitdatenManager *_pPlManager;
PostleitDataInParm _plInParm;
PostleitDataResultRecordList _plDataResultList;

Fl_Input *_pPlz ;
Fl_Input *_pOrt ;
Fl_Input *_pStr;
Fl_Table *_pStrTbl;

void onGo( Fl_Widget * ) {
    _plInParm.plz = _pPlz->value();
    _plInParm.ort = _pOrt->value();
    _plInParm.str = _pStr->value();
    _plDataResultList.clear();
    _pPlManager->search( _plInParm, _plDataResultList );
}

void onReset( Fl_Widget * ) {
}

int main() {
    _pPlManager = new PostleitdatenManager( _pldatenDir );
    Fl_Double_Window *pWin = new Fl_Double_Window( 100, 100, 380, 300, "PostleitdatenViewer");
    _pPlz = new Fl_Input( 60, 10, 50, 22, "PLZ: " );
    _pPlz->value( "91054" );
    _pOrt = new Fl_Input( 160, 10, 200, 22, "Ort: " );
    _pOrt->value( "Buckenhof" );
    _pStr = new Fl_Input( 60, 50, 300, 22, "Straße: " );

    _pStrTbl = new Fl_Table( 60, 90, 300, 150 );
    _pStrTbl->end();

    Fl_Return_Button *pGoBtn = new Fl_Return_Button( 60, 255, 50, 22, "Go!" );
    pGoBtn->callback( onGo );

    Fl_Button *pResetBtn = new Fl_Button( 120, 255, 50, 22, "Reset" );

    // Show window
    pWin->end();

    pWin->show();
    return Fl::run();
}
