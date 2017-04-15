#ifndef POSTLEITDATA_H
#define POSTLEITDATA_H

#include <string>
#include <list>

struct Strasse {
    std::string name;
    int hnr_von;
    int hnr_bis;
    int code;
    void clear() {
        name.clear();
        hnr_von = 0;
        hnr_bis = 0;
        code = 0;
    }
};

struct PostleitDataResultRecord {
    std::string plz;
    std::string ort;
    Strasse str;
    void clear() {
        plz.clear();
        ort.clear();
        str.clear();
    }
};

struct PostleitDataInParm {
    std::string plz;
    std::string ort;
    std::string str;

};

typedef std::list<PostleitDataResultRecord> PostleitDataResultRecordList;

#endif // POSTLEITDATA_H
