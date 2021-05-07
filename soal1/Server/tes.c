// global variable 
pthread_mutex_t lock;
pthread_cond_t cv;
int number_of_connected_client = 0;

// a function executed by a thread to handle each user
void *client_thread(void *arg) {

  /* some code */

  // thread is about to finish and return
  // client disconnected --> decrement number of connected clients 
  // signal that a client disconnected
  pthread_mutex_lock(&lock);
  number_of_connected_client-- ;
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&lock);
  return 0;
}

// infinte loop for accepting connections
while (1) {

  // serve 2 users at a time, queue the rest of the clients in the listen's queue 
  if( number_of_connected_client >= 2 ) {
    pthread_mutex_lock(&lock);      
    pthread_cond_wait(&cv, &lock);
    pthread_mutex_unlock(&lock);
  }

  connfd = accept(listenfd, (struct sockaddr *) NULL, NULL );

  // create a thread and serve the client 
  if( pthread_create( &thread_id , NULL , client_thread, (void*)connfd) <0)
    {
     // error in creating thread
     // do something 
    }

  // client is served --> increment number of connected clients 
  else {
    pthread_mutex_lock(&lock);
    number_of_connected_client ++;
    pthread_mutex_unlock(&lock);
  }

}