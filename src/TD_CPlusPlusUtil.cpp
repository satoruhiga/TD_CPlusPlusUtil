#include "TD_CPlusPlusUtil.h"

void ParManager::init(OP_ParameterManager* manager)
{
	this->manager = manager;
}

void ParManager::update(OP_Inputs* inputs)
{
	for (auto p : parameters)
	{
		p->update(inputs);
	}
}

void ParManager::pulsePressed(const char* name)
{
	for (auto p : pulse_parameters)
	{
		if (p->getName() == name && p->callback)
		{
			p->callback();
		}
	}
}
