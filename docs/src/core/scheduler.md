# Core Library - Task Scheduler

The scheduler headers provide functionality for adding, modifying, scheduling and transforming tasks.

The scheduler is split into three headers, which each contain one or more of the functionalities.

## Headers

- `core/scheduler.h` Groups the

  - `core/scheduler/add_remove_replace.h` Contains the
    - `TaskHandler add(const Task &task)` function, which takes one of the [Task types](task.md)
      and tries to add it into it's associated queue. Returns a valid [TaskHandler](task_handler.md) on success.
    
    - `void remove(TaskHandler &task)` function, which removes the task associated with 
      the [TaskHandler](task_handler.md) this function checks the validity of the task, 
      an invalidates the handler on successful removal.
    
    - `void removeUnsafe(const TaskHandler &task)` function, which removes the task associated with the
      [TaskHandler](task_handler.md) without checking it's validity or invalidating the handler.
    
    - `void replace(const TaskHandler &task, const Task &newTask)` function, which replaces the existing task associated
      with the [TaskHandler](task_handler.md) with the new task.
    
      > If the existing task and the new task have a different type, the function will __fatal__.

  - `core/scheduler/transfer_continue.h` Contains
    - `void continueWith(const TaskHandler &task, Handler newHandler)` function, which replaces the handler of the task
      associated with the [TaskHandler](task_handler.md) with the new function handler which is one 
      of [Handler types](handler.md).
      
      > __IMPORTANT:__ This function has no means of validating the new handler signature, so passing a handler
        incompatible with the current task's argument will produce undefined behavior
    
    - `void transfer(TaskHandler &task, Into into)` function, which transfers the task associated with 
      the [TaskHandler](task_handler.md) into the queue of a different type, based on one of  
      the [Into types](task_type_transformer.md), and modifies the handler accordingly.
      
      > If the task is invalid, or the task is transformed into it's own queue, this function __does nothing and logs__
        a failure.
    
    - `void continueInto(TaskHandler &task, Handler newHandler, Into into)` function, which transfers the task 
      associated with the [TaskHandler](task_handler.md) into the queue of a different type, based on one of 
      the [Into types](task_type_transformer.md), and replaces the function handler of the task with the `newHandler`.
      
      > __IMPORTANT:__ This function has no means of validating the new handler signature, so passing a handler
        incompatible with the current task's argument will produce undefined behavior

  - `core/scheduler/schedule.h` Contains
	- `void scheduleAll()` function, which schedules tasks from every queue.
	
	- `void scheduleQuick()` function, which schedules only tasks from the __Forever__ and __Periodic__ queues.
	
	- `void scheduleSlow()` function, which schedules only tasks from the __Once__ queues.

## Example Usage

### Add Example
```cpp
#include <core/scheduler/add_remove_replace.h>

static void updateFoo(TaskHandler task)
{
    log("Foo is updating\n");
} 

static void onceBar(TaskHandler task, uint32_t *arg)
{
    log("Incrementing arg: ", ++(*arg), "\n");
}

void main()
{
    // Adding a Periodic task with no arguments
    core::scheduler::add(core::Periodic(core::Time::Seconds(1), updateFoo));

    // Adding a Once task with default arguments
    core::scheduler::add(core::Once(onceBar));

    // Adding a Once task with initial arguments, and checking the TaskHandler
    const auto task = core::scheduler::add(core::Once(onceBar, uint32_t{0x20}));

    if(!task)
        fatal("Oh no! Failed to add task onceBar!\n");
}
```

### Remove/Replace example
```cpp
#include <core/scheduler/add_remove_replace.h>

static void displayFoo(TaskHandler task, uint32_t *arg)
{
    log("Value of foo: ", *arg, "\n");
}

static void doubleFoo(TaskHandler task, uint32_t *arg)
{
    *arg *= 2;
}

void main()
{
    // Add a Periodic displayFoo
    auto task = core::scheduler::add(core::Periodic(core::Time::Seconds(1), displayFoo);

    // Replace it with doubleFoo
    core::scheduler::replace(task, core::Periodical(core::Time::Seconds(4), doubleFoo, uint32_t{0x1000});

    // Remove doubleFoo
    core::scheduler::remove(task); 
}
```

### Transfer/Continue Example
```cpp
#include <core/scheduler/transfer_continue.h>

static void displayFoo(TaskHandler task, uint32_t *arg)
{
    // "Value of foo: 0x2000\n"
    log("Value of foo: ", *arg, "\n");

    // Make displayFoo run forever
    core::scheduler::transfer(task, core::IntoForever{});
}

static void doubleFoo(TaskHandler task, uint32_t *arg)
{
    *arg *= 2;
    // *arg == 0x2000
    
    // Continue with displayFoo
    core::scheduler::continueWith(task, displayFoo);
}

void main()
{
    // Add a Once doubleFoo
    core::scheduler::add(core::Once(doubleFoo, uint32_t{0x1000}));
}
```
