///////////////////////////////////////////////////////
class CJob
{

	virtual void run() = 0;
};

/////////////////////////////////////////////////////////
class CJobThread : public CThread
{ 
	CJobManager* manager;

	virtual void run()
	{
		while( manager->is_running() )
		{
			process_all_jobs();
			yeild();
		}

		process_all_jobs();
	}

};

///////////////////////////////////////////////////
enum ePriority
{
	LOW = 0,
	MEDIUM = 1,
	HIGH = 2,
	ePriority_COUNT
};

////////////////////////////////////////////////////
class CJobGenerateFiles : public CJob
{
	int num_files;
	size_t file_size;

	CJobGenerateFiles( int num_files, int file_size ) { ... }

	virtual void run()
	{
		GenerateFiles( num_files, file_size );
	}
};

//////////////////////////////////////////////////////
class CJobManager
{
	bool isRunning;

	ThreadSafeQueue< CJob* > queued_jobs[ ePriority_COUNT ];
	ThreadSafeQueue< CJob* > finished_jobs;

	std::vector< CJobThread* > job_threads;

	void init( int num_threads )
	{
		isRunning = true;
		add_threads( num_threads );
	}

	void add_threads( int num_threads )
	{
		for( int i = 0; i < num_threads; ++i )
			create_job_thread();
	}

	void create_job_thread()
	{
		CJobThread* job_thread = new CJobThread(this);
		job_thread.start("jobthread");

		job_threads.push_back(job_thread);
	}

	void shutdown()
	{
		isRunning = false;

		for( int i = 0; i < job_threads.size(); ++i )
		{
			CThread* thread = job_threads[i];
			thread.join();
			delete thread;
		}

		update();

	}

	void update( unsigned int max_time )
	{
		//TODO: add timer

		if( job_threads.size() == 0 )
		{
			process_job();
		}

		CTimer timer;
		timer.start()
		while( timer.elapsed_ms() < max_time )
		{
			CJob* completed_job;
			while( finished_jobs.dequeue(&completed_job)) 
			{
				if( completed_job->on_complete != nullptr )
				{
					completed_job->on_complete( completed_job->on_complete_arg );
				}
				delete completed_job;
			}
		}
	}

	void add_job( CJob* job, ePriority priority, job_complete_cb cb, void* arg )
	{
		job->on_complete = cb;
		job->on_complete_arg = arg;

		queued_jobs[priority].enqueue(job);
	}

	// returns true if a job was processed
	bool process_job()
	{
		CJob* job;

		for( ePriority p = HIGH; p >= LOW; --p )
		{
			if( queue_jobs.dequeue(&job))
			{
				job->run();

				if( jop->on_complete == nullptr )
					delete job;
				else
					finished_jobs.enqueue(job);

				return true;
			}
		}

		return false;
	}
};

/////////////////////////////////////////////////
class CThread
{
	virtual void run() = 0;

	void start( char const *name );
	void detach();
	void join();

	void sleep( unsigned int ms ); // sleeps the thread for however many ms
	void yeild(); // forces a context switch
};

////////////////////////////////////////////////
class CTestThread : public CThread
{
	vitrual void run()
	{
		//do thread work
		sleep( 1000 );
	}
};

////////////////////////////////////////////
void JoinExample()
{
	CTestThread thread;
	thread.start( "test" );

	thread.join(); // will pause the calling thread until this thread is done and cleans up the threads resources
	thread.detach(); // will NOT pause the calling thread but will allow the thread to finish and cleans up the threads resources
}

/////////////////////////////////////////
