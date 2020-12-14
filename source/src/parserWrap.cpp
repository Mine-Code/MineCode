#include <parserWrap.h>

#include <assert.h>

#include <parserContext.h>


namespace parserWrap{
    void program(parserContext& ctx){
        if(ctx.iter.peek()==L"#"){
            assert(ctx.iter.next() == L"#"   );
            assert(ctx.iter.next() == L"do"  );
            assert(ctx.iter.next() == L"once");
        }
    }
}