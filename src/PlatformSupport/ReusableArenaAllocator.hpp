/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#if !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
#define REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680



#include <algorithm>
#include <vector>



#include "ReusableArenaBlock.hpp"
#include "ArenaAllocator.hpp"



template<class ObjectType,
		 class DestroyFunctionType = ArenaBlockDestroy<ObjectType>,
#if defined(XALAN_NO_NAMESPACES)
		 class AllocatorType = allocator<ObjectType>,
#else
		 class AllocatorType = std::allocator<ObjectType>,
#endif
		 class ReusableArenaBlockType = ReusableArenaBlock<ObjectType,
														   DestroyFunctionType,
														   AllocatorType> >
class ReusableArenaAllocator : public ArenaAllocator<ObjectType,
													 DestroyFunctionType,
													 AllocatorType,
													 ReusableArenaBlockType>
{
public:

	typedef ReusableArenaBlockType::size_type			size_type;

	typedef ArenaAllocator<ObjectType,
						   DestroyFunctionType,
						   AllocatorType,
						   ReusableArenaBlockType>		BaseClassType;

	/*
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	ReusableArenaAllocator(size_type	theBlockSize) :
		BaseClassType(theBlockSize),
		m_lastBlockReferenced(0)
	{
	}

	~ReusableArenaAllocator()
	{
	}

	/*
	 * Destroy the object, and free the block for re-use.
	 *
	 * @param theObject the address of the object.
	 */
	void
	destroyObject(ObjectType*	theObject)
	{
		assert(m_blocks.size() != 0);

		// Check this, just in case...
		if (m_lastBlockReferenced != 0 && m_lastBlockReferenced->ownsObject(theObject) == true)
		{
			m_lastBlockReferenced->destroyObject(theObject);
		}
		else
		{
			// Search for the block that owns the object...
			const ArenaBlockListType::reverse_iterator	theEnd = m_blocks.rend();

			ArenaBlockListType::reverse_iterator	i = m_blocks.rbegin();

			while(i != theEnd)
			{
				if ((*i)->ownsObject(theObject) == true)
				{
					m_lastBlockReferenced = *i;

					m_lastBlockReferenced->destroyObject(theObject);

					break;
				}
				else
				{
					++i;
				}
			}
		}
	}

	/*
	 * Allocate a block of the appropriate size for an
	 * object.  Call commitAllocation() when after
	 * the object is successfully constructed.  You _must_
	 * commit an allocation before performing any other
	 * operation on the allocator.
	 *
	 * @return A pointer to a block of memory
	 */
	virtual ObjectType*
	allocateBlock()
	{
		if (m_lastBlockReferenced == 0 ||
			m_lastBlockReferenced->blockAvailable() == false)
		{
			// Search back for a block with some space available...
			ArenaBlockListType::reverse_iterator	i = m_blocks.rbegin();
			const ArenaBlockListType::reverse_iterator	theEnd = m_blocks.rend();

			while(i != theEnd)
			{
				assert(*i != 0);

				if (*i != m_lastBlockReferenced && (*i)->blockAvailable() == true)
				{
					// Ahh, found one with free space.
					m_lastBlockReferenced = *i;

					break;
				}
				else
				{
					++i;
				}
			}

			if (i == theEnd)
			{
				// No blocks have free space available, so create a new block, and
				// push it on the list.
				m_lastBlockReferenced = new ReusableArenaBlockType(m_blockSize);

				m_blocks.push_back(m_lastBlockReferenced);
			}
		}
		assert(m_lastBlockReferenced != 0 && m_lastBlockReferenced->blockAvailable() == true);

		return m_lastBlockReferenced->allocateBlock();
	}

	/*
	 * Commits the allocation of the previous
	 * allocateBlock() call.
	 *
	 * @param theObject A pointer to a block of memory
	 */
	virtual void
	commitAllocation(ObjectType*	theObject)
	{
		assert(m_blocks.size() != 0 && m_lastBlockReferenced != 0 && m_lastBlockReferenced->ownsBlock(theObject) == true);

		m_lastBlockReferenced->commitAllocation(theObject);
		assert(m_lastBlockReferenced->ownsObject(theObject) == true);
	}

private:

	ReusableArenaBlockType*		m_lastBlockReferenced;
};



#endif	// !defined(REUSABLEARENAALLOCATOR_INCLUDE_GUARD_1357924680)
