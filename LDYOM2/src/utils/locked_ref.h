#pragma once
#include <mutex>

template <typename T, typename MutexType> class LockedRef {
  private:
	T& m_ref;
	std::unique_lock<MutexType> m_lock;

  public:
	LockedRef(T& ref, MutexType& mutex)
		: m_ref(ref),
		  m_lock(mutex) {}

	T* operator->() { return &m_ref; }

	T& operator*() { return m_ref; }

	operator T&() { return m_ref; }
};