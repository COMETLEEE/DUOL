#include "ParserBase/DUOLParserBase.h"
#include "DUOLFBXParser/DUOLFBXParser.h"

DUOLParser::DUOLParserBase::DUOLParserBase()
{
}

DUOLParser::DUOLParserBase::~DUOLParserBase()
{
}

std::shared_ptr<DUOLParser::DUOLParserBase> DUOLParser::DUOLParserBase::Create()
{
    std::shared_ptr<DUOLParserBase> newFbxLoader;

    newFbxLoader = std::make_shared<DUOLFBXParser>();

    return newFbxLoader;
}
