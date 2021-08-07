#include<iostream>
#include "pthread.h"

using namespace std;

void *server( void *arg )
{
	cout << "Plant server has been started!" << endl;
    flush(cout);
    
    while (1)
    {
    }
    
    return 0;
}

void *client( void *arg )
{
    cout << "Plant client has been started!" << endl;
    flush(cout);
    
    while (1)
    {
    }
    
    return 0;
}

int main()
{
    cout << "Plant has been started!" << endl;

    /**
     * @brief 
     * 
     */
    pthread_t thread_server;
    pthread_t thread_client;

    pthread_create(&thread_server, NULL, server, NULL);
    pthread_create(&thread_client, NULL, client, NULL);

    pthread_join(thread_server, NULL);
    pthread_join(thread_client, NULL);
  
    return 0;
}