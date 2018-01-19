#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <list>
#include "RenameSeriesFile.hpp"

using namespace std;

int                     main(int argc, char **argv) {
    DIR                 *dirFd;
    struct dirent       *stream;
    RenameSeriesFile    manager;
    std::string         oldFile;
    std::string         newFile;

    if (argc != 3) {
        cout << "./renameSeries path_to_files new_file_format" << endl;
        return 1;
    }
    dirFd = opendir(argv[1]);
    if (dirFd == NULL) {
        perror("renameSeries error: ");
        return 1;
    }
    while ((stream = readdir(dirFd))) {
        if (stream->d_name[0] == '.') {
            continue;
        }
        manager.setNewFile(stream->d_name, argv[2]);
        manager.runFileModifications();
        newFile = argv[1];
        newFile += "/" + manager.getRenamedFile();
        oldFile = argv[1];
        oldFile += "/";
        oldFile += stream->d_name;
        rename(oldFile.c_str(), newFile.c_str());
    }
    return 0;
}