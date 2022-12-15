#pragma once

#include "2d/vector2d.h"

class TeamZone
{
private:
	Vector2D m_position;
	Vector2D m_size;
	int m_team;
	double m_left;
	double m_top;
	double m_right;
	double m_bottom;

	int m_spawnSize;
	Vector2D m_spawnRailGun;
	Vector2D m_spawnRocketLauncher;
	Vector2D m_spawnShotGun;

public:
	TeamZone(Vector2D position, Vector2D size, int team);
	void Render();
	void Update();
	bool IsInZone(Vector2D position);

	int GetSpawnSize() { return m_spawnSize; }
	Vector2D GetSpawnRailGun() { return m_spawnRailGun; }
	Vector2D GetSpawnRocketLauncher() { return m_spawnRocketLauncher; }
	Vector2D GetSpawnShotGun() { return m_spawnShotGun; }

};

