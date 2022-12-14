#ifndef RAVEN_HIDE_GOAL_EVALUATOR
#define RAVEN_HIDE_GOAL_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   ExploreGoal_Evaluator.h
//
//  Author: Mathis THOMAS
//
//  Desc:  class to calculate how desirable the goal of hide is
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"


class HideGoal_Evaluator : public Goal_Evaluator
{
public:

	HideGoal_Evaluator(double bias):Goal_Evaluator(bias){}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pEnt);

	void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

#endif