#include "PostleitdatenManager.h"
#include "PostleitData.h"
#include <fstream>
#include <string.h>

using namespace std;

PostleitdatenManager::PostleitdatenManager( string postleitdaten_filename )
: _plfilename( postleitdaten_filename )
{
    createPartFilenames();
}

void PostleitdatenManager::
    search( const PostleitDataInParm& inParm, PostleitDataResultRecordList& outList )
{
    if( !existPartFiles() ) {
        writePartFiles();
    }
    if( _plList.size() < 1 ) {
        loadPartFiles();
    }

    PostleitDataResultRecordList resultList;
    getPostleitDataResultRecords( inParm.plz, inParm.ort, resultList );

}

void PostleitdatenManager::createPartFilenames() {
    size_t len = _plfilename.length();
    size_t start = len - 12 + 1; //Dateinamenlänge = 8, .DAT = 4
    _version = _plfilename.substr( start, 7 );
    string path = _plfilename.substr( 0, len - 12 );

    _SBfile = path + "SB" + _version + ".DAT";
    _PLfile = path + "PL" + _version + ".DAT";
}

bool PostleitdatenManager::existPartFiles() const {

    ifstream f( _SBfile );
    if( !f.good() ) {
        return false;
    }

    ifstream f2( _PLfile );
    if( !f2.good() ) {
        return false;
    }

    return true;
}

void PostleitdatenManager::writePartFiles() {
    string line;
    ifstream data( _plfilename );
    ofstream pl( _PLfile );
    ofstream sb( _SBfile );

    string plkennung = "PL" + _version;
    string sbkennung = "SB" + _version;
    while( getline( data, line ) ) {
        if( line.find( plkennung ) != string::npos ) {
            pl << line;
        } else if( line.find( sbkennung ) != string::npos ) {
            sb << line;
        }
    }

    data.close();
    pl.close();
    sb.close();

}

//void PostleitdatenManager::loadPartFiles() {
//    ifstream plfile( _PLfile );
//    string line;
//
//    PL *pPL = new PL;
//    while( plfile.get( pPL->PLrecord, PL_RECORD_LEN ) ) {
//        pPL->satzende = 0x00;
//        fprintf( stderr, "%s\n", pPL->PLrecord );
//        _plList.push_back( pPL );
//        PL &pl = *pPL;
//        pPL = new PL;
//    }
//
//    plfile.close();
//
//    printf( "Anzahl PL-Sätze: %d\n", _plList.size() );
//    printPL();
//
//    ifstream sbfile( _SBfile );
//    SB *pSB = new SB;
//    while( sbfile.get( pSB->SBrecord, SB_RECORD_LEN )) {
//        pSB->_SB.satzende = 0x00;
//        _sbList.push_back( pSB );
//        pSB = new SB;
//    }
//
//    sbfile.close();
//
//    printf( "Anzahl SB-Sätze: %d\n", _sbList.size() );
//}

void PostleitdatenManager::loadPartFiles() {
    ifstream plfile( _PLfile );
    string line;

    if( plfile.good() ) {

        while( getline( plfile, line ) ) {
            fprintf( stderr, "Satzlaenge: %d\n%s\n", line.length(), line.c_str() );
        }

        plfile.close();

        printf( "Anzahl PL-Sätze: %d\n", _plList.size() );
        printPL();

    }
}


//void PostleitdatenManager::printPL() const {
//    for( auto itr = _plList.begin(); itr != _plList.end(); itr++ ) {
//        PL *pl = *itr;
//
//        print( "Version: ", pl->_PL.version, 9 );
//        print( "\tDatum: ", pl->_PL.datum, 8, true );
//        print( "PLZ: ", pl->_PL.plz, 5 );
//        print( "\tOrt: ", pl->_PL.oname, 40, true );
//        print( "KGS: ", pl->_PL.kgs, 8 );
//    }
//}

void PostleitdatenManager::printPL() const {
    for( auto itr = _plList.begin(); itr != _plList.end(); itr++ ) {
        PL *pl = *itr;

        print( "Version: ", pl->version, 9 );
        print( "\tDatum: ", pl->datum, 8, true );
        print( "PLZ: ", pl->plz, 5 );
        print( "\tOrt: ", pl->oname, 40, true );
        print( "KGS: ", pl->kgs, 8 );
    }
}

void PostleitdatenManager::print( const char* pLabel, const char* s, int n, bool newLine ) const {
    if( pLabel ) {
        fprintf( stderr, "%s", pLabel );
    }
    for( int i = 0; i < n; i++ ) {
        fprintf( stderr, "%c", *s++ );
    }
    if( newLine ) {
        fprintf( stderr, "\n" );
    }
}

void PostleitdatenManager::
    getPostleitDataResultRecords( const std::string& plz,
                                  const std::string& ort,
                                  PostleitDataResultRecordList& outList ) const
{
    //erst mit plz und ort die Kreisgemeindeschlüssel finden:
    string kgs;
    getKgs( plz, ort, kgs );
}

void PostleitdatenManager::getKgs( const std::string& plz, const std::string& ort, string& kgs ) const {

    auto itr = _plList.begin();
    for( ; itr != _plList.end(); itr++ ) {
        PL* pPL = *itr;
        string p, o;
        p.append( pPL->plz, 5 );
        o.append( pPL->oname, 40 );
        fprintf( stderr, "PLZ: %s, Ort: %s\n", p.c_str(), o.c_str() );
        if( !strncmp( pPL->plz, plz.c_str(), 5 ) &&
            !strncmp( pPL->oname, ort.c_str(), 40 ) )
        {
            kgs.append( pPL->kgs, 8 );
            break;
        }
    }
    return;
}

void PostleitdatenManager::getKgs( const std::string& plz, KgsList& kgsList ) const {

    auto itr = _plList.begin();
    for( ; itr != _plList.end(); itr++ ) {
        PL* pPL = *itr;
        if( !strcmp( pPL->plz, plz.c_str() ) )
        {
            Kgs kgs;
            kgs.plz.append( pPL->plz, 5 );
            kgs.ort.append( pPL->oname, 40 );
            kgs.kgs.append( pPL->kgs, 8 );
            kgsList.push_back( kgs );
        }
    }
}


PostleitdatenManager::~PostleitdatenManager()
{
    //dtor
}
