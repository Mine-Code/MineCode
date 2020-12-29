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
        std::wstring data=expr(ctx);
        std::wcout<<data<<" To "<<target<<std::endl;
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
            std::wstring value=expr(ctx);
            std::wcout<<target<<op<<value<<std::endl;
        }
    }
    std::wstring power (Context& ctx){
        if(ctx.iter.peek()==L"("){
            // inner type
            ctx.iter.next();
            std::wstring text=expr(ctx);
            assert(ctx.iter.next()==L")");
            if(isSingle(text)){
                return text;
            }else{
                return L"("+text+L")";
            }
        }
        if(isFunccall(ctx.iter.peek(),ctx.iter.peek(1))){
            struct ExecFunc execfunc=funcCall(ctx);
            std::wstring str;
            str+=execfunc.funcId;
            str+=L"(";
            for(auto arg:execfunc.args){
                str+=arg;
            }
            str+=L")";
            return str;
        }
        return value(ctx);

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
        std::vector<std::wstring> parts;

        std::wstring tmp;
        parts.emplace_back(L"*"+expo(ctx));
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
            parts.emplace_back(text+expo(ctx));
        }
        
        std::wstring others;
        int numer=1;
        int denom=1;
        for(auto part:parts){
            if(isInt(part.substr(1)) && (part[0]=='/' || part[0]=='*')){
                if(part[0]=='*'){
                    numer*=toInt(part.substr(1));
                }else if(part[0]=='/'){
                    denom*=toInt(part.substr(1));
                }
            }else{
                others+=part;
            }
        }
        if(!others.empty()){
            others=others.substr(1);
        }

        // reduction of fraction

        int gcd=std::gcd(numer,denom);
        numer/=gcd;
        denom/=gcd;
        // end: reduction of fraction

        std::wstring ret;
        if(numer==1 && others.empty()){
            ret=L"1";
        }else if(numer==1 && !others.empty()){
            ret=others;
        }else if(numer!=1){
            ret=std::to_wstring(numer);
            if(!others.empty()){
                ret+=L"*"+others;
            }
        }
        if(denom!=1){
            ret+=L"/"+std::to_wstring(denom);
        }
        return ret;
    }
    struct expr expr  (Context& ctx){
        std::vector<std::wstring> parts;

        std::wstring first;
        std::wstring text=ctx.iter.peek();
        if(text==L"+" || text==L"-"){
            first=ctx.iter.next();
        }else{
            first=L"+";
        }
        first+=term(ctx);

        parts.emplace_back(first);
        
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
            parts.emplace_back(text+term(ctx));
        }

        std::wstring ret;
        int imm=0;
        for(auto part:parts){
            if(isimm(part)){
                imm+=toInt(part);
            }else{
                ret+=part;
            }
        }
        std::wstring immStr = imm==0 ? L"" : std::to_wstring(imm);
        if(ret[0]=='+') ret=ret.substr(1);
        if(!ret.empty() && !immStr.empty()){
            ret=immStr+L"+"+ret;
        }else if(!ret.empty()){
            ret=ret;
        }else if(!immStr.empty()){
            ret=immStr;
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
            cond.val1=value(ctx);
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
            ret.funcId=expr(ctx);
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