#include "commandlineparser.h"

CommandLineParser::CommandLineParser(QStringList args)
{
    this->m_isValidParameter = false;
    this->m_filePath = "";
    this->m_optionName = "";

    if ( args.count() > 1 ) {
        QString option = args.at(1);

        if ( args.count() == 2 && option == SCAN_PARAM ) {
           this->m_isValidParameter = true;
           this->m_optionName = option;
        }
        else if ( args.count() == 3) {

            // check the name of parameters
            if ( option == SHRED_QUICK_PARAM ||
                     option == SHRED_SAFE_PARAM ||
                     option == SHRED_THROUGH_PARAM ) {
                QString path = args.at(2);

                this->m_optionName = option;
                this->m_filePath = path;
                this->m_isValidParameter = true;
            }
        }
    }
    else {
        this->m_isValidParameter = true;
    }
}

bool CommandLineParser::isValidParameter() const {
    return m_isValidParameter;
}

bool CommandLineParser::isScanParameter() const {
    return this->m_optionName == SCAN_PARAM;
}

bool CommandLineParser::isShredQuickParameter() const {
    return this->m_optionName == SHRED_QUICK_PARAM;
}

bool CommandLineParser::isShredSafeParameter() const{
    return this->m_optionName == SHRED_SAFE_PARAM;
}

bool CommandLineParser::isShredThroughParameter() const {
    return this->m_optionName == SHRED_THROUGH_PARAM;
}

QString CommandLineParser::getPath() const{
    return this->m_filePath;
}
