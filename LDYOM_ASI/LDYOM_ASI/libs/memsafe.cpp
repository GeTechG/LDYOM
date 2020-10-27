#include "memsafe.h"
#include <stdexcept>

void *memsafe::copy(void *dest, const void *src, size_t stLen)
{
	if (dest == nullptr || src == nullptr || stLen == 0)
		return nullptr;

	MEMORY_BASIC_INFORMATION	mbi;
	VirtualQuery(dest, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	void	*pvRetn = memcpy(dest, src, stLen);
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	FlushInstructionCache(GetCurrentProcess(), dest, stLen);
	return pvRetn;
}

void *memsafe::copy(void *dest, std::deque<unsigned char> src)
{
	uint8_t buf[4096];
	int		i = 0;

	for (auto ch : src)
		buf[i++] = ch;

	return copy(dest, buf, src.size());
}

int memsafe::compare(const void *_s1, const void *_s2, uint32_t len)
{
	const uint8_t	*s1 = static_cast<const uint8_t *>(_s1);
	const uint8_t	*s2 = static_cast<const uint8_t *>(_s2);
	uint8_t			buf[4096];

	for (;;)
	{
		if (len > 4096)
		{
			if (!copy(buf, s1, 4096))
				return 0;
			if (memcmp(buf, s2, 4096))
				return 0;
			s1 += 4096;
			s2 += 4096;
			len -= 4096;
		}
		else
		{
			if (!copy(buf, s1, len))
				return 0;
			if (memcmp(buf, s2, len))
				return 0;
			break;
		}
	}

	return 1;
}

int memsafe::compare(const void *_s1, std::deque<unsigned char> _s2)
{
	uint8_t buf[4096];
	int		i = 0;

	for (auto ch : _s2)
		buf[i++] = ch;

	return compare(_s1, buf, _s2.size());
}

int memsafe::set(void *_dest, int c, DWORD len)
{
	byte *dest = static_cast<byte *>(_dest);
	byte buf[4096];
	memset(buf, c, (len > 4096) ? 4096 : len);
	for (;;)
	{
		if (len > 4096)
		{
			if (!copy(dest, buf, 4096))
				return 0;
			dest += 4096;
			len -= 4096;
		}
		else
		{
			if (!copy(dest, buf, len))
				return 0;
			break;
		}
	}
	return 1;
}

void memsafe::Unprotect(DWORD address, size_t size)
{
	do
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(PVOID(address), &mbi, sizeof(mbi)))
			throw std::runtime_error("virtual query error");
		if (size > mbi.RegionSize)
			size -= mbi.RegionSize;
		else
			size = 0;
		DWORD oldp;
		if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldp))
			throw std::runtime_error("virtual protect error");
		if (DWORD(mbi.BaseAddress) + mbi.RegionSize < address + size)
			address = DWORD(mbi.BaseAddress) + mbi.RegionSize;
	} while (size);
}
