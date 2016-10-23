#ifndef __AMMO_H__
#define __AMMO_H__
#include "Item.h"
class Ammo :
	public Item
{
	// Member Methods:
public:
	Ammo();
	~Ammo();

	int GetQuantity();
	void SetQuantity(int quantity);
protected:

private:

	// Member Data:
public:

protected:

	int mi_quantity;
private:

};

#endif //__AMMO_H__