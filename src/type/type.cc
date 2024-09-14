#include "../../Include/type/type.h"

bool Type::isConstant() {return type_ == DBOT;}

bool Type::isSimple(){return type_ < DSIMPLE}

Type Type::BOTTOM = Type(Type::DBOT);
Type::Type(unsigned int type) : type_(type) {}