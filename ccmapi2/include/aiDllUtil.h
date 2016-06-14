/*
 * (c) Copyright 2006-2008 ActivIdentity
 * All Rights Reserved.
 *
 * This program is an unpublished copyrighted work which is proprietary
 * to ActivIdentity. This computer program includes Confidential,
 * Proprietary Information and is a Trade Secret of ActivIdentity.
 * Any use, disclosure, modification and/or reproduction is prohibited
 * unless authorized in writing by ActivIdentity.
 *
 * WARNING:  Unauthorized reproduction of this program as well as
 * unauthorized preparation of derivative works based upon the
 * program or distribution of copies by sale, rental, lease or
 * lending are violations of federal copyright laws and state trade
 * secret laws, punishable by civil and criminal penalties.
 * 
 * Created on May 5, 2006
 */
#pragma once
#include <new>

#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

//#ifdef AIINTERFACES_EXPORTS
//#define AIINTERFACES_API __declspec(dllexport)
//#define EXPINTERFACES_TEMPLATE
//#else
//#define AIINTERFACES_API __declspec(dllimport)
//#define EXPINTERFACES_TEMPLATE extern
//#endif

#ifdef WIN32
#define DLL_CALL __stdcall
#else
#define DLL_CALL
#endif

// macros for ease of use
#define DLL_INTERFACE		BaseDLLInterface
#define DLL_OBJECT(x)		BaseDLLObjectImpl<x>
#define DLL_OBJECT_BASE		BaseDLLObjectImpl<BaseDLLInterface>

namespace AI
{

   /* 
	* Interfaces exposed across a DLL boundary should derive from this
	* class to ensure that their memory is released cleanly and
	* safely. This class should be used in conjunction with the DLLImpl
	* template. <b>Interface should NOT define a destructor!</b>
	*
	* Based on technique in article by Chad Austin, http://aegisknight.org/cppinterface.html
	* @see BaseDLLObject 
	* @internal
	*/
	class BaseDLLInterface 
	{
	protected:
	   /* 
		* Handles the destruction of this class. This is essentially a destructor
		* that works across DLL boundaries.
		* @internal
		*/
		virtual void DLL_CALL destroy() = 0;

		public:
	   /*
		* Specialized delete that calls destroy instead of the destructor.
		* @internal
		*/
		void operator delete(void* p) 
		{
			if(p!=NULL) 
			{
				BaseDLLInterface* i = static_cast<BaseDLLInterface*>(p);
				i->destroy();
			}
		}
		void operator delete[](void* p) 
		{
			if (p!=NULL) 
			{
				BaseDLLInterface* i = static_cast<BaseDLLInterface*>(p);
				i->destroy();
			}
		}
#ifdef _DEBUG
		void operator delete(void* p, const char* lpszFileName, int nLine)		// DEBUG_DELETE
		{
			if (p!=NULL) 
			{
				BaseDLLInterface* i = static_cast<BaseDLLInterface*>(p);
				i->destroy();
			}
		}
		void operator delete[](void* p, const char* lpszFileName, int nLine)	// DEBUG_DELETE
		{
			if (p!=NULL) 
			{
				BaseDLLInterface* i = static_cast<BaseDLLInterface*>(p);
				i->destroy();
			}
		}
#endif // _DEBUG
	};

   /*
	* Implementations of interfaces across DLL boundaries that derive
	* from BaseDLLInterface should derive from this template to ensure that
	* their memory is released safely and cleanly.
	* Based on technique in article by Chad Austin, http://aegisknight.org/cppinterface.html
	* @see BaseDLLInterface 
	* @internal
	*/
	template<class I>
	class BaseDLLObjectImpl : public I 
	{
	public:

	   /*
		* This allows the implementation to simply define a destructor.  It
		* will automatically be called at the right time.
		* @internal
		*/
		virtual ~BaseDLLObjectImpl() { }

	   /*
		* Implementation of DLLInterface's destroy method. This function will
		* make sure this object gets deleted in a polymorphic manner. You
		* should NOT reimplement this method.
		* @internal
		*/
		virtual void DLL_CALL destroy() 
		{
			delete this;
		}

	   /*
		* Overloaded operator new calls global operator new since
		* DLLInterface modified this functionality to protect memory.
		* @param count The number of bytes of storage to be allocated  
		* @internal
		*/
		void* operator new(size_t count) 
		{
			return ::operator new(count);
		}
		void* operator new(size_t count, void* ptr) 
		{
			return ::operator new(count, ptr);
		}
		void* operator new[](size_t count) 
		{
			return ::operator new[](count);
		}
		void* operator new(size_t count, const char* lpszFileName, int nLine)	// DEBUG_NEW
		{
#ifdef _DEBUG
			return ::operator new(count, 1, lpszFileName, nLine);
#else
			return ::operator new(count);
#endif // _DEBUG
		}
		void* operator new[](size_t count, const char* lpszFileName, int nLine)	// DEBUG_NEW
		{
#ifdef _DEBUG
			return ::operator new[](count, 1, lpszFileName, nLine);
#else
			return ::operator new[](count);
#endif // _DEBUG
		}

	   /*
		* Overloaded operator delete calls global operator delete since
		* DLLInterface modified this functionality to protect memory.
		* @param p    pointer to the memory to delete
		* @internal
		*/
		void operator delete(void* p) 
		{
			::operator delete(p);
		}
		void operator delete[](void* p) 
		{
			::delete[] (p);
		}
		void operator delete(void* p, const char* lpszFileName, int nLine)		// DEBUG_DELETE
		{
#ifdef _DEBUG
			return ::operator delete(p, 1, lpszFileName, nLine);
#else
			return ::operator delete(p);
#endif // _DEBUG
		}
		void operator delete[](void* p, const char* lpszFileName, int nLine)	// DEBUG_DELETE
		{
#ifdef _DEBUG
			return ::operator delete[](p, 1, lpszFileName, nLine);
#else
			return ::operator delete[](p);
#endif // _DEBUG
		}

	};

} // namespace AI
namespace ActivIdentity = AI;