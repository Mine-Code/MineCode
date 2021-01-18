#include <parserCore.h>

#include <numeric>
#include <locale>
#include <codecvt>

#include <parserTypes.h>
#include <stmtProcessor.h>
#include <util.h>
#include <syntaxError.h>
#include <myassert.h>

using namespace synErr;
using namespace parserTypes;
using namespace util;
namespace parserCore{
    

    void program(Context& ctx){
        if(ctx.iter.peek()==L"#"){
            ctx.stream<<"# do once"<<std::endl;
            assert(ctx.iter.next() == L"#"   , L"mismatch preprecessor operation");
            assert(ctx.iter.next() == L"do"  , L"mismatch preprecessor operation");
            assert(ctx.iter.next() == L"once", L"mismatch preprecessor operation");
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
        }else if(isFunccall(ctx.iter.peek(),ctx.iter.peek(1))){
            stmtProcessor::executeFunction(ctx,funcCall(ctx));
        }else if(text==L"func"){
            func(ctx);
        }else{
            //put / assign
            assert(ctx.iter.hasData(),L"does not have data in iterator");
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
        assertChar("func");
        std::wstring functionName=ctx.iter.next();
        ctx.stream<<"# "<<"funcName:"<<functionName<<std::endl;
        assertChar("(");

        // read arguments
        std::vector<Arg> args;
        // args=(type,name)

        if(ctx.iter.peek() != L")"){
            args.emplace_back(arg(ctx));
        }
        while(ctx.iter.peek() != L")"){
            assertChar(",");
            args.emplace_back(arg(ctx));
        }
        // end: read arguments
        assertChar(")");
        assertChar("{");
        stmtProcessor::Func(ctx);
        assertChar("}");
    }
    void For(Context& ctx){
        assertChar("for");
        std::wstring varname=ctx.iter.next();
        assertChar("in");
        ctx.stream<<"# for ";
        if(ctx.iter.peek(1)==L"..."){
            ctx.stream<<" range"<<std::endl;
            Range target=range(ctx);
            assertChar("{");
            stmtProcessor::Forr(ctx,target.first,target.second);
        }else{
            ctx.stream<<" iter "<<std::endl;
            std::wstring target=ident(ctx);
            assertChar("{");

            stmtProcessor::For(ctx,varname,target);
        }
        assertChar("}");
    }
    void put(Context& ctx){
        auto converter=std::wstring_convert<std::codecvt_utf8<wchar_t>>();

        std::string target = converter.to_bytes(ident(ctx));
        assertChar("<<");
        struct expr val=expr(ctx);
        assert(ctx.puts.count(target)==1,L"Puts Not found");
        ctx.stream<<converter.from_bytes(ctx.puts[target]);
    }
    Arg arg(Context& ctx){
        return std::make_pair(
            ctx.iter.next(), // type
            ctx.iter.next()  // name
        );
    }
    struct value value(Context& ctx){
        struct value ret;
        wchar_t ch=ctx.iter.peek()[0];
        if(ctx.iter.peek()==L"["){
            ret.type=value::PTR;
            ret.pointer=ptr(ctx);
        }else if(isalpha(ch)){
            ret.type=value::IDENT;
            ret.ident=ident(ctx);
        }else if(ch==L'"'){
            ret.type=value::STR;
            ret.str=ctx.iter.next();
        }else if(isdigit(ch)){
            ret.type=value::IMM;
            ret.imm=util::toInt(ctx.iter.next());
        }else{
            syntaxError(ctx,L"is not value type ",__FILE__,__func__,__LINE__);
            throw ""; // do not call this
        }
        return ret;
    }
    struct ptr ptr(Context& ctx){
        struct ptr ret;
        assertChar("[");
        [[maybe_unused]] bool isImmutable=isdigit(ctx.iter.peek()[0]);
        {
            struct value *tmp=new struct value;
            *tmp=value(ctx);
            ret.base=tmp;
        }
        while(ctx.iter.peek()==L"+"){
            ctx.iter.next();
            ret.offsets.emplace_back(util::toInt(ctx.iter.next()));
        }
        assertChar("]");
        return ret;
    }
    struct value editable(Context& ctx){
        struct value ret;
        if(ctx.iter.peek()==L"["){
            ret.type=value::PTR;
            ret.pointer=ptr(ctx);
        }else{
            ret.type=value::IDENT;
            ret.ident=ident(ctx);
        }
        return ret;
    }
    std::wstring ident(Context& ctx){
        std::wstring text=ctx.iter.next();
        // check word?
        if(!isalpha(text[0])){
            processError(ctx,L"isn't ident",__FILE__,__func__,__LINE__);
        }
        // read under the `.`
        while (ctx.iter.peek()==L".")
        {
            ctx.iter.next();
            text+=L"."+ctx.iter.next();
        }
        return text;
    }
    struct value constant(Context& ctx){
        struct value ret;
        std::wstring text=ctx.iter.next();
        // check integer?
        if(!isdigit(text[0]) || text[0]!=L'"')
            processError(ctx,L"isn't constant integer",__FILE__,__func__,__LINE__);

        if(isdigit(text[0])){
            ret.type=value::IMM;
            ret.imm=util::toInt(text);
        }else if(text[0]=='"'){
            ret.type=value::STR;
            ret.str=text;
        }
        return ret;
    }
    void assign(Context& ctx){
        struct value target=editable(ctx);
        std::wstring op=ctx.iter.next();
        struct expr value;
        if(not (op==L"++" or op==L"--") ){
            value=expr(ctx);
        }
        stmtProcessor::Assign(ctx,target,op,value);
    }
    struct power power (Context& ctx){
        struct power ret;
        if(ctx.iter.peekSafe()==L"("){
            ret.type=power::EXPR;
            // inner type
            ctx.iter.next();
            ret.expr=expr(ctx);
            assertChar(")");
        }else if(isFunccall(ctx.iter.peekSafe(),ctx.iter.peekSafe(1))){
            ret.type=power::FUNCCALL;
            struct ExecFunc func=funcCall(ctx);
            ret.func=&func;
        }else if(isInt(ctx.iter.peekSafe())){
            ret.type=power::IMM;
            ret.imm=Int(ctx);
        }else if(isSingle(ctx.iter.peek()) && ctx.iter.peek()!=L"["){
            ret.type=power::VAR;
            ret.var=ctx.iter.next();
        }else if(ctx.iter.peek()==L"["){
            ret.type=power::PTR;
            ret.ptr=ptr(ctx);
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
            assertChar("**");
            val.parts.emplace_back(power(ctx));
        }
        return val;
    }
    struct term term  (Context& ctx){
        struct term ret;
        {
            expo_wrap wrap;
            wrap.type=expo_wrap::MUL;
            wrap.value=expo(ctx);
            ret.parts.emplace_back(wrap);
        }
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
            , L"excepted '*' or '/','%'"
            );

            expo_wrap Elem;
            if(text==L"*"){
                Elem.type=expo_wrap::MUL;
            }else if(text==L"/"){
                Elem.type=expo_wrap::DIV;
            }else if(text==L"%"){
                Elem.type=expo_wrap::MOD;
            }
            Elem.value=expo(ctx);
            ret.parts.emplace_back(Elem);
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
            struct power pow;
            pow.type=power::IMM;
            pow.imm=-1;

            struct expo tmp;
            tmp.parts.emplace_back(pow);

            struct expo_wrap wrap;
            wrap.value=tmp;
            wrap.type=expo_wrap::MUL;

            part.parts.emplace_back(wrap);
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
            ,   L"excepted '+' or '+', bitOperator"
            );

            part=term(ctx);
            if(text==L"-"){
                ctx.iter.next(); // read
                struct power pow;
                pow.type=power::IMM;
                pow.imm=-1;

                struct expo tmp;
                tmp.parts.emplace_back(pow);

                struct expo_wrap wrap;
                wrap.value=tmp;
                wrap.type=expo_wrap::MUL;
                
                part.parts.emplace_back(wrap);
            }
            ret.parts.emplace_back(part);
        }
        return ret;
    }
    Range range  (Context& ctx){
        int start=Int(ctx);
        assertChar("...");
        int end=Int(ctx);
        // convert start/end: wstr => int
        return std::make_pair(start,end);
    }
    int Int  (Context& ctx){
        std::wstring text=ctx.iter.next();
        if(!isdigit(text[0])){
            syntaxError(ctx,L"is not integer",__FILE__,__func__,__LINE__);
        }
        // convert test<wstr> to value<int>
        return toInt(text);
    }
    void If(Context& ctx){
        assertChar("if");
        struct cond conditional = cond(ctx);
        ctx.stream<<"# if"<<std::endl;
        assertChar("{");
        stmtProcessor::If(ctx,conditional);
        assertChar("}");
        ctx.stream<<"# fi"<<std::endl;
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
            assert(op == L"&&" ||op == L"||"
                ,L"excepted '&&' or '||'");
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
    struct condAnd condAnd  (Context& ctx){
        struct condAnd ret;
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"&&"
            )
        ){
            auto op=ctx.iter.next();
            assert(op == L"&&",L"excepted '&&'");
            ret.conds.emplace_back(cond_inner(ctx));
        }
        return ret;
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
        assertChar("while");
        struct cond conditional = cond(ctx);
        ctx.stream<<"# while {"<<std::endl;
        assertChar("{");
        stmtProcessor::While(ctx);
        assertChar("}");
        ctx.stream<<"# }"<<std::endl;
    }
    struct ExecFunc funcCall(Context& ctx){
        struct ExecFunc ret;
        if(ctx.iter.peek()==L"func"){
            // address based call
            ret.type=ExecFunc::ADDRESS;

            assert(ctx.iter.next()==L"func",L"excepted 'func'");
            assertChar("[");
            ret.funcAddr=Int(ctx);
            assertChar("]");
        }else{
            // name based call
            ret.type=ExecFunc::Name;
            ret.funcId=ident(ctx);
        }
        assertChar("(");
        if(ctx.iter.peek() != L")"){
            ret.args.emplace_back(expr(ctx));
        }
        while(ctx.iter.peek() != L")"){
            assertChar(",");
            ret.args.emplace_back(expr(ctx));
        }
        assertChar(")");
        return ret;
    }
}