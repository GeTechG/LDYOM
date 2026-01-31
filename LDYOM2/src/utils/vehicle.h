
inline int GetCarNodeIndexFromDoor(unsigned doorId) {
	int result; // eax

	result = -1;
	if (doorId <= 5) {
		switch (doorId) {
			case 0u: result = 16; break;
			case 1u: result = 17; break;
			case 2u: result = 10; break;
			case 3u: result = 8; break;
			case 4u: result = 11; break;
			case 5u: result = 9; break;
			default: return result;
		}
	}
	return result;
}