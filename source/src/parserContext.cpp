#include <parserContext.h>

parserCtx::parserContext::parserContext()
{
    stream=std::wstringstream();
    iter=iterator<std::wstring>(std::vector<std::wstring>());
    variables=std::unordered_map<std::string,varType>();
}