#include <condutil.h>

#include <parserTypes.h>

using namespace parserTypes;

condChild util::invertConditional(condChild source){
    condChild::Type old=source.op;
    switch(old){
    case condChild::SINGLE:
        source.op=condChild::SINGLE_INV;
        break;
    case condChild::SINGLE_INV:
        source.op=condChild::SINGLE;
        break;
    case condChild::EQU:
        source.op=condChild::NEQ;
        break;
    case condChild::NEQ:
        source.op=condChild::EQU;
        break;
    case condChild::LT:
        source.op=condChild::GE;
        break;
    case condChild::GT:
        source.op=condChild::LE;
        break;
    case condChild::GE:
        source.op=condChild::LT;
        break;
    case condChild::LE:
        source.op=condChild::GT;
        break;
    }
    return source;
}
condChild invertConditional(cond source){
    
}