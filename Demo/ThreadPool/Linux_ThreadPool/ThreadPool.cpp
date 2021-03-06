#include "ThreadPool.h"



ThreadPool::ThreadPool(void)
{
	m_FlagQuit = false;
	m_lCreateThreadNum = 0;
	m_lRunThreadNum = 0;
	m_lMaxThreadNum = 0;
}


ThreadPool::~ThreadPool(void)
{
}

bool ThreadPool::CreateThreadPool(long lMinThreadNum,long lMaxThreadNum)
{
	if(lMinThreadNum < 0 || lMaxThreadNum < lMinThreadNum)
	{
		return false;
	}

	m_FlagQuit = true;
	int res = sem_init(&m_sem, 0, lMaxThreadNum);
	if(res == -1)
	{
		perror("semaphore intitialization failed\n");
		exit(EXIT_FAILURE);
	}

	for(int i=0; i<lMinThreadNum; i++)
	{
		pthread_t  thread_id;
		int ret = pthread_create(&thread_id, NULL, &ThreadProc, (void*)this);
		if(ret)
		{
			m_lstHandle.push_back(thread_id);
		}
	}
	m_lMaxThreadNum = lMaxThreadNum;
	m_lCreateThreadNum = lMinThreadNum;
	return true;
}

void ThreadPool::DestoryThreadPool()
{
	m_FlagQuit = false;
	std::list<pthread_t>::iterator ite = m_lstHandle.begin();
	while(ite != m_lstHandle.end())
	{
		pthread_join(*ite,NULL);
		ite++;
	}
	m_lstHandle.clear();
	sem_destroy(&m_sem);

}

void * ThreadPool::ThreadProc (void *lpvoid)
{
	ThreadPool *pthis = (ThreadPool *)lpvoid;
	Itask *pItask = NULL;
	while(pthis->m_FlagQuit)
	{
<<<<<<< HEAD
		//���ź�
		sem_wait(&pthis->sem);
		//InterlockedIncrement(&pthis->m_lRunThreadNum);
=======

		sem_wait(&pthis->m_sem);
>>>>>>> 2cf76f66ffe38110321982a09f7d40a6cbb2342c

		pthis->m_NumLock.Lock();
		pthis->m_lRunThreadNum++;
		pthis->m_NumLock.UnLock();
<<<<<<< HEAD



=======
>>>>>>> 2cf76f66ffe38110321982a09f7d40a6cbb2342c
		while(!pthis->m_qItask.empty())
		{
			pthis->m_MyLock.Lock();
			pItask = pthis->m_qItask.front();
			pthis->m_qItask.pop();
			pthis->m_MyLock.UnLock();
			pItask->RunItask();
		}

<<<<<<< HEAD
		//InterlockedDecrement(&pthis->m_lRunThreadNum);

=======

		pthis->m_NumLock.Lock();
>>>>>>> 2cf76f66ffe38110321982a09f7d40a6cbb2342c
		pthis->m_lRunThreadNum--;
		pthis->m_NumLock.Lock();

	}
	return 0;
}

bool ThreadPool::PushItask(Itask *pItask)
{
	if(NULL == pItask)
	{
		return false;
	}
	m_MyLock.Lock();
	m_qItask.push(pItask);
	m_MyLock.UnLock();
	if(m_lRunThreadNum < m_lCreateThreadNum)
	{
		sem_post(&m_sem);
	}
	else if(m_lCreateThreadNum < m_lMaxThreadNum)
	{
		//HANDLE handle = (HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,NULL);
		pthread_t  thread_id;
		int ret = pthread_create(&thread_id, NULL, &ThreadProc, (void*)this);
		if(ret)
		{
			m_lstHandle.push_back(thread_id);
		}
		m_lCreateThreadNum++;
		sem_post(&m_sem);
	}
	else
	{
		return false;
	}

	return true;
}
