#include <parserCore.h>

#include <assert.h>

#include <parserContext.h>
#include <util.h>

namespace parserCore{
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
            text=ctx.iter.peek(1);
            if(text==L"<<"){
                // TODO:call 'put'
                std::wcerr<<"Error: not implemented put"<<std::endl;
                std::terminate();
            }else if(
                   text==L"++"
                || text==L"--"
                || util::isAssignOp(text)
            ){
                assign(ctx);
            }
        }
    }
    void func(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"func");
        std::wstring functionName=ctx.iter.next();
        std::wcout<<"funcName:"<<functionName<<std::endl;
        assert(ctx.iter.next()==L"(");
        if(ctx.iter.peek() != L")"){
            std::wcout<<arg(ctx)<<std::endl;
        }
        while(ctx.iter.peek() != L")"){
            assert(ctx.iter.next()==L",");
            std::wcout<<arg(ctx)<<std::endl;
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
        std::wstring target=value(ctx);
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
    std::wstring value(parserCtx::parserContext& ctx){
        std::wstring string;
        wchar_t ch=ctx.iter.peek(1)[0];
        if(ctx.iter.peek()==L"["){
            return ptr(ctx);
        }else if(ctx.iter.peek(1)==L"."){
            return attribute(ctx);
        }else if(ch==L'0'){
            return ctx.iter.next();
        }else if(ch==L'"'){
            return ctx.iter.next();
        }else{
            return ident(ctx);
        }
        return string;
    }
    std::wstring ptr(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"[");
        std::wstring base=value(ctx);
        std::wstring offs;
        if(ctx.iter.peek()==L"+"){
            ctx.iter.next();
            offs=ctx.iter.next();
        }
        assert(ctx.iter.next()==L"]");
        return L"ptr"+base+L" "+offs;
    }
    std::wstring attribute(parserCtx::parserContext& ctx){
        std::wstring string;
        string+=ctx.iter.next();
        while (true)
        {
            if(ctx.iter.peek()!=L"."){
                break;
            }
            ctx.iter.next();
            string+=L"."+ctx.iter.next();
        }
        return string;
    }
    std::wstring editable(parserCtx::parserContext& ctx){
        std::wstring string;
        if(ctx.iter.peek()==L"["){
            return ptr(ctx);
        }else if(ctx.iter.peek(1)==L"."){
            return attribute(ctx);
        }else{
            return ident(ctx);
        }
        return string;
    }
    std::wstring ident(parserCtx::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        // check word?
        if(!isalpha(text[0])){
            std::wcout<<"isn't ident; "<<text<<std::endl;
            throw "isn't ident";
        }
        return text;
    }
    std::wstring constant(parserCtx::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        // check integer?
        if(!isdigit(text[0]))throw "isn't integer (const)";
        // check str?
        if(text[0]!=L'"')throw "isn't integer (const)";
        return text;
    }
    void assign(parserCtx::parserContext& ctx){
        std::wstring target=editable(ctx);
        std::wstring op=ctx.iter.next();
        if(op==L"++"){
            // TODO inc
        }else if(op==L"--"){
            // TODO dec
        }else{
            std::wstring value=expr(ctx);
            std::wcout<<target<<op<<value<<std::endl;
        }
    }
    std::wstring power (parserCtx::parserContext& ctx){
        if(ctx.iter.peek()==L"("){
            // inner type
            ctx.iter.next();
            std::wstring text=expr(ctx);
            assert(ctx.iter.next()==L")");
            return text;
        }
        return value(ctx);
        
    }
    std::wstring expo  (parserCtx::parserContext& ctx){
        std::wstring text;
        text+=power(ctx);
        while(ctx.iter.hasData() && ctx.iter.peek() == L"**"){
            assert(ctx.iter.peek() == L"**");
            text+=L" pow "+power(ctx);
        }
        return text;
    }
    std::wstring term  (parserCtx::parserContext& ctx){
        std::wstring tmp;
        tmp+=expo(ctx);
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"*" ||
                ctx.iter.peek() == L"/" ||
                ctx.iter.peek() == L"%"
            )
        ){
            auto text=ctx.iter.peek();
            assert(
                text == L"*" ||
                text == L"/" ||
                text == L"%"
            );
            tmp+=text+expo(ctx);
        }
        return tmp;
    }
    std::wstring expr  (parserCtx::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        if(text==L"+" || text==L"-"){
            // TODO: process
        }
        term(ctx);
        while(
            ctx.iter.hasData() && (
                ctx.iter.peek() == L"+" ||
                ctx.iter.peek() == L"-" ||
                util::isBitOp(ctx.iter.peek()[0])
            )
        ){
            assert(
                ctx.iter.peek() == L"+" ||
                ctx.iter.peek() == L"-" ||
                util::isBitOp(ctx.iter.peek()[0])
            );
            power(ctx);
        }
    }
}