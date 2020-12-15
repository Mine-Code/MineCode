#include <parserWrap.h>

#include <assert.h>

#include <parserContext.h>
#include <util.h>

namespace parserWrap{
    void program(parserCtx::parserContext& ctx){
        if(ctx.iter.peek()==L"#"){
            assert(ctx.iter.next() == L"#"   );
            assert(ctx.iter.next() == L"do"  );
            assert(ctx.iter.next() == L"once");
            // TODO: implement do once
        }
        while(ctx.iter.hasData()){
            stmt(ctx);
        }
    }
    void stmt(parserCtx::parserContext& ctx){
        // stmt Switcher
        std::wstring text=ctx.iter.peek();
        if(text==L"for"){
            // TODO:call 'for'
        }else if(text==L"while"){
            // TODO:call 'while'
        }else if(text==L"if"){
            // TODO:call 'if'
        }else if(text==L"func"){
            func(ctx);
        }else if(text==L"for"){
            // TODO:call 'for'
        }else{
            //put / assign
            assert(ctx.iter.hasData());
            ctx.iter.next();
            text=ctx.iter.peek();

            if(text==L"<<"){
                // TODO:call 'put'
            }else if(
                   text==L"++"
                || text==L"--"
                || util::isAssignOp(text)
            ){
                // TODO:call 'assign'
            }
        }
    }
}