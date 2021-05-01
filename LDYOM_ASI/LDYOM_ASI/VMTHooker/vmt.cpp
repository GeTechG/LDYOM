#include "vmt.h"

VMTHookManager::VMTHookManager( void** vTable ) : m_vTable( vTable )
{
	m_numberFuncs = GetNumberOfFunctions( ); //getting the number of virtual functions
	m_originalFuncs = new void* [ m_numberFuncs ]; // allocating memory so we can save here the addresses of the original functions
	for( unsigned short i = 0; i < m_numberFuncs; i++ ) //loop to fill our array with the original addresses
	{
		m_originalFuncs[ i ] = GetFunctionAddyByIndex( i ); // saving the address of the original functions
	}
}

VMTHookManager::~VMTHookManager( )
{
	UnhookAll( ); //unhooking all the functions
	delete[ ] m_originalFuncs; //we need to free the allocated memory
}

void* VMTHookManager::GetFunctionAddyByIndex( unsigned short index )
{
	if( index < m_numberFuncs ) //check if the index is in the range of the vtable
	{
		return m_vTable[ index ]; //return the address
	}
	return nullptr; //if not, return nullptr
}

void* VMTHookManager::Hook( unsigned short index , void* ourFunction )
{
	uintptr_t bufferOriginalFunc = NULL; //buffer to store the address
	if( !toHook( index , true , ourFunction , &bufferOriginalFunc ) ) //checking if it failed
	{
		return nullptr; //didn't work, return nullptr
	}
	return reinterpret_cast< void* >( bufferOriginalFunc ); //worked, return the original address
}

bool VMTHookManager::Unhook( unsigned short index )
{
	if( !toHook( index ) )// checking if it failed
	{
		return false; // return false, didn't work
	}
	return true; // return true, worked
}

void VMTHookManager::UnhookAll( )
{
	for( int index = 0; index < m_numberFuncs; index++ ) //loop to unhook all the functions
	{
		if( m_vTable[ index ] == m_originalFuncs[ index ] ) //checking if the function isn't hooked
		{
			continue; //if not hooked skip this index
		}
		Unhook( index ); //unhook
	}
}

unsigned short VMTHookManager::GetNumberOfFunctions( ) //private function
{
	unsigned short index = 0; //index
	MEMORY_BASIC_INFORMATION mbiBuffer = { 0 }; //buffer to store the information of some memory region
	while( true )
	{
		if( !m_vTable[ index ] ) //if vtable[index] is null
		{
			break; //exit loop
		}
		if( !VirtualQuery( m_vTable[ index ] , &mbiBuffer , sizeof( mbiBuffer ) ) ) //if VirtualQuery failed
		{
			break; //exit loop
		}
#define CAN_EXECUTE (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)
		if( ( mbiBuffer.State != MEM_COMMIT ) || ( mbiBuffer.Protect & ( PAGE_GUARD | PAGE_NOACCESS ) ) || !( mbiBuffer.Protect & CAN_EXECUTE ) ) //if the region isn't commited and/or executable
		{
			break; //exit loop
		}
		++index;
	}
	return index;
}

bool VMTHookManager::toHook( unsigned short index , bool hook , void* ourFunction , uintptr_t* bufferOriginalFunc )
{
	DWORD OldProtection = NULL; //it will save the protection here
	if( index < m_numberFuncs )
	{
		VirtualProtect( m_vTable + index , 0x4 , PAGE_EXECUTE_READWRITE , &OldProtection );//changing the protection
		if( hook )
		{
			if( !ourFunction || !bufferOriginalFunc )
			{
				VirtualProtect( m_vTable + index , 0x4 , OldProtection , &OldProtection ); //restore original protection and return
				return false;
			}
			*bufferOriginalFunc = ( uintptr_t ) m_vTable[ index ]; // saving the original address in our buffer so we can call the function
			m_vTable[ index ] = ourFunction; //change the address to our function
			VirtualProtect( m_vTable + index , 0x4 , OldProtection , &OldProtection ); //restore protection
			return true;
		}
		else
		{
			m_vTable[ index ] = m_originalFuncs[ index ]; //copy the original address to the vtable[index]
			VirtualProtect( m_vTable + index , 0x4 , OldProtection , &OldProtection ); // restore protection
			return true;
		}
	}
	return false;
}