#include "TeamZone.h"
#include <misc/Cgdi.h>

TeamZone::TeamZone(Vector2D position, Vector2D size, int team) : m_position(position), m_size(size), m_team(team),
m_left(position.x), m_top(position.y), m_right(position.x + size.x), m_bottom(position.y + size.y),
m_spawnSize(7)
{
	Vector2D spawnOffset = Vector2D((size.x / 3) / 2, size.y / 2);
	m_spawnRailGun = Vector2D(position.x, position.y) + spawnOffset;
	m_spawnRocketLauncher = Vector2D(position.x + size.x / 3, position.y) + spawnOffset;
	m_spawnShotGun = Vector2D(position.x + 2 * size.x / 3, position.y) + spawnOffset;
}

void TeamZone::Render()
{
	gdi->YellowPen();
	gdi->OrangeBrush();
	gdi->Rect(m_left, m_top, m_right, m_bottom);
}

void TeamZone::Update()
{

}

bool TeamZone::IsInZone(Vector2D position)
{
	return position.x > m_left && position.x < m_right && position.y > m_top && position.y < m_bottom;
}
