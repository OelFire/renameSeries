//
// Created by Vincent PICOT on 18/01/2018.
//

#ifndef RENAMESERIES_RENAMESERIESFILE_HPP
#define RENAMESERIES_RENAMESERIESFILE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class RenameSeriesFile {
public:
    RenameSeriesFile();
    RenameSeriesFile(const std::string &fileName, const std::string &replaceFormat);

private:
    std::string     _fileNameOrig;
    std::string     _fileName;
    std::string     _finalFileName;
    std::string     _extensionName;
    std::string     _replaceFormat;

    std::vector<std::string>    _infos;
    std::vector<unsigned int>   _episodeNumbers;
    std::vector<unsigned int>   _episodeQuality;
    unsigned int                _validEpisodeNumber;
    unsigned int                _validQuality;
    unsigned int                _defaultQuality;

public:
    bool                runFileModifications();
    void                setNewFile(const std::string &fileName, const std::string &replaceFormat);
    const std::string   &getRenamedFile() { return _finalFileName; }

private:
    void    fetchExtensionName();
    void    fetchInfos();
    bool    fetchEpisodeNumber();
    bool    fetchQualityNumber();
    void    validateEpisodeNumber();
    void    validateQuality();
    void    modifyFile();
};


#endif //RENAMESERIES_RENAMESERIESFILE_HPP
