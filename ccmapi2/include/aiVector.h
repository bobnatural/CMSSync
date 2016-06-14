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
#include "aiDllUtil.h"
#include <stdexcept>

#ifdef AIINTERFACES_EXPORTS
#define AIVECTOR_API __declspec(dllexport)
#define EXPVECTOR_TEMPLATE
#else
#define AIVECTOR_API __declspec(dllimport)
#define EXPVECTOR_TEMPLATE extern
#endif

#define DEFAULT_VECTOR_CAPACITY 5

namespace AI
{
	/**
 	 * A resizable container class used to hold objects of a specific class type.
	 */
	template<class T>
	class Vector : public DLL_OBJECT_BASE
	{
	public:
		// constructors
		/**
		 * Default Constructor
		 */
		Vector();

		/**
		 * Copy Constructor.
		 * @param src Source vector.
		 */
		Vector(const Vector<T> &src);

		/**
		 * Destructor.
		 */
		~Vector();

		// mutators
		/**
		 * Append element to end of vector
		 * @param obj Source element.
		 */
		virtual void push_back(const T& obj);

		/**
		 * Append element to end of vector
		 * @param obj Pointer to source element.
		 */
		virtual void push_back(const T* obj);

		/**
		 * Append all elements to end of vector
		 * @param src Source vector.
		 */
		virtual void push_back(const Vector<T> &src);

		/**
		 * Remove last element in vector
		 */
		virtual void pop_back();

		/**
		 * Erases all elements in the vector
		 */
		virtual void clear();

		// operators
		/**
		 * Assignment Operator.
		 * Assigns a copy of vector src as the new content for the vector object.
		 * @param src Source vector.
		 * @return Resulting vector.
		 */
		virtual Vector<T>& operator=(const Vector<T>& src);

		/**
		 * Append Operator.
		 * Appends a copy of vector src as new content for the vector object
		 * @param src Source vector.
		 * @return Resulting vector.
		 */
		virtual Vector<T>& operator+=(const Vector<T>& src);

		/**
		 * Provides a reference to the element with a specified index
		 * @param pos Position index of desired item.
		 * @return Reference to element
		 * <p><b>Note: </b><i>Returned reference is internal location and must not be released</i></p>
		 */
		virtual T& operator[](unsigned int pos) const	{ return at(pos);	}

		// accessors
		/**
		 * Return first element.
		 * @return Element at front of vector.
		 * <p><b>Note: </b><i>Returned reference is internal location and must not be released</i></p>
		 */
		virtual T& front() const				{ return at(0);			}

		/**
		 * Return last element.
		 * @return Element at back of vector.
		 * <p><b>Note: </b><i>Returned reference is internal location and must not be released</i></p>
		 */
		virtual T& back() const					{ return at(m_size-1);	}

		/**
		 * Returns the size of the internal allocated storage space.
		 * @return Size of current capacity.
		 * <p><b>Note: </b><i>Capacity does not suppose a limit to the length of the vector. If more space is required to accomodate content, the capacity is automatically expanded.</i></p>
		 */
		virtual unsigned int capacity() const	{ return m_capacity;	}

		/**
		 * Return size of vector.
		 * @return Count of the number of elements in the vector.
		 */
		virtual unsigned int size() const		{ return m_size;		}

		/**
		 * Tests contents of vector whether if empty (size 0).
		 * @return true if the vector is empty, otherwise false.
		 */
		virtual bool empty() const				{ return m_size==0;		}

		/**
		 * Provides a reference to the element with a specified index
		 * @param pos Position index of desired character.
		 * @return Reference to element
		 * <p><b>Note: </b><i>Returned reference is internal location and must not be released</i></p>
		 */
		virtual T& at(unsigned int pos) const;

	protected:
		void init(unsigned int size=DEFAULT_VECTOR_CAPACITY);
		void increaseCapacity(unsigned int size=DEFAULT_VECTOR_CAPACITY);

	private:
		T**				m_pData;
		unsigned int	m_size;
		unsigned int	m_capacity;
	};

	/**
	 * Default Constructor
	 */
	template <class T> 
	Vector<T>::Vector()
	{
		init(DEFAULT_VECTOR_CAPACITY);
	}

	/**
	 * Copy Constructor.
	 * @param src source vector.
	 */
	template <class T> 
	Vector<T>::Vector(const Vector<T> &src)
	{
		init(src.m_capacity+DEFAULT_VECTOR_CAPACITY);
		push_back(src);
	}

	/**
	 * Assignment Operator
	 * Assigns a copy of vector src as the new content for the vector object.
	 * @param src Source vector.
	 */
	template <class T> 
	Vector<T>& Vector<T>::operator=(const Vector<T>& src)
	{
		if(this!=&src)
		{
			clear();
			init(src.m_capacity+DEFAULT_VECTOR_CAPACITY);
			push_back(src);
		}

		return *this;
	}

	/**
	 * Append Operator
	 * Appends a copy of vector src as new content for the vector object
	 * @param src Source vector.
	 */
	template <class T> 
	Vector<T>& Vector<T>::operator+=(const Vector<T>& src)
	{
		push_back(src);
		return *this;
	}

	/**
	 * Append all elements to end of vector
	 * @param src source vector.
	 */
	template <class T>
	void Vector<T>::push_back(const Vector<T> &src)
	{
		if((m_capacity-m_size)<src.m_size)
			increaseCapacity(src.m_size-m_capacity-m_size);

		for(unsigned int ix=0; ix<src.m_size; ix++)
			push_back(src[ix]);
	}

	/**
	 * Append element to end of vector
	 * @param obj Pointer to source element.
	 */
	template <class T>
	void Vector<T>::push_back(const T* obj)
	{
		push_back(*obj);
	}

	/**
	 * Append element to end of vector
	 * @param obj Source element.
	 */
	template <class T>
	void Vector<T>::push_back(const T& obj)
	{
		// check array size
		if(m_capacity<m_size+1)
			increaseCapacity();

		m_pData[m_size]=new T(obj);
		m_size++;
	}

	/**
	 * Remove last element in vector
	 */
	template <class T>
	void Vector<T>::pop_back()
	{
		if(m_size>0)
		{
			m_size--;
			delete m_pData[m_size];
		}
	}

	/**
	 * Provides a reference to the element with a specified index
	 * @param pos Position index of desired character.
	 * @return Reference to element
	 * <p><b>Note: </b><i>Returned reference is internal location and must not be released</i></p>
	 */
	template <class T>
	T& Vector<T>::at(unsigned int pos) const
	{
		// test for invalid position
		if(pos<0 || pos>=m_size || m_size==0)
			throw std::out_of_range("Position exceeds range of vector elements");

		return *m_pData[pos];
	}

	/**
	 * @internal
	 * Initialization
	 */
	template <class T>
	void Vector<T>::init(unsigned int size)
	{
		m_pData = NULL;
		m_capacity = 0;
		m_size = 0;
		increaseCapacity(size);
	}

	/**
	 * @internal
	 * Increase internal list capacity
	 */
	template <class T>
	void Vector<T>::increaseCapacity(unsigned int size)
	{
		// create new data array
		m_capacity += size;
		T** tmpData = new T*[m_capacity];
		//memset(tmpData,0,sizeof(T)*m_capacity);

		// copy existing data
		for(unsigned int ix=0; ix<m_size; ix++)
			tmpData[ix] = m_pData[ix];

		// replace data
		if(m_pData!=NULL) delete [] m_pData;
		m_pData = tmpData;
	}

	/**
	 * Erases all elements in the vector
	 */
	template <class T>
	void Vector<T>::clear()
	{
		if(m_pData!=NULL)
		{
			for(unsigned int ix=0; ix<m_size; ix++)
				delete m_pData[ix];
			delete [] m_pData;
		}
		init();
	}

	/**
	 * Destructor
	 */
	template <class T> 
	Vector<T>::~Vector()
	{
		if(m_pData!=NULL)
		{
			for(unsigned int ix=0; ix<m_size; ix++)
				delete m_pData[ix];
			delete [] m_pData;
		}
	}

} // namespace AI
namespace ActivIdentity = AI;