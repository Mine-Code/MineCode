#include <parserCore.h>

#include <numeric>

#include <parserTypes.h>
#include <stmtProcessor.h>
#include <util.h>
#include <syntaxError.h>
#include <myassert.h>
#include <mcl.h>

using namespace synErr;
using namespace parserTypes;
using namespace util;

std::wstring convPut(std::wstring src);

namespace parserCore{
    

    void program(parserTypes::parserContext& ctx){
        if(ctx.iter.peekSafe()==L"#"){
            ctx.stream<<"# do once"<<std::endl;
            assert(ctx.iter.next() == L"#"   , L"mismatch preprecessor operation");
            assert(ctx.iter.next() == L"do"  , L"mismatch preprecessor operation");
            assert(ctx.iter.next() == L"once", L"mismatch preprecessor operation");
            // TODO: implement do once
        }

        ctx.stream<<"# begin program\n";
        
        while(ctx.iter.hasData()){
            stmt(ctx);
        }

        ctx.stream<<"# end program\n";
        ctx.stream<<"__ret:\n";
    }
    void stmt(parserTypes::parserContext& ctx){
        // stmt Switcher
        std::wstring text=ctx.iter.peek();
        if(text==L"for"){
            For(ctx);
            return;
        }
        if(text==L"while"){
            While(ctx);
            return;
        }
        if(text==L"if"){
            If(ctx);
            return;
        }
        if(text==L"mcl"){
            mcl(ctx);
            return;
        }
        if(text==L"return"){
            ctx.iter.next();
            ctx.Asm->Jump(L"__ret");
            return;
        }
        if(text==L"func"){
            if(ctx.iter.peekSafe(1)==L"["){
                stmtProcessor::executeFunction(ctx,funcCall(ctx));
            }else{
                func(ctx);
            }
            return;
        }

        // skip one 'value' and read one
        auto backup=ctx.iter.index;
        value(ctx);
        text=ctx.iter.peek();
        ctx.iter.index=backup;
        // done skip and read
        if(text==L"<<"){
            put(ctx);
        }else if(text==L"("){
            stmtProcessor::executeFunction(ctx,funcCall(ctx));
        }else if(isAssignOp(text)){
            assign(ctx);
        }
    }
    void func(parserTypes::parserContext& ctx){
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
    void For(parserTypes::parserContext& ctx){
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
    void put(parserTypes::parserContext& ctx){
        std::string target = util::wstr2str(ident(ctx));
        assertChar("<<");
        // get end
        auto start=ctx.iter.index;
        struct expr val=expr(ctx);
        auto end=ctx.iter.index;
        ctx.iter.index=start;
        // get end
        std::wstring expression;
        ctx.iter.index=0; // enable absolute get
        for (size_t i = start; i < end; i++)
        {
            expression+=ctx.iter.peekSafe(i);
        }
        ctx.iter.index=end;
        assert(ctx.puts.count(target)==1,L"Puts Not found");
        ctx.stream<<util::str2wstr(ctx.puts[target]);
    }
    Arg arg(parserTypes::parserContext& ctx){
        return std::make_pair(
            ctx.iter.next(), // type
            ctx.iter.next()  // name
        );
    }
    struct value value(parserTypes::parserContext& ctx){
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
    struct ptr ptr(parserTypes::parserContext& ctx){
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
    struct value editable(parserTypes::parserContext& ctx){
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
    std::wstring ident(parserTypes::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        // check word?
        if(!isalpha(text[0])){
            processError(ctx,L"isn't ident",__FILE__,__func__,__LINE__);
        }
        // read under the `.`
        while (ctx.iter.peek()==L"." || ctx.iter.peek()==L"::")
        {
            std::wstring joint=ctx.iter.next();
            text+=joint+ctx.iter.next();
        }
        return text;
    }
    struct value constant(parserTypes::parserContext& ctx){
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
    void assign(parserTypes::parserContext& ctx){
        struct value target=editable(ctx);
        std::wstring op=ctx.iter.next();
        struct expr value;
        if(! (op==L"++" || op==L"--") ){
            value=expr(ctx);
        }
        stmtProcessor::Assign(ctx,target,op,value);
    }
    struct power power (parserTypes::parserContext& ctx){
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
    struct expo expo  (parserTypes::parserContext& ctx){
        struct expo val;
        val.parts.emplace_back(power(ctx));
        while(ctx.iter.hasData() && ctx.iter.peek() == L"**"){
            assertChar("**");
            val.parts.emplace_back(power(ctx));
        }
        return val;
    }
    struct term term  (parserTypes::parserContext& ctx){
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
    struct expr expr  (parserTypes::parserContext& ctx){
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
                isBitOpFull(ctx.iter.peek())
            )
        ){
            auto text=ctx.iter.next();
            assert(
                text == L"+" ||
                text == L"-" ||
                isBitOpFull(text)
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
    Range range  (parserTypes::parserContext& ctx){
        int start=Int(ctx);
        assertChar("...");
        int end=Int(ctx);
        // convert start/end: wstr => int
        return std::make_pair(start,end);
    }
    int Int  (parserTypes::parserContext& ctx){
        std::wstring text=ctx.iter.next();
        if(!isdigit(text[0])){
            syntaxError(ctx,L"is not integer",__FILE__,__func__,__LINE__);
        }
        // convert test<wstr> to value<int>
        return toInt(text);
    }
    void If(parserTypes::parserContext& ctx){
        assertChar("if");
        struct cond conditional = cond(ctx);
        ctx.stream<<"# if"<<std::endl;
        assertChar("{");
        stmtProcessor::If(ctx,conditional);
        assertChar("}");
        ctx.stream<<"# fi"<<std::endl;
    }
    struct cond cond  (parserTypes::parserContext& ctx){
        struct cond ret;
        std::wstring text;
        ret.conds.emplace_back(condAnd(ctx));
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"||"
            )
        ){
            assertChar("||");
            ret.conds.emplace_back(condAnd(ctx));
        }
        return ret;
    }
    struct condAnd condAnd  (parserTypes::parserContext& ctx){
        struct condAnd ret;
        ret.conds.emplace_back(cond_inner(ctx));
        while(
            ctx.iter.hasData() &&
            (
                ctx.iter.peek() == L"&&"
            )
        ){
            assertChar("&&");
            ret.conds.emplace_back(cond_inner(ctx));
        }
        return ret;
    }
    struct condChild cond_inner  (parserTypes::parserContext& ctx){
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
    void While(parserTypes::parserContext& ctx){
        assertChar("while");
        struct cond conditional = cond(ctx);
        ctx.stream<<"# while {"<<std::endl;
        assertChar("{");
        stmtProcessor::While(ctx,conditional);
        assertChar("}");
        ctx.stream<<"# }"<<std::endl;
    }
    void mcl(parserTypes::parserContext& ctx){
        assertChar("mcl");
        (*ctx.wraper)<<util::wstr2str(ctx.iter.next());
    }
    struct ExecFunc funcCall(parserTypes::parserContext& ctx){
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
    }}

std::wstring convPut(std::wstring src){
    std::wstring ret;
    auto iter=iterator<wchar_t>(util::convToVector<wchar_t,std::wstring>(src));
    while(iter.hasData()){
        wchar_t ch=iter.next();
        wchar_t ne=iter.peekSafe(1);
        if(ch=='$' && ne=='$'){
            ret+='$';
            iter.next();
        }else if(
            ch=='$' &&
            iter.peekSafe(0)=='a' &&
            iter.peekSafe(1)=='r' &&
            iter.peekSafe(2)=='g'
        ){
            ret+='#';
            iter.index+=3;
        }else{
            ret+=ch;
        }
    }
    return ret;
}