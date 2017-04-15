#ifndef POSTLEITDATENMANAGER_H
#define POSTLEITDATENMANAGER_H

#include <string>

class PostleitdatenManager
{
    public:
        PostleitdatenManager( std::string postleitdaten_filename );
        ~PostleitdatenManager();

    protected:

    private:
        std::string _plfilename;
};

#endif // POSTLEITDATENMANAGER_H
