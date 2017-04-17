#ifndef POSTLEITDATENMANAGER_H
#define POSTLEITDATENMANAGER_H

#include <string>
#include <list>
#include "PostleitData.h"

struct Kgs {
    std::string plz;
    std::string ort;
    std::string kgs;
};

typedef std::list<Kgs> KgsList;

class PostleitdatenManager
{
    public:
        PostleitdatenManager( std::string postleitdaten_filename );
        ~PostleitdatenManager();

        void search( const PostleitDataInParm& inParm,
                     PostleitDataResultRecordList& outList );

    protected:

    private:
        void createPartFilenames();
        bool existPartFiles() const;
        void writePartFiles();
        void loadPartFiles();
        void getPostleitDataResultRecords( const std::string& plz,
                                          const std::string& ort,
                                          PostleitDataResultRecordList& outList ) const;

        void getKgs( const std::string& plz, const std::string& ort, std::string& kgs ) const;
        void getKgs( const std::string& plz, KgsList& kgsList ) const;
        void printPL() const;
        void print( const char* pLabel, const char* s, int n, bool newLine = false ) const;
    private:
        std::string _plfilename;
        std::string _SBfile;
        std::string _PLfile;
        std::string _version;
        std::list<Ort*> _ortList;
        std::list<Strasse*> _strasseList;
};

#endif // POSTLEITDATENMANAGER_H
