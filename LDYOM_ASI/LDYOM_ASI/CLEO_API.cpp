
struct Mission;

extern Mission* currentMissionPtr;

extern "C" __declspec(dllexport) Mission* getCurrentMissionPtr()
{
	return currentMissionPtr;
}
