#include <parserCore.h>

#include <assert.h>

#include <parserContext.h>
#include <stmtProcessor.h>
#include <util.h>
#include <syntaxError.h>

using namespace synErr;

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
            While(ctx);
        }else if(text==L"if"){
            If(ctx);
        }else if(text==L"func"){
            func(ctx);
        }else{
            //put / assign
            assert(ctx.iter.hasData());
            // skip one 'value' and read one
            auto backup=ctx.iter.index;
            value(ctx);
            text=ctx.iter.peek();
            ctx.iter.index=backup;
            // done skip and read
            if(text==L"<<"){
                put(ctx);
            }else if(util::isAssignOp(text)){
                assign(ctx);
            }
        }
    }
    void func(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"func");
        std::wstring functionName=ctx.iter.next();
        std::wcout<<"funcName:"<<functionName<<std::endl;
        assert(ctx.iter.next()==L"(");

        // read arguments
        std::vector<Arg> args;
        // args=(type,name)

        if(ctx.iter.peek() != L")"){
            args.emplace_back(arg(ctx));
        }
        while(ctx.iter.peek() != L")"){
            assert(ctx.iter.next()==L",");
            args.emplace_back(arg(ctx));
        }
        // end: read arguments
        assert(ctx.iter.next()==L")");
        assert(ctx.iter.next()==L"{");
        stmtProcessor::Func(ctx);
        assert(ctx.iter.next()==L"}");
    }
    void For(parserCtx::parserContext& ctx){
        assert(ctx.iter.next() == L"for");
        assert(ctx.iter.next() == L"(");
        std::wstring varname=ctx.iter.next();
        assert(ctx.iter.next() == L"in");
        if(ctx.iter.peek(1)==L"..."){
            Range target=range(ctx);
            assert(ctx.iter.next() == L")");
            assert(ctx.iter.next() == L"{");

            stmtProcessor::Forr(ctx,target.first,target.second);
        }else{
            std::wstring target=value(ctx);
            assert(ctx.iter.next() == L")");
            assert(ctx.iter.next() == L"{");

            stmtProcessor::For(ctx,varname,target);
        }
        while(ctx.iter.hasData()){
                if(ctx.iter.peek()==L"}")break;
                stmt(ctx);
            }
        assert(ctx.iter.next() == L"}");
    }
    void put(parserCtx::parserContext& ctx){
        std::wstring target = value(ctx);
        assert(ctx.iter.next()==L"<<");
        std::wstring data=expr(ctx);
        std::wcout<<data<<" To "<<target<<std::endl;
    }
    Arg arg(parserCtx::parserContext& ctx){
        return std::make_pair(
            ctx.iter.next(), // type
            ctx.iter.next()  // name
        );
    }
    std::wstring value(parserCtx::parserContext& ctx){
        wchar_t ch=ctx.iter.peek()[0];
        if(ctx.iter.peek()==L"["){
            return ptr(ctx);
        }else if(ctx.iter.peek(1)==L"."){
            return attribute(ctx);
        }else if(isalpha(ch)){
            return ident(ctx);
        }else if(ch==L'"'){
            return ctx.iter.next();
        }else if(isdigit(ch)){
            return ctx.iter.next();
        }
    }
    std::wstring ptr(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"[");
        bool isImmutable=isdigit(ctx.iter.peek()[0]);
        std::wstring base=value(ctx);
        std::wstring offs;
        while(ctx.iter.peek()==L"+"){
            ctx.iter.next();
            offs=ctx.iter.next();
        }
        assert(ctx.iter.next()==L"]");
        return L"ptr"+base+L" "+offs+L"<";
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
            processError(ctx,L"isn't ident");
        }
        return text;
    }
    std::wstring constant(parserCtx::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        // check integer?
        if(!isdigit(text[0]) || text[0]!=L'"')
            processError(ctx,L"isn't constant integer");
        return text;
    }
    void assign(parserCtx::parserContext& ctx){
        std::wstring target=editable(ctx);
        std::wstring op=ctx.iter.next();
        if(op==L"++"){
            std::wcout<<target<<"++"<<std::endl;
        }else if(op==L"--"){
            std::wcout<<target<<"--"<<std::endl;
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
            return L"("+text+L")";
        }
        return value(ctx);

    }
    std::wstring expo  (parserCtx::parserContext& ctx){
        std::wstring text;
        text+=power(ctx);
        while(ctx.iter.hasData() && ctx.iter.peek() == L"**"){
            assert(ctx.iter.next() == L"**");
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
            auto text=ctx.iter.next();
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
        // TODO: calculate (optimaizition)
        std::vector<std::wstring> parts;

        std::wstring first;
        std::wstring text=ctx.iter.peek();
        if(text==L"+" || text==L"-"){
            first=ctx.iter.next();
        }
        first+=term(ctx);

        parts.emplace_back(first);
        
        while(
            ctx.iter.hasData() && (
                ctx.iter.peek() == L"+" ||
                ctx.iter.peek() == L"-" ||
                util::isBitOp(ctx.iter.peek()[0])
            )
        ){
            auto text=ctx.iter.next();
            assert(
                text == L"+" ||
                text == L"-" ||
                util::isBitOp(text[0])
            );
            parts.emplace_back(text+term(ctx));
        }

        std::wstring ret;
        for(auto part:parts){
            ret+=part;
        }
        return ret;
    }
    Range range  (parserCtx::parserContext& ctx){
        int start=Int(ctx);
        assert(ctx.iter.next()==L"...");
        int end=Int(ctx);
        // convert start/end: wstr => int
        return std::make_pair(start,end);
    }
    int Int  (parserCtx::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        if(!isdigit(text[0])){
            syntaxError(ctx,L"is not integer");
        }
        // convert test<wstr> to value<int>
        int value=0;
        if(text[0]==L'0' && (text[1]==L'x' || text[1]==L'o') ){
            if(text[1]==L'x'){
                for(auto ch: text.substr(2)){
                    value*=0x10;
                    if(util::inRange(L'0',ch,L'9')){
                        value+=ch-'0';
                    }else{
                        value+=tolower(ch)-'a';
                    }
                }
            }else if(text[1]==L'o'){
                for(auto ch: text.substr(2)){
                    value*=8;
                    value+=ch-'0';
                }
            }
        }else{
            for(auto ch: text){
                value*=10;
                value+=ch-'0';
            }
        }
        return value;
    }
    void If(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"if");
        assert(ctx.iter.next() == L"(");
        std::wstring conditional = cond(ctx);
        std::wcout<<"if "<<conditional<<std::endl;
        assert(ctx.iter.next() == L")");
        assert(ctx.iter.next() == L"{");
        stmtProcessor::If(ctx);
        assert(ctx.iter.next() == L"}");
    }
    std::wstring cond  (parserCtx::parserContext& ctx){
        std::wstring text;
        text+=cond_inner(ctx);
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"&&" ||
                ctx.iter.peek() == L"||"
            )
        ){
            auto text=ctx.iter.next();
            assert(text == L"&&" ||text == L"||");
            text+=text+cond_inner(ctx);
        }
        return text;
    }
    std::wstring cond_inner  (parserCtx::parserContext& ctx){
        std::wstring text;
        if(util::isCondOpFull(ctx.iter.peekSafe(1))){
            std::wstring value1=expr(ctx);
            std::wstring op    =ctx.iter.next();
            std::wstring value2=expr(ctx);
            return value1+op+value2;
        }else{
            return value(ctx);
        }
        return text;
    }
    void While(parserCtx::parserContext& ctx){
        assert(ctx.iter.next()==L"while");
        assert(ctx.iter.next() == L"(");
        std::wstring conditional = cond(ctx);
        std::wcout<<"while "<<conditional<<std::endl;
        assert(ctx.iter.next() == L")");
        assert(ctx.iter.next() == L"{");
        stmtProcessor::While(ctx);
        assert(ctx.iter.next() == L"}");
    }
}