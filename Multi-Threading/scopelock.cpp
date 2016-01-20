class ScopeLock
{
	CriticalSection* m_cs;

public:
	ScopeLock( CriticalSection* cs )
	{
		m_cs = cs;
		cs->Enter();
	}
	~ScopeLock()
	{
		cs->Exit();
	}

};


#define CS_SCOPE( cs ) ScopeLock cs_##__LINE__##(&cs)


CriticalSection my_lock;

void foo()
{
	CS_SCOPE( my_lock );
}