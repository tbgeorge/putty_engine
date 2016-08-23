class BitArray
{

public:
	BitArray( void *buffer, size_t len, bool initialValue )
	{
		m_buffer = (byte_t*) buffer;
		m_bufferLen = len;
		reset( initialValue );
	}

	void Reset( bool value )
	{
		if( value )
		{
			memset( m_buffer, 0xff, m_bufferLen );
		}
		else
		{
			memset( m_buffer, 0x00, m_bufferLen );
		}
	}

	void Set( uint_t idx )
	{
		uint_t const byte_idx = idx / 8;
		uinn_t const bit_idx = idx % 8;
		ASSERT( byte_idx < m_bufferLen );

		byte_t b = m_buffer[byte_idx];
		b = b | ( 1 << bit_idx );
		m_buffer[byte_idx] = b;
	}

	void Unset( uint_t idx )
	{
		uint_t const byte_idx = idx / 8;
		uinn_t const bit_idx = idx % 8;
		ASSERT( byte_idx < m_bufferLen );

		byte_t b = m_buffer[byte_idx];
		b = b & ~(1 << bit_idx );
		m_buffer[byte_idx] = b;
	}

	bool Get( uint_t idx )
	{
		uint_t const byte_idx = idx / 8;
		uinn_t const bit_idx = idx % 8;
		ASSERT( byte_idx < m_bufferLen );

		byte_t const b = m_buffer[byte_idx];
		return ( b & ( 1 << bit_idx) ) != 0;
	}

private:
	byte_t *m_buffer;
	size_t m_bufferLen;
};

template <size_t BIT_SIZE>
class TBitArray : public BitArray
{
public:
	TBitArray( bool initialValue ) 
		: BitArray( m_fixedBuffer, (BIT_SIZE + 7)/8, initialValue ) {}

	TBitArray() 
		: BitArray( m_fixedBuffer, (BIT_SIZE + 7)/8, false ) {}

private:
	byte_t m_fixedBuffer[ (BIT_SIZE + 7) / 8 ]
};

class Foo()
{
public:
	TBitArray<16000> used_reliables;
};