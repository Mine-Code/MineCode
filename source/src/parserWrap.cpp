#include <parserWrap.h>
#include <assert.h>
namespace parserWrap{
    void program(streamType& st,tokensType tokens){
        if(tokens.peek()==L"#"){
            assert(tokens.next() == L"#"   );
            assert(tokens.next() == L"do"  );
            assert(tokens.next() == L"once");
        }
    }
}