/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
// Class header file.
#include "AttributeListImpl.hpp"



#include <algorithm>
#include <cassert>



#include "DOMStringHelper.hpp"
#include "XalanAutoPtr.hpp"



AttributeListImpl::AttributeListImpl() :
	AttributeList(),
	m_AttributeKeyMap(),
	m_AttributeVector()
{
		m_AttributeVector.reserve(eDefaultVectorSize);
}



AttributeListImpl::~AttributeListImpl()
{
	// Clean up everything...
	clear();
}



AttributeListImpl::AttributeListImpl(const AttributeListImpl&	theSource) :
	AttributeList(),
	m_AttributeKeyMap(),
	m_AttributeVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());
}



AttributeListImpl::AttributeListImpl(const AttributeList&	theSource) :
	AttributeList(),
	m_AttributeKeyMap(),
	m_AttributeVector()
{
	// Use the assignment operator to do the dirty work...
	*this = theSource;

	assert(getLength() == theSource.getLength());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());
}



void
AttributeListImpl::deleteEntries(AttributeVectorType&	theVector)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Delete all of the objects in the temp vector.
	for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<AttributeVectorEntry>());
}



AttributeListImpl&
AttributeListImpl::operator=(const AttributeListImpl&	theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		// Some temporary structures to hold everything
		// until we're done.
		AttributeKeyMapType		tempMap;
		AttributeVectorType		tempVector;

		const unsigned int	theLength = theRHS.getLength();

		// Reserve the appropriate capacity right now...
		tempVector.reserve(theLength);

		try
		{
			// Copy the vector entries, and build the index map...
			for(unsigned int i = 0; i < theLength; i++)
			{
				assert(theRHS.m_AttributeVector[i] != 0);

				XalanAutoPtr<AttributeVectorEntry>	theEntry(
					new AttributeVectorEntry(*theRHS.m_AttributeVector[i]));

				// Add the item...
				tempVector.push_back(theEntry.get());

				// The entry is now safely in the vector, so release the
				// XalanAutoPtr...
				AttributeVectorEntry* const		entry = theEntry.release();

				// Create an entry in the index map...
				tempMap.insert(AttributeKeyMapType::value_type(entry->m_Name.begin(),
															   entry));
			}
		}
		catch(...)
		{
			deleteEntries(tempVector);

			throw;
		}

		// OK, we're safe, so swap the contents of the
		// containers.  This is guaranteed not to throw.
		m_AttributeKeyMap.swap(tempMap);
		m_AttributeVector.swap(tempVector);

		// This will delete all of the old entries.
		deleteEntries(tempVector);

		assert(getLength() == theLength);
		assert(m_AttributeKeyMap.size() == m_AttributeVector.size());
	}

	return *this;
}



AttributeListImpl&
AttributeListImpl::operator=(const AttributeList&	theRHS)
{
	if (this != &theRHS)
	{
		// Note that we can't chain up to our base class operator=()
		// because it's private.

		const unsigned int	theLength = theRHS.getLength();

		// Some temporary structures to hold everything
		// until we're done.
		AttributeKeyMapType		tempMap;
		AttributeVectorType		tempVector;

		// Keep our old entries, in case something
		// bad happens.
		tempMap.swap(m_AttributeKeyMap);
		tempVector.swap(m_AttributeVector);

		try
		{
			// Reserve the appropriate capacity right now...
			m_AttributeVector.reserve(theLength);

			// Add each attribute.
			for(unsigned int i = 0; i < theLength; i++)
			{
				addAttribute(theRHS.getName(i),
							 theRHS.getType(i),
							 theRHS.getValue(i));
			}
		}
		catch(...)
		{
			// Swap everything back...
			tempMap.swap(m_AttributeKeyMap);
			tempVector.swap(m_AttributeVector);

			// This will delete anything new we've
			// created.
			deleteEntries(tempVector);

			throw;
		}

		assert(getLength() == theLength);
		assert(m_AttributeKeyMap.size() == m_AttributeVector.size());
	}

	return *this;
}



unsigned int
AttributeListImpl::getLength() const
{
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	return m_AttributeVector.size();
}



const XMLCh*
AttributeListImpl::getName(const unsigned int index) const
{
	assert(index < getLength());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	return m_AttributeVector[index]->m_Name.begin();
}



const XMLCh*
AttributeListImpl::getType(const unsigned int index) const
{
	assert(index < getLength());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	return m_AttributeVector[index]->m_Type.begin();
}



const XMLCh*
AttributeListImpl::getValue(const unsigned int index) const
{
	assert(index < getLength());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	return m_AttributeVector[index]->m_Value.begin();
}



const XMLCh*
AttributeListImpl::getType(const XMLCh* const name) const
{
	assert(name != 0);
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	// Find the name in the key map.
	const AttributeKeyMapType::const_iterator	i =
				m_AttributeKeyMap.find(name);

	if (i != m_AttributeKeyMap.end())
	{
		// Found it, so return a pointer to the type.
		return (*i).second->m_Type.begin();
	}
	else
	{
		return 0;
	}
}



const XMLCh*
AttributeListImpl::getValue(const char* const name) const
{
	return getValue(MakeXalanDOMCharVector(name).begin());
}



const XMLCh*
AttributeListImpl::getValue(const XMLCh* const name) const
{
	assert(name != 0);
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	// Find the name in the key map.
	const AttributeKeyMapType::const_iterator	i =
				m_AttributeKeyMap.find(name);

	if (i != m_AttributeKeyMap.end())
	{
		// Found it, so return a pointer to the value.
		return (*i).second->m_Value.begin();
	}
	else
	{
		return 0;
	}
}



void
AttributeListImpl::clear()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	deleteEntries(m_AttributeVector);

	// Clear everything out.
	m_AttributeVector.clear();
	m_AttributeKeyMap.clear();
}



// A convenience function to find the length of a null-terminated
// array of XMLChs
static const XMLCh*
endArray(const XMLCh*	data)
{
	const XMLCh*	theEnd = data;

	while(*theEnd)
	{
		++theEnd;
	}

	return theEnd;
}



bool
AttributeListImpl::addAttribute(
			const XMLCh*	name,
			const XMLCh*	type,
			const XMLCh*	value)
{
	assert(name != 0);
	assert(type != 0);
	assert(value != 0);
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	bool	fResult = false;

	// Update the attribute, if it's already there...
	const AttributeKeyMapType::iterator		i =
		m_AttributeKeyMap.find(name);

	if (i != m_AttributeKeyMap.end())
	{
		// Create the new vectors, then swap them...
		XMLChVectorType		theNewType(type, endArray(type) + 1);
		XMLChVectorType		theNewValue(value, endArray(value) + 1);

		theNewType.swap((*i).second->m_Type);
		theNewValue.swap((*i).second->m_Value); 
	}
	else
	{
		XalanAutoPtr<AttributeVectorEntry>	theEntry(
					new AttributeVectorEntry(XMLChVectorType(name, endArray(name) + 1),
											 XMLChVectorType(value, endArray(value) + 1),
											 XMLChVectorType(type, endArray(type) + 1)));

		// Add the new one.
		m_AttributeVector.push_back(theEntry.get());

		// The entry is now safely in the vector, so release the
		// XalanAutoPtr...
		AttributeVectorEntry* const		entry = theEntry.release();

		// Create an entry in the index map.
		m_AttributeKeyMap.insert(AttributeKeyMapType::value_type(entry->m_Name.begin(), entry));

		fResult = true;
	}

	assert(m_AttributeKeyMap.find(name) != m_AttributeKeyMap.end());
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

	return fResult;
}



bool
AttributeListImpl::removeAttribute(const XMLCh*		name)
{
	assert(name != 0);
	assert(m_AttributeKeyMap.size() == m_AttributeVector.size());

#if !defined(XALAN_NO_NAMESPACES)
	using std::bind1st;
	using std::equal_to;
	using std::find_if;
#endif

	bool	fResult = false;

	// Find it in the key map.
	const AttributeKeyMapType::iterator		i =
				m_AttributeKeyMap.find(name);

	if (i != m_AttributeKeyMap.end())
	{
		// Found it, so now find the entry in the
		// vector.
		const AttributeVectorType::iterator		j =
			find_if(m_AttributeVector.begin(),
					m_AttributeVector.end(),
					bind1st(equal_to<AttributeVectorEntry*>(), (*i).second));
		assert(j != m_AttributeVector.end());

		// This will delete the entry, even if something
		// bad happens updating the containers.
		XalanAutoPtr<AttributeVectorEntry>	theGuard(*j);

		// Erase it from the vector.
		m_AttributeVector.erase(j);

		assert(m_AttributeVector.size() ==
					m_AttributeKeyMap.size() - 1);

		// Erase it from the key map.
		m_AttributeKeyMap.erase(i);

		assert(m_AttributeVector.size() ==
					m_AttributeKeyMap.size());

		fResult = true;
	}

	return fResult;
}
