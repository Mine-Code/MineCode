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
            For(ctx);
        }else if(text==L"while"){
            // TODO:call 'while'
            std::wcerr<<"Error: not implemented while"<<std::endl;
            std::terminate();
        }else if(text==L"if"){
            // TODO:call 'if'
            std::wcerr<<"Error: not implemented if"<<std::endl;
            std::terminate();
        }else if(text==L"func"){
            func(ctx);
        }else if(text==L"for"){
            // TODO:call 'for'
            std::wcerr<<"Error: not implemented for"<<std::endl;
            std::terminate();
        }else{
            //put / assign
            assert(ctx.iter.hasData());
            ctx.iter.next();
            text=ctx.iter.peek();

            if(text==L"<<"){
                // TODO:call 'put'
                std::wcerr<<"Error: not implemented put"<<std::endl;
                std::terminate();
            }else if(
                   text==L"++"
                || text==L"--"
                || util::isAssignOp(text)
            ){
                // TODO:call 'assign'
                std::wcerr<<"Error: not implemented assign"<<std::endl;
                std::terminate();
            }
        }
    }
    void func(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"func");
        std::wstring functionName=ctx.iter.next();
        assert(ctx.iter.next()==L"(");
        std::wcout<<arg(ctx);
        while(ctx.iter.peek() != L")"){
            assert(ctx.iter.next()==L",");
            std::wcout<<arg(ctx);
        }
        assert(ctx.iter.next()==L")");
        assert(ctx.iter.next()==L"{");
        while(ctx.iter.hasData()){
            if(ctx.iter.peek()==L"}")break;
            stmt(ctx);
        }
        assert(ctx.iter.next()==L"}");
    }
    void For(parserCtx::parserContext& ctx){
        //"for"  , ident, "in", value|range, "{", {stmt}, "}"
        assert(ctx.iter.next() == L"for");
        std::wstring varname=ctx.iter.next();
        assert(ctx.iter.next() == L"in");
        std::wstring target=ctx.iter.next();
        assert(ctx.iter.next() == L"{");
        while(ctx.iter.hasData()){
            if(ctx.iter.peek()==L"}")break;
            stmt(ctx);
        }
        assert(ctx.iter.next() == L"}");

    }
    std::wstring arg(parserCtx::parserContext& ctx){
        std::wstring type=ctx.iter.next();
        std::wstring name=ctx.iter.next();
        return name+L":"+type;
    }
}