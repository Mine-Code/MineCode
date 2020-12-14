#include <parserWrap.h>

#include <assert.h>

#include <parserContext.h>


namespace parserWrap{
    void program(parserCtx::parserContext& ctx){
        if(ctx.iter.peek()==L"#"){
            assert(ctx.iter.next() == L"#"   );
            assert(ctx.iter.next() == L"do"  );
            assert(ctx.iter.next() == L"once");
            // TODO: implement do once
        }
    }
    void stmt(parserCtx::parserContext&){
        
    }
}