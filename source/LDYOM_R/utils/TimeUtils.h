#pragma once
#include <CMenuManager.h>
#include <CTimer.h>

#include "ktcoro_wait.hpp"

inline ktwait waitInGame(const unsigned int timeInMs) {
	unsigned elapsed = 0;
	while (elapsed < timeInMs) {
		co_await 1;
		if (!FrontEndMenuManager.m_bMenuActive) {
			elapsed += CTimer::m_snTimeInMilliseconds - CTimer::m_snPreviousTimeInMilliseconds;
		}
	}
}
