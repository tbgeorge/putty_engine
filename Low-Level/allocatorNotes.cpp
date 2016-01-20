struct sign_post
{
	sign_post *prev, *next;
	bool free;

	// size of buffer, not including sign_post
	size_t size;
};


class SignPostAllocator
{
	void* buffer;
	sign_post* posts;

	void init( size_t initial_size )
	{
		buffer = malloc( initial_size );

		sign_post *post = (sign_post*) buffer;
		post->size = initial_size - sizeof( sign_post );
		post->next = nullptr;
		post->prev = nullptr;
		post->free = true;

		posts = post;
	}

	void deinit()
	{
		FATAL_ASSERT( (posts->next == nullptr )
			&& (posts->prev == nullptr )
			&& (posts->free) );

		free(buffer);
	}

	void* alloc( size_t size )
	{
		// walk list and find first free, split, and return
		size_t alloc_size = size + sizeof( sign_post );
		sign_post *iter = *posts;

		while( iter != nullptr )
		{
			if( iter->free && iter->size >= alloc_size )
			{
				break;
			}

			iter = iter->next;
		}

		if( iter == nullptr )
			return; // no free memory


		char* iter_buffer = (char*)iter + sizeof(sign_post);
		sign_post* next_signPost = (sign_post*)(iter_buffer + size );
		next_signPost->free = true;
		next_signPost->prev = iter;
		next_signPost->next = iter->next;
		next_signPost->size = iter->size - alloc_size;

		iter->size = size;
		iter->next = next_signPost;
		iter->free = false;

		return iter_buffer;
	}

	void free( void* ptr )
	{
		char* iter_buffer = (char*)ptr;

		sign_post* post = (sign_post*) (iter_buffer - sizeof(sign_post) );

		FATAL_ASSERT( post->free == false );

		// check previous or next, see if they're free, or both are free
		if( (post->prev != nullptr) && (post->prev->free) 
			&& (post->next) && (post->next->free ) )
		{
			size_t size = post->prev->size + post->size + post->next->size 
				+ 2 * sizeof( sign_post ); // 2, this post and next post, previous is staying

				sign_post *prev = post->prev;
				prev->size = size;
				prev->next = post->next->next;
		}
		else if( (post->prev != nullptr ) && (post->prev->free ) )
		{
			size_t size = post->prev->size + post->size + sizeof( sign_post );

			sign_post* prev = post->prev;
			prev->size = size;
			prev->next = post->next;
		}
		else if( ( post->next != nullptr ) && (post->next->free ) )
		{
			size_t size = post->size + post->next->size + sizeof( sign_post );

			post->size = size;
			post->next = post->next->next;
			post->free = true;
		}
		else
		{
			post->free = true;
		}
	}
};

