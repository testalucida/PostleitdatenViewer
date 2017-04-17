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
            copyRTrimmed( pOrt->name, pl.oname, sizeof( pl.oname ) );
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
            copyRTrimmed( pStr->name, sb.name46, sizeof( sb.name46 ) );
            pStr->hnr_von.append( sb.hnr_von, sizeof( sb.hnr_von ) );
            pStr->hnr_bis.append( sb.hnr_bis, sizeof( sb.hnr_bis ) );
            copyRTrimmed( pStr->name22, sb.name22, sizeof( sb.name22 ) );
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
    Kgs kgs;
    if( getKgs( plz, ort, kgs ) ) {
        //dann alle Straßen mit diesem Kreisgemeindeschlüssel finden:
        PostleitDataResultRecord *pRec = new PostleitDataResultRecord;
        pRec->ort.kgs = kgs.kgs;
        pRec->ort.alort = kgs.alort;
        pRec->ort.name = ort;
        pRec->ort.plz = plz;
    }
}

bool PostleitdatenManager::getKgs( const string& plz, const string& ort, Kgs& kgs ) const {

    auto itr = _ortList.begin();
    for( ; itr != _ortList.end(); itr++ ) {
        Ort* pOrt = *itr;
        if( pOrt->plz == plz && pOrt->name == ort ) {
            kgs.plz = pOrt->plz;
            kgs.ort = pOrt->name;
            kgs.kgs = pOrt->kgs;
            kgs.alort = pOrt->alort;
            return true;
        }
    }
    kgs.kgs = kgs.alort = "<not found>";
    return false;
}

bool PostleitdatenManager::getKgs( const string& plz, KgsList& kgsList ) const {

    auto itr = _ortList.begin();
    for( ; itr != _ortList.end(); itr++ ) {
        Ort* pOrt = *itr;
        if( pOrt->plz == plz ) {
            Kgs kgs;
            kgs.plz = pOrt->plz;
            kgs.ort = pOrt->name;
            kgs.kgs = pOrt->kgs;
            kgs.alort = pOrt->alort;
            kgsList.push_back( kgs );
        }
    }
    
    return !kgsList.empty();
}

bool PostleitdatenManager::
getStrassen( const KgsList& kgsList, const string& strFilter, PostleitDataResultRecordList& outList ) const 
{
    for( auto kitr = kgsList.begin(); kitr != kgsList.end(); kitr++ ) {
        Kgs kgs = *kitr;
        for( auto sitr = _strasseList.begin(); sitr != _strasseList.end(); sitr++ ) {
            Strasse *pStr = *sitr;
            if( kgs.kgs == pStr->kgs && matches(strFilter, pStr->name ) ) {
                PostleitDataResultRecord *pOutRec = new PostleitDataResultRecord;
                pOutRec->ort.name = kgs.ort;
                pOutRec->ort.plz = kgs.plz;
                pOutRec->ort.alort = kgs.alort;
                pOutRec->ort.kgs = kgs.kgs;
                pOutRec->str = *pStr;
                outList.push_back( pOutRec );
            }
        }
    }
}

bool PostleitdatenManager::
matches( const std::string& filter, const std::string& strasse ) const {
    if( filter.empty() || filter == "*" ) {
        return true;
    }
    
    return !strasse.compare( 0, filter.length(), filter );
}

/*
string PostleitdatenManager::rtrim( const string& str ) const {
    if( str.length() < 1 ) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr( 0, last + 1 );
}
*/

void PostleitdatenManager::copyRTrimmed( std::string& dest, const char* pSrc, int srcLen ) {
    const char *pEnd = pSrc + srcLen - 1;
    for( ; *pEnd == ' ' && pEnd >= pSrc; pEnd-- );
    for( ; pSrc <= pEnd; pSrc++ ) {
        dest.push_back( *pSrc  );
    }
}

PostleitdatenManager::~PostleitdatenManager()
{
    //dtor
}
