#pragma once

class CTM2Hack;

enum AXES
{
	X_AXIS,
	Z_AXIS,
	Y_AXIS
};

class CCarMover
{
public:
	CCarMover(CTM2Hack *pHack);
	~CCarMover();

	void SwitchStatus(void);
	void MoveAxis(AXES axis, bool bDirection);

private:
	CTM2Hack *m_pHack;
	bool m_bEnabled;

	const float MOVE_DISTANCE = 3.0f;
};

