# Philosophers

This is a project called "philo" which simulates the dining philosophers problem using threads and mutexes. It is implemented in the C programming language.

The dining philosophers problem is a classic synchronization problem in computer science, which involves a group of philosophers sitting around a table with a bowl of rice and chopsticks. Each philosopher alternates between thinking and eating. However, there are only a limited number of chopsticks available for the philosophers to share. The challenge is to design an algorithm that allows the philosophers to dine peacefully without getting into a deadlock or starvation situation.

The "philo" project provides a solution to this problem by creating a simulation of the dining philosophers scenario. It utilizes threads to represent each philosopher and mutexes to represent the chopsticks. The program ensures that the philosophers follow a set of rules to avoid deadlocks and provide fairness in resource allocation.

## Compilation

To build the project, follow these steps:

1. Navigate to the project directory:

   ```
   cd philo
   ```

2. Build the project using the provided Makefile:

   ```
   make
   ```

This will compile the source files and generate an executable named `philo`.

## Run

Once you have built the project, you can run it with the following command:

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

- `<number_of_philosophers>`: The number of philosophers sitting around the table.
- `<time_to_die>`: The maximum time in milliseconds that a philosopher can survive without eating.
- `<time_to_eat>`: The time in milliseconds it takes for a philosopher to finish eating.
- `<time_to_sleep>`: The time in milliseconds the philosopher spends sleeping after eating.
- `[number_of_times_each_philosopher_must_eat]` (optional): The number of times each philosopher must eat before the simulation ends. If not provided, the simulation will continue indefinitely.

Example usage:

```
./philo 5 800 200 200
```

This command will run the simulation with 5 philosophers, a time to die of 800 milliseconds, a time to eat of 200 milliseconds, and a time to sleep of 200 milliseconds. In this example, the simulation will continue indefinitely and any philosopher will die.

## Check Data Races
The project cannot have `data races`, which means it cannot have access to a shared variable between threads without a synchronization mechanism. To check if the project has `data races` was used the compilation flag `-fsanitize=thread`. To compile the program with this flag, on the Makefile, remove the `#` of line 21 and save.
```
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread
```
Then compile the project again.
```bash
make re
``` 

If the program has no data races, the program will run normally. Otherwise, the program will stop and show at the terminal where the error is.

## Grade: 100/100

## Used tests
- 42-philosophers-tester: https://github.com/dantonik/42-philosophers-tester
- alientest_philosopher: https://github.com/lienardale/alientest_philosopher
- philosophers-visualizer: https://nafuka11.github.io/philosophers-visualizer