#include <parserCore.h>

#include <assert.h>
#include <numeric>

#include <parserTypes.h>
#include <stmtProcessor.h>
#include <util.h>
#include <syntaxError.h>

using namespace synErr;
using namespace parserTypes;
using namespace util;
namespace parserCore{
    

    void program(Context& ctx){
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
    void stmt(Context& ctx){
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
        }else if(isFunccall(ctx.iter.peek(),ctx.iter.peek(1))){
            stmtProcessor::executeFunction(ctx,funcCall(ctx));
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
            }else if(isAssignOp(text)){
                assign(ctx);
            }
        }
    }
    void func(Context& ctx){
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
    void For(Context& ctx){
        assert(ctx.iter.next() == L"for");
        std::wstring varname=ctx.iter.next();
        assert(ctx.iter.next() == L"in");
        if(ctx.iter.peek(1)==L"..."){
            Range target=range(ctx);
            assert(ctx.iter.next() == L"{");

            stmtProcessor::Forr(ctx,target.first,target.second);
        }else{
            std::wstring target=value(ctx);
            assert(ctx.iter.next() == L"{");

            stmtProcessor::For(ctx,varname,target);
        }
        while(ctx.iter.hasData()){
                if(ctx.iter.peek()==L"}")break;
                stmt(ctx);
            }
        assert(ctx.iter.next() == L"}");
    }
    void put(Context& ctx){
        std::wstring target = value(ctx);
        assert(ctx.iter.next()==L"<<");
        expr(ctx);
        std::wcout<<"Data write To "<<target<<std::endl;
    }
    Arg arg(Context& ctx){
        return std::make_pair(
            ctx.iter.next(), // type
            ctx.iter.next()  // name
        );
    }
    std::wstring value(Context& ctx){
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
        }else{
            syntaxError(ctx,L"is not value type");
            throw ""; // do not call this
        }
    }
    std::wstring ptr(Context& ctx){
        assert(ctx.iter.next()==L"[");
        [[maybe_unused]] bool isImmutable=isdigit(ctx.iter.peek()[0]);
        std::wstring base=value(ctx);
        std::wstring offs;
        while(ctx.iter.peek()==L"+"){
            ctx.iter.next();
            offs=ctx.iter.next();
        }
        assert(ctx.iter.next()==L"]");
        return L"ptr"+base+L" "+offs+L"<";
    }
    std::wstring attribute(Context& ctx){
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
    std::wstring editable(Context& ctx){
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
    std::wstring ident(Context& ctx){
        std::wstring text=ctx.iter.next();
        // check word?
        if(!isalpha(text[0])){
            processError(ctx,L"isn't ident");
        }
        return text;
    }
    std::wstring constant(Context& ctx){
        std::wstring text=ctx.iter.next();
        // check integer?
        if(!isdigit(text[0]) || text[0]!=L'"')
            processError(ctx,L"isn't constant integer");
        return text;
    }
    void assign(Context& ctx){
        std::wstring target=editable(ctx);
        std::wstring op=ctx.iter.next();
        if(op==L"++"){
            std::wcout<<target<<"++"<<std::endl;
        }else if(op==L"--"){
            std::wcout<<target<<"--"<<std::endl;
        }else{
            expr(ctx);
            std::wcout<<target<<"edit by "<<op<<std::endl;
        }
    }
    struct power power (Context& ctx){
        struct power ret;
        if(ctx.iter.peek()==L"("){
            ret.type=power::EXPR;
            // inner type
            ctx.iter.next();
            return power(ctx);
            assert(ctx.iter.next()==L")");
        }else if(isFunccall(ctx.iter.peek(),ctx.iter.peek(1))){
            ret.type=power::FUNCCALL;
            ret.func=&funcCall(ctx);
        }else if(isInt(ctx.iter.peek())){
            ret.type=power::IMM;
            ret.imm=Int(ctx);
        }else if(isSingle(ctx.iter.peek())){
            ret.type=power::VAR;
            ret.var=ctx.iter.next();
        }else{
            ret.type=power::EXPR;
            ret.expr=expr(ctx);
        }
        return ret;

    }
    struct expo expo  (Context& ctx){
        struct expo val;
        val.parts.emplace_back(power(ctx));
        while(ctx.iter.hasData() && ctx.iter.peek() == L"**"){
            assert(ctx.iter.next() == L"**");
            val.parts.emplace_back(power(ctx));
        }
        return val;
    }
    struct term term  (Context& ctx){
        struct term ret;

        ret.parts.emplace_back(expo(ctx));
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
            ret.parts.emplace_back(expo(ctx));
        }
        return ret;
    }
    struct expr expr  (Context& ctx){
        struct expr ret;
        struct term part;
        std::wstring text=ctx.iter.peek();
        
        part=term(ctx);
        if(text==L"+"){
            ctx.iter.next(); // read
        }else if(text==L"-"){
            ctx.iter.next(); // read
            struct expo tmp;
            tmp.parts.emplace_back(L"-1");

            part.parts.emplace_back(tmp);
        }
        ret.parts.emplace_back(part);
        
        while(
            ctx.iter.hasData() && (
                ctx.iter.peek() == L"+" ||
                ctx.iter.peek() == L"-" ||
                isBitOp(ctx.iter.peek()[0])
            )
        ){
            auto text=ctx.iter.next();
            assert(
                text == L"+" ||
                text == L"-" ||
                isBitOp(text[0])
            );

            part=term(ctx);
            if(text==L"-"){
                struct expo tmp;
                tmp.parts.emplace_back(L"-1");
                part.parts.emplace_back(tmp);
            }
            ret.parts.emplace_back(part);
        }
        return ret;
    }
    Range range  (Context& ctx){
        int start=Int(ctx);
        assert(ctx.iter.next()==L"...");
        int end=Int(ctx);
        // convert start/end: wstr => int
        return std::make_pair(start,end);
    }
    int Int  (Context& ctx){
        std::wstring text=ctx.iter.next();
        if(!isdigit(text[0])){
            syntaxError(ctx,L"is not integer");
        }
        // convert test<wstr> to value<int>
        return toInt(text);
    }
    void If(Context& ctx){
        assert(ctx.iter.next()==L"if");
        struct cond conditional = cond(ctx);
        assert(ctx.iter.next() == L"{");
        stmtProcessor::If(ctx);
        assert(ctx.iter.next() == L"}");
    }
    struct cond cond  (Context& ctx){
        struct cond conditional;
        std::wstring text;
        conditional.first=cond_inner(ctx);
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"&&" ||
                ctx.iter.peek() == L"||"
            )
        ){
            auto op=ctx.iter.next();
            assert(op == L"&&" ||op == L"||");
            if(op==L"&&"){
                conditional.conds.emplace_back(std::make_pair(
                    cond::AND,cond_inner(ctx)
                ));
            }else if(op==L"||"){
                conditional.conds.emplace_back(std::make_pair(
                    cond::OR,cond_inner(ctx)
                ));
            }
        }
        return conditional;
    }
    struct condChild cond_inner  (Context& ctx){
        struct condChild cond;
        std::wstring text;
        if(isCondOpFull(ctx.iter.peekSafe(1))){
            cond.val1=expr(ctx);
            std::wstring op=ctx.iter.next();
            if(op == L"<"){
                cond.op=condChild::LT;
            }else if(op == L">"){
                cond.op=condChild::GT;
            }else if(op == L"<="){
                cond.op=condChild::LE;
            }else if(op == L">="){
                cond.op=condChild::GE;
            }else if(op == L"=="){
                cond.op=condChild::EQU;
            }else if(op == L"!="){
                cond.op=condChild::NEQ;
            }
            cond.val2=expr(ctx);
        }else{
            cond.op=condChild::SINGLE;
            cond.single=value(ctx);
        }
        return cond;
    }
    void While(Context& ctx){
        assert(ctx.iter.next()==L"while");
        struct cond conditional = cond(ctx);
        assert(ctx.iter.next() == L"{");
        stmtProcessor::While(ctx);
        assert(ctx.iter.next() == L"}");
    }
    struct ExecFunc funcCall(Context& ctx){
        struct ExecFunc ret;
        if(ctx.iter.peek()==L"func"){
            // address based call
            ret.type=ExecFunc::ADDRESS;

            assert(ctx.iter.next()==L"func");
            assert(ctx.iter.next()==L"[");
            ret.funcAddr=expr(ctx);
            assert(ctx.iter.next()==L"]");
        }else{
            // name based call
            ret.type=ExecFunc::Name;

            if(ctx.iter.peekSafe(1)==L"."){
                ret.funcId=attribute(ctx);
            }else{
                ret.funcId=ident(ctx);
            }
        }
        assert(ctx.iter.next()==L"(");
        if(ctx.iter.peek() != L")"){
            ret.args.emplace_back(ctx.iter.next());
        }
        while(ctx.iter.peek() != L")"){
            assert(ctx.iter.next()==L",");
            ret.args.emplace_back(ctx.iter.next());
        }
        assert(ctx.iter.next()==L")");
        return ret;
    }
}