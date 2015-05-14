#include"SanContainerDef.h"
using namespace std;
using namespace San;
stSANUNKNOWN::stSANUNKNOWN(const SString &strDesc, const SVALUE &Value)
	:description(strDesc), value(Value)
{
}
stSANUNKNOWN::~stSANUNKNOWN()
{
}
bool stSANUNKNOWN::operator==(const stSANUNKNOWN &Variable) const
{
	return (this->description == Variable.description) && (this->value == Variable.value);
}
bool stSANUNKNOWN::operator!=(const stSANUNKNOWN &Variable) const
{
	return (this->description != Variable.description) || (this->value != Variable.value);
}
stSANSIMPLELOCK::stSANSIMPLELOCK(const bool LockState)
	:bLock(LockState)
{
}
stSANSIMPLELOCK::~stSANSIMPLELOCK()
{
}
void stSANSIMPLELOCK::iLock()
{
	while (this->bLock){};
	this->bLock = true;
}
void stSANSIMPLELOCK::iUnLock()
{
	this->bLock = false;
}
void stSANSIMPLELOCK::lock()
{
	this->iLock();
}
void stSANSIMPLELOCK::unlock()
{
	this->iUnLock();
}