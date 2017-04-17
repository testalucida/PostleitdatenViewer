#ifndef POSTLEITDATENMANAGER_H
#define POSTLEITDATENMANAGER_H

#include <string>
#include <list>
#include "PostleitData.h"


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

        bool getOrtList( const std::string& plz, const std::string& ort, OrtList& ) const;
        //bool getKgs( const std::string& plz, KgsList& kgsList ) const;
        bool getStrassen( const OrtList&, const std::string&, PostleitDataResultRecordList& ) const;
        inline bool matches( const std::string&, const std::string& ) const;
        void printPL() const;
        void print( const PostleitDataResultRecordList & ) const;
        //void print( const char* pLabel, const char* s, int n, bool newLine = false ) const;
        //std::string rtrim( const std::string& str ) const;
        inline void copyRTrimmed( std::string& dest, const char* pSrc, int srcLen );
    private:
        std::string _plfilename;
        std::string _SBfile;
        std::string _PLfile;
        std::string _version;
        std::list<Ort*> _ortList;
        std::list<Strasse*> _strasseList;
};

#endif // POSTLEITDATENMANAGER_H
