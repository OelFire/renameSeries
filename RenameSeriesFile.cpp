//
// Created by Vincent PICOT on 18/01/2018.
//

#include "RenameSeriesFile.hpp"

RenameSeriesFile::RenameSeriesFile(const std::string &fileName, const std::string &replaceFormat) :
    _fileNameOrig(fileName),
    _fileName(fileName),
    _replaceFormat(replaceFormat),
    _defaultQuality(0)
{
    char answer;
    std::cout << "Voulez vous mettre une qualité par défaut si celle-ci n'est pas trouvé ? (Y/N): ";
    std::cin >> answer;
    if (answer == 'Y' || answer == 'y') {
        std::cout << "Entrez la qualité par défaut: ";
        std::cin >> _defaultQuality;
        if (_defaultQuality != 720 && _defaultQuality != 1080 && _defaultQuality != 480) {
            do {
                std::cout << "Veuillez entrer une qualité valide (480/720/1080): ";
                std::cin >> _defaultQuality;
            } while (_defaultQuality != 720 && _defaultQuality != 1080 && _defaultQuality != 480);
        }
    }
}

RenameSeriesFile::RenameSeriesFile() :
    _fileNameOrig(""),
    _fileName(""),
    _replaceFormat(""),
    _defaultQuality(0)
{
    char answer;
    std::cout << "Voulez vous mettre une qualité par défaut si celle-ci n'est pas trouvé ? (Y/N): ";
    std::cin >> answer;
    if (answer == 'Y' || answer == 'y') {
        std::cout << "Entrez la qualité par défaut: ";
        std::cin >> _defaultQuality;
        if (_defaultQuality != 720 && _defaultQuality != 1080 && _defaultQuality != 480) {
            do {
                std::cout << "Veuillez entrer une qualité valide (480/720/1080): ";
                std::cin >> _defaultQuality;
            } while (_defaultQuality != 720 && _defaultQuality != 1080 && _defaultQuality != 480);
        }
    }
}

bool RenameSeriesFile::runFileModifications() {
    if (_fileName == "" || _fileNameOrig == "") {
        return false;
    }
    fetchExtensionName();
    fetchInfos();
    if (!fetchEpisodeNumber()) {
        return false;
    }
    validateEpisodeNumber();
    fetchQualityNumber();
    validateQuality();
    modifyFile();
    _finalFileName = _replaceFormat;
    return true;
}

void RenameSeriesFile::fetchInfos() {
    size_t              pos = 0;
    size_t              pos2 = 0;
    std::string         firstToken;

    while ((pos = _fileName.find_first_of("([{", pos)) != std::string::npos) {
        pos2 = pos;
        firstToken = _fileName.substr(pos, 1);
        if (firstToken == "(") {
            firstToken = ")";
        }
        else if (firstToken == "[") {
            firstToken = "]";
        }
        else if (firstToken == "{") {
            firstToken = "}";
        }
        pos2 = _fileName.find_first_of(firstToken, pos2);
        _infos.push_back(_fileName.substr(pos, pos2 - pos + 1));
        _fileName.erase(pos, pos2 - pos + 1);
        pos = 0;
    }
}

bool RenameSeriesFile::fetchEpisodeNumber() {
    size_t pos = 0;
    size_t pos2 = 0;
    size_t res;

    while ((pos = _fileName.find_first_of("0123456789", pos)) != std::string::npos) {
        std::stringstream ss;
        pos2 = pos;
        pos2 = _fileName.find_first_not_of("0123456789", pos2);
        ss.str(_fileName.substr(pos, pos2 - pos));
        ss >> res;
        _episodeNumbers.push_back(res);
        pos = pos2;
    }
    if (_episodeNumbers.size() == 0) {
        return false;
    }
    return true;
}

void RenameSeriesFile::setNewFile(const std::string &fileName, const std::string &replaceFormat) {
    _fileName = fileName;
    _fileNameOrig = fileName;
    _replaceFormat = replaceFormat;
    _finalFileName.clear();
    _extensionName.clear();
    _infos.clear();
    _episodeQuality.clear();
    _episodeNumbers.clear();
    _validQuality = 0;
    _validEpisodeNumber = 0;
}

void RenameSeriesFile::fetchExtensionName() {
    size_t pos = 0;
    size_t pos2 = 0;
    size_t len = _fileName.size();

    while ((pos = _fileName.find_first_of(".", pos)) != std::string::npos) {
        pos2 = pos;
        pos++;
    }
    if (pos2 == 0) {
        return;
    }
    _extensionName = _fileName.substr(pos2, len - pos2 + 1);
    _fileName.erase(pos2, len - pos2 + 1);
}

void RenameSeriesFile::modifyFile() {
    size_t      pos = 0;
    std::string flag;
    std::string tmpValue;
    std::string tmp;

    while ((pos = _replaceFormat.find_first_of("%", pos)) != std::string::npos) {
        if (pos == std::string::npos) {
            return;
        }
        flag = _replaceFormat.substr(pos, 2);
        if (flag == "%e") {
            tmpValue = std::to_string(_validEpisodeNumber);
        }
        else if (flag == "%q") {
            if (_validQuality == 0 && _defaultQuality == 0) {
                std::cout << "Nous n'avons pas trouver de qualité dans le nom original '" << _fileNameOrig << "'" << std::endl << "Veuillez entrer une qualité: ";
                std::cin >> _validQuality;
            }
            else if (_validQuality == 0 && _defaultQuality > 0) {
                _validQuality = _defaultQuality;
            }
            tmpValue = "[";
            tmpValue += std::to_string(_validQuality);
            tmpValue += "p]";
        }
        tmp = _replaceFormat.substr(0, pos) + tmpValue + _replaceFormat.substr(pos + 2, _replaceFormat.size() - pos + 1);
        _replaceFormat = tmp;
        pos = 0;
    }
    _replaceFormat += _extensionName;
}

void RenameSeriesFile::validateEpisodeNumber() {
    size_t  i;
    int     choice;

    if (_episodeNumbers.size() == 1) {
        _validEpisodeNumber = _episodeNumbers.front();
    }
    else if (_episodeNumbers.size() == 2) {
        unsigned int first = _episodeNumbers.at(0);
        unsigned int second = _episodeNumbers.at(1);
        if ((second == 720 || second == 1080 || second == 480) && first > 10) {
            _validEpisodeNumber = first;
            return;
        }
        if ((first == 720 || first == 1080 || first == 480) && second > 10) {
            _validEpisodeNumber = second;
            return;
        }
    }
    if (_episodeNumbers.size() > 1) {
        std::cout << "Veuillez taper le chiffre correspondant au bon numéro d'épisode, le nom original est: '" << _fileNameOrig << "'." << std::endl;
        for (i = 0; i < _episodeNumbers.size(); i++) {
            std::cout << i << ".\t" << _episodeNumbers.at(i) << std::endl;
        }
        std::cout << "Votre choix: ";
        std::cin >> choice;
        if (choice >= i || choice < 0) {
            do {
                std::cout << "Mettez un chiffre valide [0 à " << i - 1 << "]: ";
                std::cin >> choice;
            } while (choice >= i || choice < 0);
        }
        _validEpisodeNumber = _episodeNumbers.at(choice);
    }
}

void RenameSeriesFile::validateQuality() {
    size_t i;
    int    choice = 0;

    if (_episodeQuality.size() == 1) {
        _validQuality = _episodeQuality.front();
    }
    else if (_episodeQuality.size() > 1) {
        std::cout << "Veuillez taper le chiffre correspondant à la bonne qualité, le nom original est: '" << _fileNameOrig << "'." << std::endl;
        std::cout << "-1:\tAucune correspondance" << std::endl;
        for (i = 0; i < _episodeQuality.size(); i++) {
            std::cout << i << ".\t" << _episodeQuality.at(i) << std::endl;
        }
        std::cout << "Votre choix: ";
        std::cin >> choice;
        if (choice >= i || choice < -1) {
            do {
                std::cout << "Mettez un chiffre valide: ";
                std::cin >> choice;
            } while (choice > i || choice < -1);
        }
        if (choice >= 0) {
            _validQuality = _episodeQuality.at(choice);
        }
    }
    if (_episodeQuality.size() == 0 || choice == -1) {
        _validQuality = 0;
    }
}

bool RenameSeriesFile::fetchQualityNumber() {
    for (size_t i = 0; i < _infos.size(); i++) {
        std::string str(_infos.at(i));
        size_t pos = 0;
        size_t pos2 = 0;
        size_t res;

        while ((pos = str.find_first_of("0123456789", pos)) != std::string::npos) {
            std::stringstream ss;
            pos2 = pos;
            pos2 = str.find_first_not_of("0123456789", pos2);
            if (pos2 - pos < 3) {
                pos = pos2;
                continue;
            }
            ss.str(str.substr(pos, pos2 - pos));
            ss >> res;
            if (res != 720 && res != 1080 && res != 480) {
                pos = pos2;
                continue;
            }
            _episodeQuality.push_back(res);
            pos = pos2;
        }
    }
    if (_episodeQuality.size() == 0) {
        return false;
    }
    return true;
}