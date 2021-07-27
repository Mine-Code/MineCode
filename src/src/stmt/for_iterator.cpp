#include <syntaxError.h>

#include <stmt/for_iterator.hpp>
parserTypes::stmt::ForIter::~ForIter() {}
void parserTypes::stmt::ForIter::exec(parserCore& ctx) {
  synErr::processError(&ctx,
                       L"iterator based for statements is not implemented",
                       __FILE__, __func__, __LINE__);
}