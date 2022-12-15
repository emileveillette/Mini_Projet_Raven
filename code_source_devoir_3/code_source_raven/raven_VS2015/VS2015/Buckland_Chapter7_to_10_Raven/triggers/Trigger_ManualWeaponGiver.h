#ifndef MANUAL_WEAPON_GIVER_H
#define MANUAL_WEAPON_GIVER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     Trigger_WeaponGiver.h
//
//  Author:   Mat Buckland
//
//  Desc:     This trigger 'gives' the triggering bot a weapon of the
//            specified type 
//
//-----------------------------------------------------------------------------
#include "Triggers/Trigger.h"
#include "../Raven_Bot.h"
#include <iosfwd>


class Trigger_ManualWeaponGiver : public Trigger<Raven_Bot>
{
private:

	//vrtex buffers for rocket shape
	std::vector<Vector2D>         m_vecRLVB;
	std::vector<Vector2D>         m_vecRLVBTrans;

public:

	//this type of trigger is created when reading a map file
	Trigger_ManualWeaponGiver(int id, double x, double y, double r);

	void Activate();

	//if triggered, this trigger will call the PickupWeapon method of the
	//bot. PickupWeapon will instantiate a weapon of the appropriate type.
	void Try(Raven_Bot*);

	void Update() {};

	//draws a symbol representing the weapon type at the trigger's location
	void Render();

	void Read(double x, double y, double r);
};




#endif