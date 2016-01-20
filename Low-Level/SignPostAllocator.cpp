//=================================================================================
// SignPostAllocator.cpp
// Author: Tyler George
// Date  : November 12, 2015
//=================================================================================

////===========================================================================================
///===========================================================================================
//  Includes
///===========================================================================================
////===========================================================================================
#include <malloc.h>
#include "Engine/Low-Level/SignPostAllocator.hpp"
#include "Engine/Utilities/Error.hpp"

////===========================================================================================
///===========================================================================================
// SignPostAllocator Class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
//  Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
SignPostAllocator::SignPostAllocator()
    : m_buffer( nullptr )
    , m_firstPost( nullptr )
{
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
SignPostAllocator::~SignPostAllocator()
{

}


////===========================================================================================
///===========================================================================================
// Startup/Shutdown
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SignPostAllocator::Startup( size_t initialBufferSize )
{
    m_bufferSize = initialBufferSize;

    // Initialize buffer
    m_buffer = malloc( initialBufferSize );

    size_t signPostSize = sizeof( SignPost );
    RECOVERABLE_ASSERT( initialBufferSize > signPostSize );

    // Initialize first sign post
    SignPost* post = (SignPost*)m_buffer;
    post->size = initialBufferSize - sizeof( SignPost ); // account for the size of the first sign post
    post->prev = nullptr;
    post->next = nullptr;
    post->isFree = true;

    m_firstPost = post;
    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SignPostAllocator::Shutdown()
{
    // If this fails, the user failed to DeAlloc all the memory they Alloc-ed
//     RECOVERABLE_ASSERT( ( m_firstPost->next == nullptr ) 
//                && ( m_firstPost->prev == nullptr ) 
//                && ( m_firstPost->isFree ) );

    free( m_buffer );
}


////===========================================================================================
///===========================================================================================
// Accessors/Queries
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* SignPostAllocator::Alloc( size_t size )
{
    size = size + (4 - 1) &~ (4 - 1); // TODO: macro or utility this <--

    // walk the list of SignPosts and find the best fitting free space, split, and return
    size_t realSize = (size + sizeof( SignPost )); 
    SignPost* postIter = m_firstPost;

    SignPost* bestFit = nullptr;
    while (postIter != nullptr)
    {
        if (postIter->isFree && postIter->size >= realSize)
        {
//             if (bestFit)
//             {
//                 if (postIter->size < bestFit->size)
//                     bestFit = postIter;
//             }
//             else
                bestFit = postIter;
                break;
        }
        postIter = postIter->next;
    }

    // cannot find a space large enough for requested allocation
    RECOVERABLE_ASSERT( bestFit != nullptr );

    Byte* postBuffer = (Byte*)bestFit + sizeof( SignPost );
    SignPost* nextSignPost = (SignPost*)(postBuffer + size);
    nextSignPost->isFree = true;
    nextSignPost->prev = bestFit;
    nextSignPost->next = bestFit->next;
    nextSignPost->size = bestFit->size - realSize;

    if (bestFit->next)
        bestFit->next->prev = nextSignPost;

    bestFit->size = size;
    bestFit->next = nextSignPost;
    bestFit->isFree = false;

    return postBuffer;    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SignPostAllocator::DeAlloc( void* ptr )
{
    Byte* buffer = (Byte*)ptr;
    SignPost* post = (SignPost*)(buffer - sizeof( SignPost ));

    RECOVERABLE_ASSERT( !post->isFree );

    // check previous and next post, see if they're free, and join free space
    
    // both are free, join both
    if ((post->prev) && (post->prev->isFree) && (post->next) && (post->next->isFree))
    {
        size_t size = post->prev->size + post->size + post->next->size
            + 2 * sizeof( SignPost ); // 2, this post and the next post must be freed, previous will be modified
        
        SignPost* prev = post->prev;
        prev->size = size;
        prev->next = post->next->next;

        if ( prev->next )
            prev->next->prev = prev;
    }

    // only previous is free
    else if ((post->prev) && (post->prev->isFree))
    {
        size_t size = post->prev->size + post->size + sizeof( SignPost );

        SignPost* prev = post->prev;
        prev->size = size;
        prev->next = post->next;
        
        if (prev->next )
            prev->next->prev = prev;
    }

    // only next is free
    else if ((post->next) && (post->next->isFree))
    {
        size_t size = post->size + post->next->size + sizeof( SignPost );

        post->size = size;
        post->next = post->next->next;
        post->isFree = true;

        if ( post->next )
            post->next->prev = post;
    }

    // neither is free
    else
    {
        post->isFree = true;
    }

    return;
}
