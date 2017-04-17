#ifndef POSTLEITDATA_H
#define POSTLEITDATA_H

#include <string>
#include <list>

static const int RECORD_LEN = 325;

union PL {
    char PLrecord[RECORD_LEN];

    struct  {

        char version [9]; //0-8
        char datum [8];   //9-16
        char plz [5];     //17-21
        char alort[8];    //22-29
        char art[2];      //30-31
        char stverz;      //32
        char pfverz;      //33
        char oname[40];   //34-73
        char ozusatz[30]; //74-103
        char art_ozusatz; //104
        char oname24[24]; //105-128
        char postlag;     //129
        char la_brief[8]; //130-137
        char la_alort[8]; //138-145
        char kgs[8];      //146-153
        char ortcode[3];
        char leitcode_max[3]; //160
        char rabatt_info_schwer;
        char reserve[2];
        char fz_nr[2];
        char bz_nr[2];
        char satzende; //'$' 
    } ;
};

union SB {
    char SBrecord[RECORD_LEN];

    struct {

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
        char satzende; //'$' 
    } ;
};

struct Ort {
    std::string kgs;
    std::string alort;
    std::string name;
    std::string plz;
 
    void clear() {
        name.clear();
        plz.clear();
        kgs.clear();
        alort.clear();
    }
};
struct Strasse {
    std::string kgs;
    std::string alort;
    std::string name;
    std::string hnr_von;
    std::string hnr_bis;
    std::string name22;
    std::string code;
    
    void clear() {
        kgs.clear();
        alort.clear();
        name.clear();
        name22.clear();
        hnr_von.clear();
        hnr_bis.clear();
        code.clear();
    }
};

struct PostleitDataResultRecord {
    Ort ort;
    Strasse str;
    void clear() {
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
