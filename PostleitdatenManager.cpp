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
    if( _ortList.size() < 1 ) {
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

void PostleitdatenManager::loadPartFiles() {
    ifstream plfile( _PLfile );
    string line;
    char delim = 13;

    if( plfile.good() )  {
        PL pl;
        while( plfile.getline( pl.PLrecord, sizeof(pl.PLrecord), delim ) ) {
            Ort *pOrt = new Ort;
            pOrt->kgs.append( pl.kgs, sizeof( pl.kgs ) );
            pOrt->alort.append( pl.alort, sizeof( pl.alort ) );
            pOrt->name.append( pl.oname, sizeof( pl.oname ) );
            pOrt->plz.append( pl.plz, sizeof( pl.plz ) );
            _ortList.push_back( pOrt );
        }

        plfile.close();
    }
        
    ifstream sbfile( _SBfile );
    if( sbfile.good() ) {
        SB sb;
        while( sbfile.getline( sb.SBrecord, sizeof(sb.SBrecord), delim ) ) {
            Strasse *pStr = new Strasse;
            pStr->kgs.append( sb.kgs, sizeof( sb.kgs ) );
            pStr->alort.append( sb.alort, sizeof( sb.alort ) );
            pStr->name.append( sb.name46, sizeof( sb.name46 ) );
            pStr->hnr_von.append( sb.hnr_von, sizeof( sb.hnr_von ) );
            pStr->hnr_bis.append( sb.hnr_bis, sizeof( sb.hnr_bis ) );
            pStr->name22.append( sb.name22, sizeof( sb.name22 ) );
            pStr->code.append( sb.code, sizeof( sb.code ) );
            
            _strasseList.push_back( pStr );
        }
        
        sbfile.close();
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

void PostleitdatenManager::getKgs( const string& plz, const string& ort, string& kgs ) const {

    auto itr = _ortList.begin();
    for( ; itr != _ortList.end(); itr++ ) {
        Ort* pOrt = *itr;
        if( pOrt->plz == plz && pOrt->name == ort ) {
            kgs = pOrt->kgs;
            return;
        }
    }
    kgs = "<not found>";
    return;
}

void PostleitdatenManager::getKgs( const std::string& plz, KgsList& kgsList ) const {

    auto itr = _ortList.begin();
    for( ; itr != _ortList.end(); itr++ ) {
        Ort* pOrt = *itr;
        if( pOrt->plz == plz ) {
            Kgs kgs;
            kgs.plz = pOrt->plz;
            kgs.ort = pOrt->name;
            kgs.kgs = pOrt->kgs;
            kgsList.push_back( kgs );
        }
    }
}


PostleitdatenManager::~PostleitdatenManager()
{
    //dtor
}
