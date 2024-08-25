#include "IncludeAliasFileProcessor.h"
#include "FileManager.h"
#include "StringUtils.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace PJ;
using namespace PJ_Dev;

void IncludeAliasFileProcessor::Process(FilePath filePath) {
    FileManager fm;
    auto extension = fm.FileExtension(filePath, false);
    if ("h" == extension) {
        String pathString = filePath.string();
        VectorList<std::string> pathComponents =
            ComponentsSeparatedBy(pathString, fm.PreferredSeparator());
        VectorList<String> relativeComponents;
        VectorList<String> forwardComponents;

        for (VectorList<std::string>::const_reverse_iterator i = pathComponents.rbegin();
             i != pathComponents.rend(); i++) {
            String strI = *i;
            if (ToLower(strI) == "phasejump") {
                break;
            }

            relativeComponents.insert(relativeComponents.begin(), *i);
        }

        for (int i = 0; i < pathComponents.size() - relativeComponents.size(); i++) {
            Add(forwardComponents, pathComponents[i]);
        }

        FilePath includeFilePath;
        String relativeString = "../";

        for (int i = 0; i < relativeComponents.size(); i++) {
            auto rc = relativeComponents[i];
            relativeString += rc;

            auto isFinal = i == relativeComponents.size() - 1;
            if (!isFinal) {
                relativeString += "/";
            }
        }

        for (int i = 0; i < forwardComponents.size(); i++) {
            auto fc = forwardComponents[i];
            auto forwardPath = FilePath(fc.c_str());
            includeFilePath /= forwardPath;
        }

        includeFilePath /= FilePath("Includes");
        includeFilePath /= FilePath(filePath.filename());

        cout << relativeString << "\n";

        // Create and open a text file
        ofstream file(includeFilePath.string());

        // Write to the file
        file << "#include \"" << relativeString << "\"";

        // Close the file
        file.close();
    }
}
