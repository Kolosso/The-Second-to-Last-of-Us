#include "Ammo.h"


Ammo::Ammo()
: mi_quantity(0)
{
}


Ammo::~Ammo()
{
}

int Ammo::GetQuantity()
{
	return mi_quantity;
}

void Ammo::SetQuantity(int quantity)
{
	mi_quantity = quantity;
}
