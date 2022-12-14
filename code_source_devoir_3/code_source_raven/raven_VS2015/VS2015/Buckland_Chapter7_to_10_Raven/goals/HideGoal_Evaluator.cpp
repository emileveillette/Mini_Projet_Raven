#include "HideGoal_Evaluator.h"
#include "../navigation/Raven_PathPlanner.h"
#include "../Raven_ObjectEnumerations.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"

#include "Goal_Think.h"
#include "Raven_Goal_Types.h"




//---------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double HideGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	//value used to tweak the desirability
	const double Tweaker = 0.9;

	double scorer;

	double Desirability = 0;

	if (pBot->GetTargetSys()->isTargetWithinFOV())
	{
		scorer = pBot->Score() / 11;
		Clamp(scorer, 0, 0.3);
		Desirability = Tweaker * ((1 - Raven_Feature::Health(pBot)) / 3 + scorer);
	}

	//ensure the value is in the range 0 to 1
	Clamp(Desirability, 0, 1);

	return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void HideGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
	pBot->GetBrain()->AddGoal_Hide();
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void HideGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "EX: " + ttos(CalculateDesirability(pBot), 2));
}