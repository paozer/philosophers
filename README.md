The aim of this solo project is to initiate students to the wonders of threading processes and protecting resources that are shared across threads or processes.

This project is subject to 42's norm which restricts naming conventions, function and line lenghts and more. Some weird sections may therefore be the result of said norm. If you don't respect the norm you fail the project.

I did this project at home during quarantine which made the evaluation difficult due to the differences between mine and the evaluators computer.

### Rules
This is 42's take on the famous philosophers problem. A number of philosophers sit around a table and share a bowl of spaghetti as well as the forks to eat. In philo_one there is one fork between every pair of philosophers. In philo_two and three the forks are in the middle of the table. The philosophers life cycle is the following: take two forks, eat, put down the forks, sleep, think and startover.
When launching each simulation the program takes following arguments:
- number of philosophers
- time to die: if time between meals exceeds this the philo dies
- time to eat: each philo takes this amount of time to eat
- time to sleep: each philo takes this amount of time to sleep
- optionally the number of required meals each philosopher has to eat

If a philosopher dies or all philos ate enough the program returns.

All three variations are a solution to said problem using different tools: threads/mutexes, threads/semaphores or processes/semaphores. At every step of a philos lifecycle except putting down the forks he writes a log the stdout.

### Resources
- [introduction](https://computing.llnl.gov/tutorials/pthreads/)
- [threads/synchronization/processes](https://github.com/angrave/SystemProgramming/wiki/)
- [threads/synchronization/processes](https://greenteapress.com/thinkos/thinkos.pdf)
- [semaphore example](https://gist.github.com/junfenglx/7412986)
- [debugging](https://clang.llvm.org/docs/ThreadSanitizer.html)

### Allowed Functions
- all philos: memset, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join
- philo_one: pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
- philo_two: sem_open, sem_close, sem_post, sem_wait, sem_unlink
- philo_three: waitpid, sem_open, sem_close,sem_post, sem_wait, sem_unlink
