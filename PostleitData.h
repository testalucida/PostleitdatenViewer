#ifndef POSTLEITDATA_H
#define POSTLEITDATA_H

#include <string>
#include <list>

static const int PL_RECORD_LEN = 168;
static const int SB_RECORD_LEN = 234;

struct PL {
    char PLrecord[PL_RECORD_LEN];

    union  {

        char version [9];
        char datum [8];
        char plz [5];
        char alort[8];
        char art[2];
        char stverz;
        char pfverz;
        char oname[40];
        char ozusatz[30];
        char art_ozusatz;
        char oname24[24];
        char postlag; //130
        char la_brief[8];
        char la_alort[8];
        char kgs[8];
        char ortcode[3];
        char leitcode_max[3]; //160
        char rabatt_info_schwer;
        char reserve[2];
        char fz_nr[2];
        char bz_nr[2];
        char satzende; //'$' -> wird zu 0x00
    } ;
};

struct SB {
    char SBrecord[SB_RECORD_LEN];

    union {

        char version [9];
        char datum [8];
        char alort[8];
        char schluessel[11]; //36
        char hnr_von[8];
        char hnr_bis[8];
        char status;
        char hnr_1000;
        char stverz; //55
        char name_sort[46]; //101
        char name46[46];
        char name22[22];
        char reserve; //170
        char hnr_typ;
        char plz[5];
        char code[3];
        char otl_schl[3];
        char alorg_b[8]; //190
        char kgs[8];
        char alort_neu[8];
        char schluessel_neu[11]; //217
        char hnr_von_neu[8];
        char hnr_bis_neu[8];
        char satzende; //'$' -> wird zu 0x00
    } _SB;
};

struct Strasse {
    std::string name;
    std::string hnr_von;
    std::string hnr_bis;
    int code;
    void clear() {
        name.clear();
        hnr_von.clear();
        hnr_bis.clear();
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
