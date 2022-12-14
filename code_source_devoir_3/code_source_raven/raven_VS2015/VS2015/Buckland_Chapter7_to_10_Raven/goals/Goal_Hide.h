#ifndef GOAL_HIDE_H
#define GOAL_HIDE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Goal_Hide.h
//
//  Author: Mathis THOMAS
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"





class Goal_Hide : public Goal<Raven_Bot>
{
private:

	//the position the bot is moving to
	Vector2D  m_vPosition;

public:

	Goal_Hide(Raven_Bot* pBot);

	void Activate();

	int  Process();

	void Terminate();

	void Render();

};






#endif
