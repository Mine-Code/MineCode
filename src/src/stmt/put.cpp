#include <syntaxError.h>

#include <stmt/put.hpp>
parserTypes::stmt::Put::~Put() {}
void parserTypes::stmt::Put::exec(parserCore& ctx) {
  synErr::processError(&ctx, L"called not implemented function", __FILE__,
                       __func__, __LINE__);
}