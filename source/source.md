# < SystemC >

## [1. SystemC Module](#1) <a id="start"></a>
## [2. Constructor: SC_CTOR](#2)
## [3. SC_HAS_PROCESS](#3)
## [4. Simulation Process](#4)
## [5. Simulation Stages](#5)
## [6. Time Notation](#6)
## [7. Concurrency](#7)
## [8. Event](#8)
## [9.Combined Events](#9)
## [10.Delta Cycle](#10)
## [11.Sensitivity](#11)
## [12.Initialization](#12)
## [13.Process: Method](#13)
## [14.Event Queue](#14)
## [15.Combined Event Queue](#15)
## [16.Mutex](#16)
## [17.Semaphore](#17)
## [18.FIFO](#18)
## [19.Signal: read and write](#19)
## [20.Signal: detect event](#20)
## [21.Signal: many writers](#21)
## [22.Resolved Signal](#22)
## [23.sc_signal<bool>](#23)
## [24.Buffer](#24)
## [25.Communication: port](#25)
## [26.Communication: export](#26)
## [27.Communication: port 2 port](#27)
## [28.Communication: specialized ports](#28)
## [29.Communication: port array](#29)
## [30.Primitive Channel](#30)
## [31.Hierarchical Channel](#31)
## [32.Trace File](#32)
## [33.Error and Message Report](#33)
## [34.Customized Data Type](#34)
## [35.Clock](#35)
## [36.Process: CTHREAD](#36)
#

## <a id="1"></a>1. SA systemC module is:

**A systemC module is:**
  1. the smallest container of functionality with state, behavior, and strucure for hierarchical connectivity.
  2. a C++ class, which inherits a systemC basic class: sc_module.
  3. the principle structural building block of SystemC.
  4. used to repsent a component in real systems.

**How to define a systemC module:**
  1. SC_MODULE(module_name) {}: this uses the systemC defined macro "SC_MODULE", which is equivement to #2.
  2. struct module_name: public sc_module {}: a struct that inherits sc_module.
  3. class module_name : public sc_module { public: }: a class that inherits sc_module.

Note, class is identical to struct except for its default access control mode of "private", as compared to "public" of struct.

**How to use a systemC module:**
  1. Objects of class sc_module can only be constructed during elaboration. It shall be an error to instantiate a module during simulation.
  2. Every class derived (directly or indirectly) from sc_module shall have at least one constructor. Every constructor shall have one and only one parameter of class sc_module_name but may have further parameters of classes other than sc_module_name.
  3. A string-valued argument shall be passed to the constructor of every module instance. It is good practice to make this string name the same as the C++ variable name through which the module is referenced, if such a variable exists.
  4. (To be explained later) Inter-module communication should typically be accomplished using interface method calls; that is, a module should communicate with its environment through its ports. Other communication mechanisms are permissible, for example, for debugging or diagnostic purposes.ystemC Module

**Example**

```sh
	/* Headle file */
	SC_CTOR(module_a) {
		cout << "construct module a \n";
	}
    };

    struct module_b : public sc_module {
        
        SC_CTOR(module_b) {
            cout << "construct module b \n";
        }
    };

    class module_c : public sc_module {
        
    public:
        SC_CTOR(module_c) {
            cout << "construct module c \n";
        }
    };

    /* Source file */
    cout << "\n # 1> SystemC Module ===========================================> \n \n";
    module_a zModule_a("zModule_a");
    module_b zModule_b("zModule_b");
    module_c zModule_c("zModule_c");
    sc_start();
    cout << "\n>>>=============================================================> \n";

```

## <a id="2"></a>2. Constructor: SC_CTOR

Every c++ function must have a constructor. For a normal c++ function, a default constructor will be auto-generated if not explicitly provided.
However, every systemC module must have a unique "name", which is provided when instantiating a module object. This requires a constructor with at least one parameter.

SystemC provides a macro (SC_CTOR) for convenience when declaring or defining a constructor of a module. SC_CTOR:
  1. shall only be used where the rules of C++ permit a constructor to be declared and can be used as the declarator of a constructor declaration or a constructor definition.
  2. has only one argument, which is the name of the module class being constructed.
  3. cannot add user-defined arguments to the constructor. If an application needs to pass additional arguments, the constructor shall be provided explicitly. 

  **Example**
  ```sh
    /* Headle file */
    SC_MODULE(ctor_module_a) {
        SC_CTOR(ctor_module_a) {
            SC_METHOD(method_module_a);
        }

        void method_module_a() {
            cout << __FUNCTION__ << endl;
        }
    };

    SC_MODULE(ctor_module_b) {

        SC_CTOR(ctor_module_b) {
            SC_METHOD(method_module_b);
        }

        void method_module_b();
    };

    SC_MODULE(ctor_module_c) {
        const int value;
        SC_CTOR(ctor_module_c);

        ctor_module_c(sc_module_name name, int val) : sc_module(name), value(val) {
            SC_METHOD(method_module_c);
        }

        void method_module_c() {
            cout << __FUNCTION__ << ", value = " << value << endl;
        }
    };

    /* Source file */
    cout << "\n 2> Constructor: SC_CTOR ======================================> \n";
    ctor_module_a zctor_module_a("zctor_module_a");
    ctor_module_b zctor_module_b("zctor_module_b");
    ctor_module_c zctor_module_C("zctor_module_c", 7);

  ```
## [>>>>>>> Back](#start)

## <a id="3"></a>3. SC_HAS_PROCESS
SC_HAS_PROCESS is introduced since systemC v2.0. It takes only one argument which is the name of the module class. It is often compared with SC_CTOR. Let's see how these two macros are defined:
  1. SC_SCOR: #define SC_CTOR(user_module_name) typedef user_module_name SC_CURRENT_USER_MODULE; user_module_name( ::sc_core::sc_module_name )
  2. SC_HAS_PROCESS: #define SC_HAS_PROCESS(user_module_name) typedef user_module_name SC_CURRENT_USER_MODULE

When providing e.g. "module" as input argument to SC_CTOR and SC_HAS_PROCESS, they expand to:
  1. SC_CTOR(module): typedef module SC_CURRENT_USER_MODULE; module( ::sc_core::sc_module_name )
  2. SC_HAS_PROCESS(module): typedef module SC_CURRENT_USER_MODULE;

From this you can see:
1. both defines the "module" as "SC_CURRENT_USER_MODULE", which is used when registering member functions to simulation kernel via SC_METHOD/SC_THREAD/SC_CTHREAD.
2. SC_CTOR also declares a default constructor with module name as the only input argument. The impact is:
  a) SC_CTOR saves one line of code to write a constructor file, whereas if using SC_HAS_PROCESS, one has to declare a constructor function:
      module_class_name(sc_module_name name, additional argument ...);
  b) since SC_CTOR has a constructor functior declarition, it can only be placed inside class header.

My recommendation:
  1. Don't use SC_CTOR or SC_HAS_PROCESS if a module has no simulation process (member functions registered to simulation kernel via SC_METHOD/SC_THREAD/SC_CTHREAD)
  2. Use SC_CTOR if the module need no additional parameter (other than module name) to instantiate
  3. Use SC_HAS_PROCESS when additional parameters are needed during instantiation

```sh
    /* Headle file */
    SC_MODULE(process_module_a) {

	SC_HAS_PROCESS(process_module_a);
	process_module_a(sc_module_name name) {
		SC_METHOD(method_process_moduele_a);
	}

	void method_process_moduele_a() {
            cout << __FUNCTION__ << ": " << name() << ", SC_HAS_PROCESS" << endl;
        }
    };

    SC_MODULE(process_module_b) {

        const int value;
        SC_HAS_PROCESS(process_module_b);

        process_module_b(sc_module_name name, int val) : sc_module(name), value(val) {
            SC_METHOD(method_process_module_b);
        }

        void method_process_module_b() {
            
            cout << __FUNCTION__ << ": " << name() << ", SC_HAS_PROCESS : " << "value : " << value << endl;
        }
    };

    /* Source file */
    cout << "# 3> SC_HAS_PROCESS ==========================================================>";
    process_module_a zprocess_module_a("zprocess_module_a");
    process_module_b zprocess_module_b("zprocess_module_b", 7);

```

## [>>>>>>> Back](#start)
## <a id="4"></a>4. Simulation Process

**A systemC module is:**
  1. the smallest container of functionality with state, behavior, and strucure for hierarchical connectivity.
  2. a C++ class, which inherits a systemC basic class: sc_module.
  3. the principle structural building block of SystemC.
  4. used to repsent a component in real systems.

**How to define a systemC module:**
  1. SC_MODULE(module_name) {}: this uses the systemC defined macro "SC_MODULE", which is equivement to #2.
  2. struct module_name: public sc_module {}: a struct that inherits sc_module.
  3. class module_name : public sc_module { public: }: a class that inherits sc_module.

Note, class is identical to struct except for its default access control mode of "private", as compared to "public" of struct.

**How to use a systemC module:**
  1. Objects of class sc_module can only be constructed during elaboration. It shall be an error to instantiate a module during simulation.
  2. Every class derived (directly or indirectly) from sc_module shall have at least one constructor. Every constructor shall have one and only one parameter of class sc_module_name but may have further parameters of classes other than sc_module_name.
  3. A string-valued argument shall be passed to the constructor of every module instance. It is good practice to make this string name the same as the C++ variable name through which the module is referenced, if such a variable exists.
  4. (To be explained later) Inter-module communication should typically be accomplished using interface method calls; that is, a module should communicate with its environment through its ports. Other communication mechanisms are permissible, for example, for debugging or diagnostic purposes.

  ```sh
    /* Header file */
    SC_MODULE(process) {

	sc_clock clk;
	SC_CTOR(process) : clk("clk", 1, SC_SEC) {

		SC_METHOD(method);
		SC_THREAD(thread);
		SC_CTHREAD(cthread, clk);
	}

	void method(void);
	void thread();
	void cthread();

    /* Source file */
    void process::method() {

        cout << "method triggred @" << sc_time_stamp() << endl;
        next_trigger(sc_time(1, SC_SEC));
    }

    void process::thread() {
        
        while (true) {

            // cout << __FUNCTION__ <<" triggered @ " << sc_time_stamp() << std::endl;
            wait(2, SC_SEC);
        }
    }

    void process::cthread() {

        while (true) {

            cout << __FUNCTION__ << " triggered @ " << sc_time_stamp() << std::endl;
            wait();
        }
    }

    /* main.cpp file */
    process zprocess("zprocess");
    };
  ```

## [>>>>>>> Back](#start)
## <a id="5"></a>5. Simulation Stages

**The systemC application has three phases/stages of operation:**
  1. Elaboration: execution of statements prior to sc_start().
    The primary purpose is to create internal data structures to support the semantics of simulation.
    During elaboration, the parts of the module hierarchy (modules, ports, primitive channels, and processes) are created, and ports and exports are bound to channels.
  2. Execution: further break-down to two stages:
    a) Initialization
      simulation kernel identifies all simulation processes and place them in either a runnable or waiting process set.
      All simulation processes are in runnable set except those requesting "no initialization".
    b) Simulation
      is commonly described as a state machine that schedules processes to run, and advances simulation time. It has two internal phases:
        1) evaluate: run all runnable processes one at a time. Each process runs till reaches wait() or return. Stops if no runnable processes left.
        2) advance-time: once the set of runnable processes is emptied, simulation enters advance-time phase where it does:
          a) move simulated time to the closest time with a scheduled event
          b) move processes waiting for that particular time into the runnable set
          c) return to evaluation phase
        The progression from evaluate to advance-time continues until one of the three things occurs. Then it moves to the cleanup phase.
          a) all processes have yielded
          b) a process has executed sc_stop()
          c) maximum time is reached
  3. Cleanup or post-processing: destroy objects, releases memory, close open files etc.

**Four callback functions are called by the kernel at various stages during elaboration and simulation. They have the following declarations:**
  1. virtual void before_end_of_elaboration(): called after the construction of the module hierarchy
  2. virtual void end_of_elaboration(): called at the very end of elaboration after all callbacks to before_end_of_elaboration have completed and after the completion of any instantiation or port binding performed by those callbacks and before starting simulation.
  3. virtual void start_of_simulation(): 
    a) called immediately when the application calls sc_start for the first time or at the very start of simulation, if simulation is initiated under the direct control of the kernel.
    b) if an application makes multiple calls to sc_start, start_of_simulation is called on the first call to sc_start.
    c) called after the callbacks to end_of_elaboration and before invoking the initialization phase of the scheduler.
  4. virtual void end_of_simulation():
    a) called when the scheduler halts because of sc_stop or at the very end of simulation if simulation is initiated under the direct control of the kernel.
    b) called only once even if sc_stop is called multiple times.

```sh
    /* Header file+ */
    SC_MODULE(stage) {
	SC_CTOR(stage) {
		cout << __FUNCTION__ << ": " << sc_time_stamp() << ": Elaboration: constructor" << std::endl;
		SC_THREAD(stageThread);
	}

	void stageThread();
	~stage() {
		cout << __FUNCTION__ << ": " << sc_time_stamp() << ": Cleanup: desctructor" << std::endl;
	}

	void before_end_of_elaboration() {
		std::cout << "before end of elaboration" << std::endl;
	}
	void end_of_elaboration() {
		std::cout << "end of elaboration" << std::endl;
	}
	void start_of_simulation() {
		std::cout << "start of simulation" << std::endl;
	}
	void end_of_simulation() {
		std::cout << "end of simulation" << std::endl;
	}

    /* Source file */
    void stage::stageThread() {

	cout << __FUNCTION__ << ": " << sc_time_stamp() << ": Execution.initialization" << std::endl;
	int i = 0;
	while (true) {
		wait(1, SC_SEC);
		cout << __FUNCTION__ << ": " << sc_time_stamp() << ": Execution.simulation" << std::endl;
		if (++i >= 2) {
			sc_stop();
		}
    }

    /* main.cpp file */
    stage zstage("zstage");
};
```

## [>>>>>>> Back](#start)
## <a id="6"></a> 6. Time Notation

**Let's first understand the difference of the two time measurements:**
  1. wall-clock time
    the time from the start of execution to completion, including time waiting on other system activities and applications
  2. simulated time
    the time being modeled by the simulation, which may be less than or greater than the simulation's wall-clock time.

In systemC, sc_time is the data type used by simulation kernal to track simulated time. It defines several time units:
SC_SEC, SC_MS, SC_US, SC_NS, SC_PS, SC_FS. Each subsequent time unit is 1/1000 of its preceder.

sc_time objects may be used as operands for assignment, arithmetic, and comparison operations:
  multiplication allows one of its operands to be a double
  division allows the divisor to be a double

SC_ZERO_TIME:
a macro representing a time value of zero. It is good practice to use this constant whenever writing a time value of zero, for example, when creating a delta notification or a delta time-out.

To get current simulated time, use sc_time_stamp().

```sh
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );
    sc_set_time_resolution(1, SC_FS);
    sc_set_default_time_unit(1, SC_SEC);
    std::cout << "1 SEC =     " << sc_time(1, SC_SEC).to_default_time_units() << " SEC"<< std::endl;
    std::cout << "1  MS = " << sc_time(1, SC_MS).to_default_time_units()  << " SEC"<< std::endl;
    std::cout << "1  US = " << sc_time(1, SC_US).to_default_time_units()  << " SEC"<< std::endl;
    std::cout << "1  NS = " << sc_time(1, SC_NS).to_default_time_units()  << " SEC"<< std::endl;
    std::cout << "1  PS = " << sc_time(1, SC_PS).to_default_time_units()  << " SEC"<< std::endl;
    std::cout << "1  FS = " << sc_time(1, SC_FS).to_default_time_units()  << " SEC"<< std::endl;
    sc_start(7261, SC_SEC);
    double t = sc_time_stamp().to_seconds();
    std::cout << int(t) / 3600 << " hours, " << (int(t) % 3600) / 60 << " minutes, " << (int(t) % 60) << "seconds" << std::endl;
    return 0;
}
```

## [>>>>>>> Back](#start)
## <a id="7"></a> 7.Concurrency

SystemC uses simulation processes to model concurrency. It's not true concurrent execution.
When multiple processes are simulated as running concurrently, only one is executed at a particular time. However, the simulated time remain unchanged until all concurrent processes finishes their current tasks.
Thus, these processes are running concurrently on the same "simulated time". This differs from e.g. the Go language, which is real concurrency.

Let's understand the simulated concurrency with a simple example.

```sh
    SC_MODULE(concurrency) {
        SC_CTOR(concurrency) {
            SC_THREAD(thread1);
            SC_THREAD(thread2);
        }
    void thread1() {
        while(true) {
        std::cout << sc_time_stamp() << ": thread1" << std::endl;
        wait(2, SC_SEC);
        }
    }
    void thread2() {
        while(true) {
        std::cout << "\t" << sc_time_stamp() << ": thread2" << std::endl;
        wait(3, SC_SEC);
        }
    }
};
```

## [>>>>>>> Back](#start)
## <a id="8"></a> 8.Event

An event is an object of class sc_event used for process synchronization.
A process instance may be triggered or resumed on the occurrence of an event, i.e., when the event is notified.
Any given event may be notified on many separate occasions.

**sc_event has the following methods:**
  1. void notify(): create an immediate notification
  2. void notify(const sc_time&), void notify(double, sc_time_unit):
    a) zero time: create a delta notification.
    b) non-zero time: create a timed notification at the given time, expressed relative to the simulation time when function notify is called
  3. cancel(): delete any pending notification for this event
    a) At most one pending notification can exist for any given event.
    b) Immediate notification cannot be cancelled.

**Constraints:**
  1. Objects of class sc_event may be constructed during elaboration or simulation.
  2. Events may be notified during elaboration or simulation, except that it shall be an error to create an immediate notification during elaboration or from one of the callbacks:
    a) before_end_of_elaboration,
    b) end_of_elaboration, or
    c) start_of_simulation.

**A given event shall have no more than one pending notification:**
  1. If function notify is called for an event that already has a notification pending, only the notification scheduled to occur at the earliest time shall survive.
  2. The notification scheduled to occur at the later time shall be cancelled (or never be scheduled in the first place).
  3. An immediate notification is taken to occur earlier than a delta notification, and a delta notification earlier than a timed notification. This is irrespective of the order in which function notify is called.

Events can be combined with each other, and with a timer. This example shows a process waiting for only one event.

```sh
    /* Header file */
    SC_MODULE(event) {
	
	sc_event e;
	SC_CTOR(event) {
		SC_THREAD(trigger);
		SC_THREAD(catcher);
	}

	void trigger();
	void catcher();
    };

    /* Source file */
    void event::trigger() {
        while (true)
        {
            e.notify(0, SC_SEC);
            if (sc_time_stamp() == sc_time(2, SC_SEC)) {
                e.cancel();
            }

            wait(2, SC_SEC);
        }
    }

    void event::catcher() {
        while (true)
        {
            wait(e);
            cout << "Event cateched at : " << sc_time_stamp() << std::endl;
        }
    }

    /* main.cpp file */
    event zevent("zevent");
    sc_start(8, SC_SEC);

```

## [>>>>>>> Back](#start)
## <a id="9"></a> 9.Combined Events

**The following forms of wait() are supported.**
   1. wait(): wait on events in sensitivity list (SystemC 1.0).
   2. wait(e1): wait on event e1.
   3. wait(e1 | e2 | e3): wait on events e1, e2, or e3.
   4. wait(e1 & e2 & e3): wait on events e1, e2, and e3.
   5. wait(200, SC_NS): wait for 200 ns.
   6. wait(200, SC_NS, e1): wait on event e1, timeout after 200 ns.
   7. wait(200, SC_NS, e1 | e2 | e3): wait on events e1, e2, or e3, timeout after 200 ns.
   8. wait(200, SC_NS, e1 & e2 & e3): wait on events e1, e2, and e3, timeout after 200 ns.
   9. wait(sc_time(200, SC_NS)): wait for 200 ns.
  10. wait(sc_time(200, SC_NS), e1): wait on event e1, timeout after 200 ns.
  11. wait(sc_time(200, SC_NS), e1 | e2 | e3): wait on events e1, e2, or e3, timeout after 200 ns.
  12. wait(sc_time(200, SC_NS), e1 & e2 & e3 ): wait on events e1, e2, and e3, timeout after 200 ns.
  13. wait(200): wait for 200 clock cycles, SC_CTHREAD only (SystemC 1.0)
  14. wait(0, SC_NS): wait one delta cycle.
  15. wait(SC_ZERO_TIME): wait one delta cycle.

Note:
  a mixture of "|" operators and "&" operators is not supported in SystemC 2.0

```sh
    /* Header file */
    SC_MODULE(combine) {
    /* declare multiple events */
    sc_event e1, e2, e3, e4, e5, e6, e7, e8, e9, e10;
    SC_CTOR(combine) {
    /* reigster trigger */
    SC_THREAD(trigger);
    /* register catchers */
    SC_THREAD(catcher_0);
    SC_THREAD(catcher_1);
    SC_THREAD(catcher_2and3);
    SC_THREAD(catcher_4or5);
    SC_THREAD(catcher_timeout_or_6);
    SC_THREAD(catcher_timeout_or_7or8);
    SC_THREAD(catcher_timeout_or_9and10);
    }

    /* ource file */
    void trigger(void) {
        e1.notify(1, SC_SEC);
        e2.notify(2, SC_SEC);
        e3.notify(3, SC_SEC);
        e4.notify(4, SC_SEC);
        e5.notify(5, SC_SEC);
        e6.notify(6, SC_SEC);
        e7.notify(7, SC_SEC);
        e8.notify(8, SC_SEC);
        e9.notify(9, SC_SEC);
        e10.notify(10, SC_SEC);
    }

    void catcher_0(void) {
        wait(2, SC_SEC);
        std::cout << sc_time_stamp() << ": 2sec timeout" << std::endl;
    }
    void catcher_1(void) {
        wait(e1);
        std::cout << sc_time_stamp() << ": catch e1" << std::endl;
    }
    void catcher_2and3(void) {
        wait(e2 & e3);
        std::cout << sc_time_stamp() << ": catch e2 and e3" << std::endl;
    }
    void catcher_4or5(void) {
        wait(e4 | e5);
        std::cout << sc_time_stamp() << ": catch e4 or e5" << std::endl;
    }
    void catcher_timeout_or_6(void) {
        wait(sc_time(5, SC_SEC), e6);
        std::cout << sc_time_stamp() << ": 5sec timeout or catch e6"<< std::endl;
    }
    void catcher_timeout_or_7or8(void) {
        wait(sc_time(20, SC_SEC), e7 | e8);
        std::cout << sc_time_stamp() << ": 20sec timeout or catch e7 or e8" << std::endl;
    }
    void catcher_timeout_or_9and10(void) {
        wait(sc_time(20, SC_SEC), e9 & e10);
        std::cout << sc_time_stamp() << ": 20sec timeout or catch (e9 and e10)" << std::endl;
    }

    /* main.cpp */
    combine combined("combined");
    sc_start();
```

## [>>>>>>> Back](#start)
## <a></a> 10.Delta Cycle

A delta cycle can be thought of as a very small step of time within the simulation, which does not increase the user-visible time.

A delta cycle is comprised of separate evaluate and update phases, and multiple delta cycles may occur at a particular simulated time. 
When a signal assignment occurs, other processes do not see the newly assigned value until the next delta cycle.

**When is delta cycle used:**
  1. notify(SC_ZERO_TIME) causes the event to be notified in the evaluate phase of the next delta cycle, this is called a "delta notification".
  2. A (direct or indirect) call to request_update() causes the update() method to be called in the update phase of the current delta cycle. 

```sh
    SC_MODULE(delta) {
        int x = 1, y = 1;
        SC_CTOR(delta) {
            SC_THREAD(add_x);
            SC_THREAD(multiply_x);
            SC_THREAD(add_y);
            SC_THREAD(multiply_y);
        }
    }

    void add_x() {
        std::cout << "add_x: " << x << " + 2" << " = ";
        x += 2;
        std::cout << x << std::endl;
    }
    
    /* x *= 3 happens after a delta cycle */
    void multiply_x() {
        wait(SC_ZERO_TIME);
        std::cout << "multiply_x: " << x << " * 3" << " = ";
        x *= 3;
        std::cout << x << std::endl;
    }

    /* y += 2 happens after a delta cycle */
    void add_y() {
        wait(SC_ZERO_TIME);
        std::cout << "add_y: " << y << " + 2" << " = ";
        y += 2;
        std::cout << y << std::endl;
    }

    /* y *=3 happens first 8/
    void multiply_y() {
        std::cout << "multiply_y: " << y << " * 3" << " = ";
        y *= 3;
        std::cout << y << std::endl;
    }
```

## [>>>>>>> Back](#start)
## <a id="11"></a> 11.Sensitivity

The sensitivity of a process instance is the set of events and time-outs that can potentially cause the process to be resumed or triggered.

A process instance is said to be sensitive to an event if the event has been added to the static sensitivity or dynamic sensitivity of the process instance.
A time-out occurs when a given time interval has elapsed.

**Two types of sensitivities:**
  1. Static sensitivity is fixed during elaboration, supported with a sensitivity list for each process in a module.
  2. Dynamic sensitivity may vary over time under the control of the process itself, support with wait() for a thread, or next_trigger() for a method.

```sh
    SC_MODULE(SENSITIVITY) {
        /* events for inter-process triggering */+
        sc_event e1, e2;
        SC_CTOR(SENSITIVITY) {
            /* register processes */
            SC_THREAD(trigger_1);
            SC_THREAD(trigger_2);
            SC_THREAD(catch_1or2_dyn);
            SC_THREAD(catch_1or2_static);
            /* static sensitivity for the preceeding process, can only "OR" the triggers */
            sensitive << e1 << e2;
        }
        void trigger_1() {
            /* delay trigger by a delta cycle, make sure catcher is ready */
            wait(SC_ZERO_TIME);
            while (true) {
                /* trigger e1 */
                e1.notify();
                /* dynamic sensitivity, re-trigger after 2 */
                wait(2, SC_SEC);
            }
        }
        /* delay trigger by a delta cycle */
        void trigger_2() {
            wait(SC_ZERO_TIME);
            while (true) {
                /* trigger e2 */
                e2.notify();
                /* dynamic sensitivity, re-trigger after 3 s */
                wait(3, SC_SEC);
            }
        }
        void catch_1or2_dyn() {
            while (true) {
            /* dynamic sensitivity */
            wait(e1 | e2);
                std::cout << "Dynamic sensitivty: e1 or e2 @ " << sc_time_stamp() << std::endl;
            }
        }
        void catch_1or2_static(void) {
            while (true) {
            /* static sensitivity */
                wait();
                std::cout << "Static sensitivity: e1 or e2 @ " << sc_time_stamp() << std::endl;
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="12"></a> 12.Initialization

Initialization is part of the execution stage, which happens after sc_start(). During initialization it perform the following three steps in the order given:
  1) Run the update phase but without continuing to the delta notification phase.
  2) Add every method and thread process instance in the object hierarchy to the set of runnable processes, excluding:
    a) those process instances for which the function dont_initialize has been called, and
    b) clocked thread processes.
  3) Run the delta notification phase. At the end of the delta notification phase, go to the evaluation phase.

NOTE:
  1. the update and delta notification phases are necessary because update requests can be created during elaboration in order to set initial values for primitive channels, for example, from function initialize of class sc_inout.
  2. In SystemC 1.0,
    a) thread processes are not executed during the initialization phase of the simulation.
    b) method processes are executed during the initialization phase of the simulation if they are made sensitive to input signals/ports.
  3. SystemC 2.0 scheduler will execute all thread processes and all method processes during the initialization phase of the simulation.
    If the behavior of a thread process is different between SystemC 1.0 and SystemC 2.0, insert one wait() statement before the infinite loop of the thread process.
  5. during initialization Phase, processes (SC_METHODs in SystemC 1.0; SC_METHODs and SC_THREADs in SystemC 2.0) are executed in an unspecified order.
  4. dont_initialize(): used to prevent the scheduler from executing a thread or method process during the initialization phase. Applies to the last declared process.

```sh
    SC_MODULE(INITIALIZATION) {
        /* event for inter-process trigger */
        sc_event e;
        SC_CTOR(INITIALIZATION) {
            /* no static sensitivity */
            SC_THREAD(trigger);
            SC_THREAD(catcher_1);
            SC_THREAD(catcher_2);
            SC_THREAD(catcher_3);

            /* statically sensitive to e */
            sensitive << e;
            /* don't initialize */
            dont_initialize();
        }
        void trigger() {
            // e triggered at 1, 3, 5, 7 ...
            while (true) {
            // notify after 1 s
            e.notify(1, SC_SEC);
            // trigger every 2 s
            wait(2, SC_SEC);
            }
        }
        void catcher_1() {
            while (true) {
                std::cout << sc_time_stamp() << ": catcher_1 triggered" << std::endl;
                // dynamic sensitivity
                wait(e);
            }
        }
        void catcher_2() {
            // avoid initialization --- mimic systemC 1.0 behavior
            wait(e);
            while (true) {
                std::cout << sc_time_stamp() << ": catcher_2 triggered" << std::endl;
                // dynamic sensitivity
                wait(e);
            }
        }
        // relies on dont_initialize() to avoid initialization
        void catcher_3() {
            while (true) {
                std::cout << sc_time_stamp() << ": catcher_3 triggered" << std::endl;
                // dynamic sensitivity
                wait(e);
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="13"></a> 13.Process: Method

**Method:**
  1. may have static sensitivity.
  2. only a method process, may call the function next_trigger to create dynamic sensitivity.
  2. cannot be made runnable as a result of an immediate notification executed by the process itself, regardless of the static sensitivity or dynamic sensitivity of the method process instance.

**next_trigger():**
  1. is a member function of class sc_module, a member function of class sc_prim_channel, and a non-member function.
  2. can be called from:
    a) a member function of the module itself,
    b) from a member function of a channel, or
    c) from any function that is ultimately called from a method process.

NOTE:
  1) any local variables declared within the method process will be destroyed on return from the process. Data members of the module should be used to store persistent state associated with the method process.

**Recall the difference between SC_METHOD and SC_THREAD**
  1. SC_METHOD(func): does not have its own thread of execution, consumes no simulated time, cannot be suspended, and cannot call code that calls wait()
  2. SC_THREAD(func): has its own thread of execution, may consume simulated time, can be susupended, and can call code that calls wait()

```sh
    SC_MODULE(PROCESS) {
        SC_CTOR(PROCESS) { // constructor
            SC_THREAD(thread); // register a thread process
            SC_METHOD(method); // register a method process
        }
        void thread() {
            int idx = 0; // declare only once
            while (true) { // loop forever
            std::cout << "thread"<< idx++ << " @ " << sc_time_stamp() << std::endl;
            wait(1, SC_SEC); // re-trigger after 1 s
            }
        }
        void method() {
            // notice there's no while loop here
            int idx = 0; // re-declare every time method is triggered
            std::cout << "method" << idx++ << " @ " << sc_time_stamp() << std::endl;
            next_trigger(1, SC_SEC);
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="14"></a> 14.Event Queue

**An event queue:**
  1. has a member function notify(), same as an event
  2. is a hierarchical channel and can have multiple notifications pending, this differs from an event which can have only one outstanding notificaiton scheduled.
  3. can only be constructed during elaboration.
  4. does not support immediate notification

**Member functions:**
  1. void notify(double, sc_time_unit) or void notify(const sc_time&):
    a) zero time i.e. SC_ZERO_TIME: a delta notification
    b) non-zero time: notification scheduled relative to the simulation time when function notify is called.
  2. void cancel_all(): immediately delete every pending notification for this event queue object including both delta and timed notifications.

```sh
    SC_MODULE(QUEUE) {
        sc_event e;
        sc_event_queue eq;
        SC_CTOR(QUEUE) {
            SC_THREAD(trigger);
            SC_THREAD(catch_e);
            sensitive << e;     // catch_e() will be triggered by event e
            dont_initialize();  // don't run cach_e() during initialization phase
            SC_THREAD(catch_eq);
            sensitive << eq;    // cach_eq() will be triggered by event queue eq
            dont_initialize();  // don't run catch_eq() during initialization phase
        }
        void trigger() {
            while (true) {
                e.notify(2, SC_SEC);    // trigger e afer 2 s
                e.notify(1, SC_SEC);    // trigger e after 1 s, replaces previous trigger
                eq.notify(2, SC_SEC);   // trigger eq after 2 s
                eq.notify(1, SC_SEC);   // trigger eq after 1 s, both triggers available
                wait(10, SC_SEC);       // another round
            }
        }
        void catch_e() {
            while (true) {
                std::cout << sc_time_stamp() << ": catches e" << std::endl;
                wait();     // no parameter --> wait for static sensitivity, i.e. e
            }
        }
        void catch_eq() {
            while (true) {
                std::cout << sc_time_stamp() << ": catches eq" << std::endl;
                wait(); // wait for eq
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="15"></a> 15.Combined Event Queue

1. Multiple event queues can be "OR"-ed to form the static sensitivity of a process.
  cannot use "AND" operation in static sensitivity.
2. Event queue cannot be used as input to wait(), thus cannot be used in dynamic sensitivity.

```sh
    SC_MODULE(QUEUE_COMBINED) {
        sc_event_queue eq1, eq2;
        SC_CTOR(QUEUE_COMBINED) {
            SC_THREAD(trigger);
            SC_THREAD(catcher);
            sensitive << eq1 << eq2; // eq1 "or" eq2, cannot "and"
            dont_initialize();
        }
        void trigger() {
            eq1.notify(1, SC_SEC); // eq1 at 1 s
            eq1.notify(2, SC_SEC); // eq1 at 2 s
            eq2.notify(2, SC_SEC); // eq2 at 2 s
            eq2.notify(3, SC_SEC); // eq2 at 3 s
        }
        void catcher() {
            while (true) {
                std::cout << sc_time_stamp() << ": catches trigger" << std::endl;
                wait(); // cannot use event queue in dynamic sensitivity
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="16"></a> 16.Mutex

**A mutex:**
  1. is a predefined channel intended to model the behavior of a mutual exclusion lock used to control access to a resource shared by concurrent processes.
  2.shall be in one of two exclusive states: unlocked or locked:
    a) only one process can lock a given mutex at one time.
    b) a mutex can only be unlocked by the process hat locked it. After being unlocked, a mutex may be locked by a different process.

**Member functions:**
  1. int lock():
    a) If the mutex is unlocked, lock() shall lock the mutex and return.
    b) If the mutex is locked, lock() shall suspend until the mutex is unlocked (by another process).
    c) If multiple processes attempt to lock the mutex in the same delta cycle, the choice of which process instance is given the lock shall be non-deterministic.
    d) shall unconditionally return the value 0.
  2. int trylock():
    a) If the mutex is unlocked, trylock() shall lock the mutex and shall return the value 0.
    b) If the mutex is locked, trylock() shall immediately return the value –1. The mutex shall remain locked.
  3. int unlock():
    a) If the mutex is unlocked, unlock() shall return the value –1. The mutex shall remain unlocked.
    b) If the mutex was locked by a process instance other than the calling process, unlock() shall return the value –1. The mutex shall remain locked.
    c) If the mutex was locked by the calling process, member function unlock shall unlock the mutex and shall return the value 0.
      1. immediate notification shall be used to signal the act of unlocking a mutex to other processes

```sh
    SC_MODULE(MUTEX) {
    sc_mutex m;
    SC_CTOR(MUTEX) {
        SC_THREAD(thread_1);
        SC_THREAD(thread_2);
    }
    void thread_1() {
        while (true) {
            if (m.trylock() == -1) { // try to lock the mutex
                m.lock(); // failed, wait to lock
                std::cout << sc_time_stamp() << ": thread_1 obtained resource by lock()" << std::endl;
            } else { // succeeded
                std::cout << sc_time_stamp() << ": thread_1 obtained resource by trylock()" << std::endl;
            }
            wait(1, SC_SEC); // occupy mutex for 1 s
            m.unlock(); // unlock mutex
            std::cout << sc_time_stamp() << ": unlocked by thread_1" << std::endl;
            wait(SC_ZERO_TIME); // give time for the other process to lock the mutex
        }
    }
    void thread_2() {
        while (true) {
            if (m.trylock() == -1) { // try to lock the mutex
                m.lock(); // failed, wait to lock
                std::cout << sc_time_stamp() << ": thread_2 obtained resource by lock()" << std::endl;
            } else { // succeeded
                std::cout << sc_time_stamp() << ": thread_2 obtained resource by trylock()" << std::endl;
            }
            wait(1, SC_SEC); // occupy mutex for 1 s
            m.unlock(); // unlock mutex
            std::cout << sc_time_stamp() << ": unlocked by thread_2" << std::endl;
            wait(SC_ZERO_TIME); // give time for the other process to lock the mutex
        }
    }
};
```

## [>>>>>>> Back](#start)
## <a id="17"></a> 17.Semaphore

**A semaphore:**
  1. is a predefined channel intended to model the behavior of a software semaphore used to provide limited concurrent access to a shared resource.
  2. has an integer value, the semaphore value, which is set to the permitted number of concurrent accesses when the semaphore is constructed.
    a) if the initial value is one, the semaphore is equivelent to a mutex.

**Member functions:**
  1. int wait():
    a) If the semaphore value is greater than 0, wait() shall decrement the semaphore value and return.
    b) If the semaphore value is equal to 0, wait() shall suspend until the semaphore value is incremented (by another process).
    c) Shall unconditionally return the value 0.
  2. int trywait():
    a) If the semaphore value is greater than 0, trywait() shall decrement the semaphore value and shall return the value 0.
    b) If the semaphore value is equal to 0, trywait() shall immediately return the value –1 without modifying the semaphore value.
  3. int post():
    a) shall increment the semaphore value.
    b) shall use immediate notification to signal the act of incrementing the semaphore value to any waiting processes.
    c) shall unconditionally return the value 0.
  4. int get_value(): shall return the semaphore value.

```sh
    SC_MODULE(SEMAPHORE) {
        sc_semaphore s; // declares semaphore
        SC_CTOR(SEMAPHORE) : s(2) { // init semaphore with 2 resources
            SC_THREAD(thread_1); // register 3 threads competing for resources
            SC_THREAD(thread_2);
            SC_THREAD(thread_3);
        }
        void thread_1() {
            while (true) {
                if (s.trywait() == -1) { // try to obtain a resource
                    s.wait(); // if not successful, wait till resource is available
                }
                std::cout<< sc_time_stamp() << ": locked by thread_1, value is " << s.get_value() << std::endl;
                wait(1, SC_SEC); // occupy resource for 1 s
                s.post(); // release resource
                std::cout<< sc_time_stamp() << ": unlocked by thread_1, value is " << s.get_value() << std::endl;
                wait(SC_ZERO_TIME); // give time for the other process to lock
            }
        }
        void thread_2() {
            while (true) {
                if (s.trywait() == -1) { // try to obtain a resource
                    s.wait(); // if not successful, wait till resource is available
                }
                std::cout<< sc_time_stamp() << ": locked by thread_2, value is " << s.get_value() << std::endl;
                wait(1, SC_SEC); // occupy resource for 1 s
                s.post(); // release resource
                std::cout<< sc_time_stamp() << ": unlocked by thread_2, value is " << s.get_value() << std::endl;
                wait(SC_ZERO_TIME); // give time for the other process to lock
            }
        }
        void thread_3() {
            while (true) {
                if (s.trywait() == -1) { // try to obtain a resource
                    s.wait(); // if not successful, wait till resource is available
                }
                std::cout<< sc_time_stamp() << ": locked by thread_3, value is " << s.get_value() << std::endl;
                wait(1, SC_SEC); // occupy resource for 1 s
                s.post(); // release resource
                std::cout<< sc_time_stamp() << ": unlocked by thread_3, value is " << s.get_value() << std::endl;
                wait(SC_ZERO_TIME); // give time for the other process to lock
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="18"></a> 18.FIFO

**sc_fifo:**
  1. is a predefined primitive channel intended to model the behavior of a fifo, i.e., a first in first out buffer.
  2. has a number of slots for storing values. The number of slots is fixed when the object is constructed.
  3. implements the sc_fifo_in_if<T> interface and the sc_fifo_out_if<T> interface.

**Constructors:**
  1. explicit sc_fifo(int size_ = 16): calls the base class constructor from its initializer list as: sc_prim_channel(sc_gen_unique_name( "fifo" ))
  2. explicit sc_fifo(const char* name_, int size_ = 16): calls the base class constructor from its initializer list as: sc_prim_channel(name_)
  Both constructors initialize the number of slots in the fifo to the value given by the parameter size_. The number of slots shall be greater than zero.

**Member functions for read:**
  1. void read(T&), T read():
    a) return the value least recently written into the fifo and remove that value from the fifo such that it cannot be read again.
    b) the order in which values are read from the fifo shall precisely match the order in which values were written into the fifo.
    c) values written into the fifo during the current delta cycle are not available for reading in that delta cycle, but become available for reading in the immediately following delta cycle.
    d) if the fifo is empty, shall suspend until the data-written event is notified.
  2. bool nb_read(T&):
    a), b), c) same as read()
    d) if the fifo is empty, member function nb_read shall return immediately without modifying the state of the fifo, without calling request_update, and with a return value of false. Otherwise, if a value is available for reading, the return value of nb_read() shall be true.
  3. operator T(): equivalent to "operator T() {return read();}""

**Member functions for write:**
  1. write(const T&):
    a) write the value passed as an argument into the fifo.
    b) multiple values may be written within a single delta cycle.
    c) if values are read from the fifo during the current delta cycle, the empty slots in the fifo so created do not become free for the purposes of writing until the immediately following delta cycle.
    d) if the fifo is full, write() shall suspend until the data-read event is notified.
  2. bool nb_write(const T&):
    a), b), c) same as write()
    d) if the fifo is full, nb_write() shall return immediately without modifying the state of the fifo, without calling request_update, and with a return value of false. Otherwise, the return value of nb_write() shall be true.
  3. operator=: equivalent to "sc_fifo<T>& operator= (const T& a) {write(a); return *this;}"

**Member functions for events:**
  1. sc_event& data_written_event(): shall return a reference to the data-written event, that is notified in the delta notification phase that occurs at the end of the delta cycle in which a value is written into the fifo.
  2. sc_event& data_read_event(): shall return a reference to the data-read event, that is notified in the delta notification phase that occurs at the end of the delta cycle in which a value is read from the fifo.

**Member functions for available values and free slots:**
  1. int num_available(): returns the number of values that are available for reading in the current delta cycle. The calculation shall deduct any values read during the current delta cycle but shall not add any values written during the current delta cycle.
  2. int num_free(): returns the number of empty slots that are free for writing in the current delta cycle. The calculation shall deduct any slots written during the current delta cycle but shall not add any slots made free by reading in the current delta cycle.

```sh
    SC_MODULE(FIFO) {
        sc_fifo<int> f1, f2, f3;
        SC_CTOR(FIFO) : f1(2), f2(2), f3(2) { // fifo with size 2
            SC_THREAD(generator1);
            SC_THREAD(consumer1);

            SC_THREAD(generator2);
            SC_THREAD(consumer2);

            SC_THREAD(generator3);
            SC_THREAD(consumer3);
        }
        void generator1() { // blocking write
            int v = 0;
            while (true) {
            f1.write(v); // same as f = v, which is not recommended.
            std::cout << sc_time_stamp() << ": generator1 writes " << v++ << std::endl;
            wait(1, SC_SEC); // write every 1 s
            }
        }
        void consumer1() { // blocking read
            int v = -1;
            while (true) {
                f1.read(v); // same as v = int(f), which is not recommended; or, v = f1.read();
                std::cout << sc_time_stamp() << ": consumer1 reads " << v << std::endl;
                wait(3, SC_SEC); // read every 3 s, fifo will fill up soon
            }
        }
        void generator2() { // non-blocking write
            int v = 0;
            while (true) {
                while (f2.nb_write(v) == false ) { // nb write until succeeded
                    wait(f2.data_read_event()); // if not successful, wait for data read (a fifo slot becomes available)
                }
                std::cout << sc_time_stamp() << ": generator2 writes " << v++ << std::endl;
                wait(1, SC_SEC); // write every 1 s
            }
        }
        void consumer2() { // non-blocking read
            int v = -1;
            while (true) {
                while (f2.nb_read(v) == false) {
                    wait(f2.data_written_event());
                }
                std::cout << sc_time_stamp() << ": consumer2 reads " << v << std::endl;
                wait(3, SC_SEC); // read every 3 s, fifo will fill up soon
            }
        }
        void generator3() { // free/available slots before/after write
            int v = 0;
            while (true) {
                std::cout << sc_time_stamp() << ": generator3, before write, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
                f3.write(v++);
                std::cout << sc_time_stamp() << ": generator3, after write, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
                wait(1, SC_SEC);
            }
        }
        void consumer3() { // free/available slots before/after read
            int v = -1;
            while (true) {
                std::cout << sc_time_stamp() << ": consumer3, before read, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
                f3.read(v);
                std::cout << sc_time_stamp() << ": consumer3, after read, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
                wait(3, SC_SEC); // read every 3 s, fifo will fill up soon
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="19"></a> 19.Signal: read and write

**sc_signal:**
  1. is a predefined primitive channel intended to model the behavior of a single piece of wire carrying a digital electronic signal.
  2. uses the evaluate-update scheme to ensure deterministic behavior in the case of simultaneous read and write actions. We maintain a current and new value.
  3. its write() method will submit an update request if the new value is different from the current value.
  4. implements the sc_signal_inout_if<T> interface.

**Constructors:**
  1. sc_signal(): calls the base class constructor from its initializer list as: sc_prim_channel(sc_gen_unique_name("signal"))
  2. sc_signal(const char* name_): calls the base class constructor from its initializer list as: sc_prim_channel(name_)

**Member functions:**
  1. T& read() or operator const T& (): return a reference to the current value of the signal but shall not modify the state of the signal.
  2. void write(const T&): modifies the value of the signal such that the signal appears to have the new value (as returned by member function read) in the next delta cycle but not before then.
  3. operator=: equivalent to write()
  4. sc_event& default_event(), sc_event& value_changed_event(): return a reference to the value-changed event.
  5. bool event(): return true if and only if the value of the signal changed in the update phase of the immediately preceding delta cycle and at the current simulation time.

**As compared to fifo:**
  1. sc_signal has only one slot for read/write
  2. sc_signal triggers an update request only if the new value is different from the current value
  3. read from sc_signal won't remove the value

**Other than execution phase, a sc_signal:**
  1. may be written during elaboration to initialize the value of the signal.
  2. may be written from function sc_main during elaboration or while simulation is paused, that is, before or after the call to function sc_start.

```sh
    SC_MODULE(SIGNAL) {
        sc_signal<int> s;
        SC_CTOR(SIGNAL) {
            SC_THREAD(readwrite);
        }
        void readwrite() {
            s.write(3);
            std::cout << "s = " << s << "; " << s.read() << std::endl;
            wait(SC_ZERO_TIME);
            std::cout << "after delta_cycle, s = " << s << std::endl;
            
            s = 4;
            s = 5;
            int tmp = s;
            std::cout << "s = " << tmp << std::endl;
            wait(SC_ZERO_TIME);
            std::cout << "after delta_cycle, s = " << s.read() << std::endl;
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="20"></a>20.Signal: detect event

1. sc_event& default_event(), sc_event& value_changed_event(): return a reference to the value-changed event.
2. bool event(): return true if and only if the value of the signal changed in the update phase of the immediately preceding delta cycle and at the current simulation time.

```sh
    SC_MODULE(SIGNAL_EVENT) {
        sc_signal<int> s1, s2; // defines two signal channels
        SC_CTOR(SIGNAL_EVENT) {
            SC_THREAD(producer1);
            SC_THREAD(producer2);
            SC_THREAD(consumer); // consumer sensitive to (s1 OR s2)
            sensitive << s1 << s2; // same as: sensitive << s1.default_event() << s2.value_changed_event();
            dont_initialize();
        }
        void producer1() {
            int v = 1;
            while (true) {
            s1.write(v++); // write to s1
            wait(2, SC_SEC);
            }
        }
        void producer2() {
            int v = 1;
            while (true) {
                s2 = v++; // write to s2
                wait(3, SC_SEC);
            }
        }
        void consumer() {
            while (true) {
                if ( s1.event() == true && s2.event() == true) { // both triggered
                    std::cout << sc_time_stamp() << ": s1 & s2 triggered" << std::endl; 
                } else if (s1.event() == true) { // only s1 triggered
                    std::cout << sc_time_stamp() << ": s1 triggered" << std::endl; 
                } else { // only s2 triggered
                    std::cout << sc_time_stamp() << ": s2 triggered" << std::endl; 
                }
                wait();
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="21"></a>21.Signal: many writers

**Class definition of sc_signal:**
  template <class T, sc_writer_policy WRITER_POLICY = SC_ONE_WRITER> class sc_signal: public sc_signal_inout_if<T>, public sc_prim_channel {}
1. If WRITER_POLICY == SC_ONE_WRITER, it shall be an error to write to a given signal instance from more than one process instance at any time during simulation.
2. If WRITER_POLICY == SC_MANY_WRITERS:
  a) it shall be an error to write to a given signal instance from more than one process instance during any given evaluation phase,
  b) but different process instances may write to a given signal instance during different delta cycles.

Thus, by default an sc_signal has only one writer; when declared as MANY_WRITERS, the writers can write to the signal channel at different time.

As to consumers, an sc_signal may have more than one consumers. They can all read from the signal channel at same or different time.

```sh
    SC_MODULE(MULTI) {
        sc_signal<int> s1; // a single-writer signal
        sc_signal<int, SC_MANY_WRITERS> s2; // a multi-writer signal
        SC_CTOR(MULTI) {
            SC_THREAD(writer1); // writes to s1
            SC_THREAD(writer2); // writes to s1 and s2
            SC_THREAD(consumer1);
            sensitive << s1; // sensitive to s1
            dont_initialize();
            SC_THREAD(consumer2);
            sensitive << s1 << s2; // sensitive to s1 and s2
            dont_initialize();
        }
        void writer1() {
            int v = 1; // init value
            while (true) {
                s1.write(v); // write to s1
                s2.write(v); // write to s2
                std::cout << sc_time_stamp() << ": writer1 writes " << v++ << std::endl;
                wait(1, SC_SEC); // write every 1 s
            }
        }
        void writer2() {
            int v = -1; // init value
            while (true) {
                // s1.write(v); /* cannot, otherwise runtime error: (E115) sc_signal<T> cannot have more than one driver*/
                wait(SC_ZERO_TIME); // needed to offset the write time. Otherwise runtime error: conflicting write in delta cycle 0 
                s2.write(v); // write to s2
                std::cout << sc_time_stamp() << ": writer2 writes " << v-- << std::endl;
                wait(1, SC_SEC); // write every 1 s
            }
        }
        void consumer1() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer1 reads s1=" << s1.read() << "; s2=" << s2.read() << std::endl; // read s1 and s2
                wait(); // wait for s1
            }
        }
        void consumer2() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer2 reads s1=" << s1.read() << "; s2=" << s2.read() << std::endl; // read s1 and s2
                wait(); // wait for s1 or s2
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="22"></a>22.Resolved Signal

A resolved signal is an object of class sc_signal_resolved or class sc_signal_rv. It differs from sc_signal in that a resolved signal may be written by multiple processes, conflicting values being resolved within the channel.
  1. sc_signal_resolved is a predefined primitive channel derived from class sc_signal.
  2. sc_signal_rv is a predefined primitive channel derived from class sc_signal.
    a) sc_signal_rv is similar to sc_signal_resolved.
    b) The difference is that the argument to the base class template sc_signal is type sc_dt::sc_lv<W> instead of type sc_dt::sc_logic.

Class definition:
  1. class sc_signal_resolved: public sc_signal<sc_dt::sc_logic,SC_MANY_WRITERS>
  2. template <int W> class sc_signal_rv: public sc_signal<sc_dt::sc_lv<W>,SC_MANY_WRITERS>

Resolution table for sc_signal_resolved:
|   | 0 | 1 | Z | X |
|---|---|---|---|---|
| 0 | 0 | X | 0 | X |
| 1 | X | 1 | 1 | X |
| Z | 0 | 1 | Z | X |
| X | X | X | X | X |

In short, a resolved signal channel can be writter by multiple processes at the same time. This differs from an sc_signal, which can only be written by one process at each delta cycle.
run

```sh
    SC_MODULE(RESOLVED_SIGNAL) {
        sc_signal_resolved rv; // a resolved signal channel
        vector<sc_logic> levels; // declares a vector of possible 4-level logic values
        SC_CTOR(RESOLVED_SIGNAL) : levels(vector<sc_logic>{sc_logic_0, sc_logic_1, sc_logic_Z, sc_logic_X}){ // init vector for possible 4-level logic values
            SC_THREAD(writer1);
            SC_THREAD(writer2);
            SC_THREAD(consumer);
        }
        void writer1() {
            int idx = 0;
            while (true) {
                rv.write(levels[idx++%4]); // 0,1,Z,X, 0,1,Z,X, 0,1,Z,X, 0,1,Z,X
                wait(1, SC_SEC);            // writes every 1 s
            }
        }
        void writer2() {
            int idx = 0;
            while (true) {
                rv.write(levels[(idx++/4)%4]); // 0,0,0,0, 1,1,1,1, Z,Z,Z,Z, X,X,X,X
                wait(1, SC_SEC); // writes every 1 s
            }
        }
        void consumer() {
            wait(1, SC_SEC); // delay read by 1 s
            int idx = 0;
            while (true) {
                std::cout << " " << rv.read() << " |"; // print the read value (writer1 and writer2 resolved)
                if (++idx % 4 == 0) { std::cout << std::endl; } // print a new line every 4 values
                wait(1, SC_SEC); // read every 1 s
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="23"></a>23.sc_signal <'bool'>

sc_signal_in_if<bool> and sc_signal_in_if<sc_dt::sc_logic> are interfaces that provide additional member functions appropriate for two-valued signals. sc_signal implements these functions:
  1. posedge_event() returns reference to an event that is notified whenever the value of the channel changes and the new value of the channel is true or '1'.
  2. negedge_event() returns a reference to an event that is notified whenever the value of the channel changes and the new value of the channel is false or '0'.
  3. posedge() returns true if and only if the value of the channel changed in the update phase of the immediately preceding delta cycle and at the current simulation time, and the new value of the channel is true or '1'.
  4. negedge() returns true if and only if the value of the channel changed in the update phase of the immediately preceding delta cycle and at the current simulation time, and the new value of the channel is false or '0'.

```sh
    SC_MODULE(SIGNAL_BOOL) {
        sc_signal<bool> b;
        SC_CTOR(SIGNAL_BOOL) {
            SC_THREAD(writer);
            SC_THREAD(consumer);
            sensitive << b; // triggered by every value change
            dont_initialize();
            SC_THREAD(consumer_pos);
            sensitive << b.posedge_event(); // triggered by value change to true
            dont_initialize();
            SC_THREAD(consumer_neg);
            sensitive << b.negedge_event(); // triggered by value change to false
            dont_initialize();
        }
        void writer() {
            bool v = true;
            while (true) {
                b.write(v); // write to channel
                v = !v; // toggle value
                wait(1, SC_SEC); // write every 1 s
            }
        }
        void consumer() {
            while (true) {
                if (b.posedge()) { // if new value is true
                    std::cout << sc_time_stamp() << ": consumer receives posedge, b = " << b << std::endl;
                } else { // if new value is false
                    std::cout << sc_time_stamp() << ": consumer receives negedge, b = " << b << std::endl;
                }
                wait(); // wait for any value change
            }
        }
        void consumer_pos() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer_pos receives posedge, b = " << b << std::endl;
                wait(); // wait for value change to true
            }
        }
        void consumer_neg() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer_neg receives negedge, b = " << b << std::endl;
                wait(); // wait for value change to false
            }
        }
    };

```

## [>>>>>>> Back](#start)
## <a id="24"></a>24.Buffer

sc_buffer is a predefined primitive channel derived from class sc_signal.
It differs from class sc_signal in that a value-changed event is notified whenever the buffer is written rather than only when the value of the signal is changed. 
E.g.,

if current value of a "signal" == 1: write 1 to it won't trigger value update event.

if current value of a "buffer" == 1: write 1 to it will trigger value update event.

```sh
    SC_MODULE(BUFFER) {
        sc_signal<int> s; // declares a signal channel
        sc_buffer<int> b; // declares a buffer channel
        SC_CTOR(BUFFER) {
            SC_THREAD(writer); // writes to both signal and buffer
            SC_THREAD(consumer1);
            sensitive << s; // triggered by signal
            dont_initialize();
            SC_THREAD(consumer2);
            sensitive << b; // triggered by buffer
            dont_initialize();
        }
        void writer() {
            int val = 1; // init value
            while (true) {
                for (int i = 0; i < 2; ++i) { // write same value to channel twice
                    s.write(val); // write to signal
                    b.write(val); // write to buffer
                    wait(1, SC_SEC); // wait after 1 s
                }
                val++; // value change
            }
        }
        void consumer1() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer1 receives " << s.read() << std::endl;
                wait(); // receives from signal
            }
        }
        void consumer2() {
            while (true) {
                std::cout << sc_time_stamp() << ": consumer2 receives " << b.read() << std::endl;
                wait(); // receives from buffer
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="25"></a>25.Communication: port

**Three key concepts for communication:**
  1. Interface:
    a) an abstract class derived from sc_interface but not derived from sc_object.
    b) contains a set of pure virtual functions that shall be defined in one or more channels derived from that interface.
  2. Port:
    a) provides the means by which a module can be written such that it is independent of the context in which it is instantiated.
    b) forwards interface method calls to the channel to which the port is bound.
    c) defines a set of services (as identified by the type of the port) that are required by the module containing the port.
  3. Channel:
    a) sc_prim_channel is the base class for all primitive channels.
    b) channel may provide public member functions that can be called using the interface method call paradigm.
    c) a primitive channel shall implement one or more interfaces.

In short:
  port requires services, interface defines services, channel implements services.
  a port can connect (bind) to a channel if the channel implements the interface that the port requires.
  a port is basically a pointer to a channel.

when to use port:
  1. If a module is to call a member function belonging to a channel that is outside the module itself, that call should be made using an interface method call through a port of the module. Otherwise is considered bad coding style.
  2. However, a call to a member function belonging to a channel instantiated within the current module may be made directly. This is known as portless channel access.
  3. If a module is to call a member function belonging to a channel instance within a child module, that call should be made through an export of the child module.

```sh
    SC_MODULE(MODULE1) { // defines one module
        sc_signal<int> s; // a signal (channel) inside the module
        sc_port<sc_signal_out_if<int> > p; // a port used to write to an outside channel
        SC_CTOR(MODULE1) {
            SC_THREAD(selfWrite); // a process to write to own channel
            SC_THREAD(selfRead); // a process to read from own channel
            sensitive << s; // triggered by value change on the channel
            dont_initialize();
            SC_THREAD(outsideWrite); // a process to write to an outside channel
        }
        void selfWrite() {
            int val = 1; // init value
            while (true) {
                s.write(val++); // write to own channel
                wait(1, SC_SEC); // repeat after 1 s
            }
        }
        void selfRead() {
            while (true) {
                std::cout << sc_time_stamp() << ": reads from own channel, val=" << s.read() << std::endl; // read from own channel
                wait(); // receives from signal
            }
        }
        void outsideWrite() {
            int val = 1; // init value
            while (true) {
                p->write(val++); // write to an outside channel, calls the write method of the outside channel. p is a pointer.
                wait(1, SC_SEC);
            }
        }
        };
        SC_MODULE(MODULE2) { // a module that reads from an outside channel
        sc_port<sc_signal_in_if<int> > p; // a port used to read from an outside channel
        SC_CTOR(MODULE2) {
            SC_THREAD(outsideRead); // a process to read from an outside channel
            sensitive << p; // triggered by value change on the channel
            dont_initialize();
        }
        void outsideRead() {
            while (true) {
                std::cout << sc_time_stamp() << ": reads from outside channel, val=" << p->read() << std::endl; // use port to read from the channel, like a pointer.
                wait(); // receives from port
            }
        }
    };

    /* main.c */
    MODULE1 module1("module1"); // instantiate module1
    MODULE2 module2("module2"); // instantiate module2
    sc_signal<int> s; // declares a signal (channel) outside module1 and moudle2
    module1.p(s); // binds (connects) port p of module1 to channel (signal) s
    module2.p(s); // binds port p of module2 to channel s
    sc_start(2, SC_SEC);

```

## [>>>>>>> Back](#start)
## <a id="26"></a>26.Communication: export

**An export:**
  1. allows a module to provide an interface to its parent module.
  2. forwards interface method calls to the channel to which the export is bound.
  3. defines a set of services that are provided by the module containing the export.

**When to use export:**
  1. Providing an interface through an export is an alternative to a module simply implementing the interface.
  2. The use of an explicit export allows a single module instance to provide multiple interfaces in a structured manner.
  3. If a module is to call a member function belonging to a channel instance within a child module, that call should be made through an export of the child module.

```sh
    SC_MODULE(MODULE1) { // defines one module
        sc_export<sc_signal<int>> p; // an export for other modules to connect
        sc_signal<int> s; // a signal (channel) inside the module. If not using export, the channel need to be defined outside module1.
        SC_CTOR(MODULE1) {
            p(s); // bind an export to an internal channel
            SC_THREAD(writer); // a process to write to an internal channel
        }
        void writer() {
            int val = 1; // init value
            while (true) {
                s.write(val++); // write to an internal channel
                wait(1, SC_SEC);
            }
        }
        };
        SC_MODULE(MODULE2) { // a module that reads from an export
        sc_port<sc_signal_in_if<int>> p; // a port used to read from an export of another module
        SC_CTOR(MODULE2) {
            SC_THREAD(reader); // a process to read from an outside channel
            sensitive << p; // triggered by value change on the channel
            dont_initialize();
        }
        void reader() {
            while (true) {
                std::cout << sc_time_stamp() << ": reads from outside channel, val=" << p->read() << std::endl; // use port to read from the channel, like a pointer.
                wait(); // receives from port
            }
        }
    };
```

## [>>>>>>> Back](#start)
## <a id="27"></a>27.Communication: port 2 port

So far we covered the cases of:
  1. connecting two processes of same module via channel:
      process1() --> channel --> process2() 
  2. connecting two processes of different modules via port and channel:
      module1::process1() --> module1::port1 --> channel --> module2::port2 --> module2::process2()
  3. connecting two processes of different modules via export:
      module1::process1() --> module1::channel --> module1::export1 --> module2::port2 --> module2::process2()

In all these cases, a channel is needed to connect the ports. There is a special case that allows a port to directly connect to a port of submodules. I.e.,
  module::port1 --> module::submodule::port2

```sh
    #include <systemc>
    using namespace sc_core;

    SC_MODULE(SUBMODULE1) { // a submodule that writes to channel
        sc_port<sc_signal_out_if<int>> p;
        SC_CTOR(SUBMODULE1) {
            SC_THREAD(writer);
        }
        void writer() {
            int val = 1; // init value
            while (true) {
            p->write(val++); // write to channel through port
            wait(1, SC_SEC);
            }
        }
        };
        SC_MODULE(SUBMODULE2) { // a submodule that reads from channel
            sc_port<sc_signal_in_if<int>> p;
            SC_CTOR(SUBMODULE2) {
                SC_THREAD(reader);
                sensitive << p; // triggered by value change on the channel
                dont_initialize();
            }
            void reader() {
                while (true) {
                std::cout << sc_time_stamp() << ": reads from channel, val=" << p->read() << std::endl;
                wait(); // receives from channel through port
                }
            }
        };
        SC_MODULE(MODULE1) { // top-level module
            sc_port<sc_signal_out_if<int>> p; // port
            SUBMODULE1 sub1; // declares submodule
            SC_CTOR(MODULE1): sub1("sub1") { // instantiate submodule
                sub1.p(p); // bind submodule's port directly to parent's port
            }
        };
        SC_MODULE(MODULE2) {
            sc_port<sc_signal_in_if<int>> p;
            SUBMODULE2 sub2;
            SC_CTOR(MODULE2): sub2("sub2") {
                sub2.p(p); // bind submodule's port directly to parent's port
        }
    };

    int sc_main(int, char*[]) {
        MODULE1 module1("module1"); // instantiate module1
        MODULE2 module2("module2"); // instantiate module2
        sc_signal<int> s; // define channel outside module1 and module2
        module1.p(s); // bind module1's port to channel, for writing purpose
        module2.p(s); // bind module2's port to channel, for reading purpose
        sc_start(2, SC_SEC);
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="28"></a>28.Communication: specialized ports

Other than using the basic sc_port class to declare ports, there are various specialized port classes to use with different channel types, or provide additional functions:

   1. sc_in: a specialized port class for use with signals.
   2. sc_fifo_in is a specialized port class for use when reading from a fifo.
   3. sc_fifo_out is a specialized port class for use when writing to a fifo.
   4. sc_in<bool> and sc_in<sc_dt::sc_logic>: value_changed(), pos(), neg()
   5. sc_inout is a specialized port class for use with signals: value_changed(), initialize()
   6. sc_inout<bool> and sc_inout<sc_dt::sc_logic> are specialized port classes that provide additional member functions for two-valued signals: value_changed(), initialize(), pos(), neg()
   7. sc_out is derived from class sc_inout and is identical to class sc_inout except for differences inherent in it being a derived class, for example, constructors and assignment operators.
   8. sc_in_resolved is a specialized port class for use with resolved signals. It is similar to sc_in<sc_dt::sc_logic> from which it is derived. The only difference is that a port of class sc_in_resolved shall be bound to a channel of class sc_signal_resolved, whereas a port of class sc_in<sc_dt::sc_logic> may be bound to a sc_signal<sc_dt::sc_logic,WRITER_POLICY> or class sc_signal_resolved.
   9. sc_inout_resolved is a specialized port class for use with resolved signals. It is similar in behavior to port class sc_inout<sc_dt::sc_logic> from which it is derived. The only difference is that a port of class sc_inout_resolved shall be bound to a channel of class sc_signal_resolved, whereas a port of class sc_inout<sc_dt::sc_logic> may be bound to a channel of class sc_signal<sc_dt::sc_logic,WRITER_POLICY> or class sc_signal_resolved.
  10. sc_out_resolved is derived from class sc_inout_resolved, and it is identical to class sc_inout_resolved except for differences inherent in it being a derived class, for example, constructors and assignment operators.
  11. sc_in_rv is a specialized port class for use with resolved signals. It is similar in behavior to port class sc_in<sc_dt::sc_lv<W>> from which it is derived. The only difference is that a port of class sc_in_rv shall be bound to a channel of class sc_signal_rv, whereas a port of class sc_in<sc_dt::sc_lv<W>> may be bound to a channel of class sc_signal<sc_dt::sc_lv<W>,WRITER_POLICY> or class sc_signal_rv.
  12. sc_inout_rv is a specialized port class for use with resolved signals. It is similar in behavior to port class sc_inout<sc_dt::sc_lv<W>> from which it is derived. The only difference is that a port of class sc_inout_rv shall be bound to a channel of class sc_signal_rv, whereas a port of class sc_inout<sc_dt::sc_lv<W>> may be bound to a sc_signal<sc_dt::sc_lv<W>,WRITER_POLICY> or class sc_signal_rv.
  13. sc_out_rv is derived from class sc_inout_rv, and it is identical to class sc_inout_rv except for differences inherent in it being a derived class, for example, constructors and assignment operators. 

A basic sc_port<sc_signal_inout_if<int>> can only access the member functions provided by the signal channel:

   1. read()
   2. write()
   3. default_event() // called when a port is used to define the static sensitivity via operator<< of class sc_sensitive.
   4. event() // check if an event happened, returns true/false
   5. value_changed_event() // event of value change

An sc_port<sc_signal_inout_if<bool>> has access to these additional member functions provided by signal<bool> channel:

   6. posedge() // returns true if value changes from false to true
   7. posedge_event() // event for value changes from false to true
   8. negedge() // returns true if value changes from true to false
   9. negedge_event() // event for value changes from tru to false

A specialized port of sc_inout<> provides additional member functions:

  10. initialize() // initialize value of a port before it's bounded to a channel
  11. value_changed() // used for establishing sensitivity before port is bound to a channel (pointer not initialized)

When the underlining signal channel is of type bool or sc_logic, sc_inout<bool> offers two more member functions:

  12. pos() // establish sensitivity before port is bound
  13. neg() // establish sensitivity before port is bound

In above listed member functions:
  1~9 are offered by the signal channel, accessible via "port->method()";
  10~13 are offered by the specialized port, accessible via "port.method()".

```sh
    SC_MODULE(WRITER) {
        sc_out<bool> p1, p2; // specialized ports
        SC_CTOR(WRITER) {
            SC_THREAD(writer);
            p1.initialize(true); // #10, initialize default value to true
        }
        void writer() {
            bool v = true;
            while (true) {
            p1->write(v); // #2 write through port
            v = !v; // value change
            wait(1, SC_SEC); // repeat after 1 s
            }
        }
    };
    SC_MODULE(READER) {
        sc_in<bool> p1, p2; // specialized ports
        SC_CTOR(READER) {
            SC_THREAD(reader1);
            sensitive << p1 << p2; // #3 default_event(), same as p->default_event() or p.default_event()
            dont_initialize();
            SC_THREAD(reader2);
            sensitive << p1.value_changed(); // #11, sensitive to value change event of an un-bound port
            dont_initialize();
            SC_THREAD(reader3);
            sensitive << p1.neg(); // #13, sensitive to neg event of an un-bound port
            dont_initialize();
            SC_THREAD(reader4);
            sensitive << p1.pos(); // #12, sensitive to pos event of an un-bound port
            dont_initialize();
        }
        void reader1() {
            while (true) {
            std::cout << sc_time_stamp() << ": default_event. p1 = " << p1->read() << "; p1 triggered? " << p1->event() << "; p2 triggered? " << p2->event() << std::endl; // #1 read(), #4 event()
            wait();
            }
        }
        void reader2() {
            while (true) {
            std::cout << sc_time_stamp() << ": value_changed_event. p1 = " << p1->read() <<  std::endl; // #1 read()
            wait();
            }
        }
        void reader3() {
            while (true) {
            std::cout << sc_time_stamp() << ": negedge_event. p1 = " << p1->read() << "; negedge = " << p1->negedge() << std::endl; // #8, if negedge happened
            wait();
            }
        }
        void reader4() {
            while (true) {
            std::cout << sc_time_stamp() << ": posedge_event. p1 = " << p1->read() <<  "; posedge = " << p1->posedge() << std::endl; // #6, if posedge happened
            wait();
            }
        }
    };

    /* Main.cpp */
    int sc_main(int, char*[]) {
        WRITER writer("writer"); // instantiate writer
        READER reader("reader"); // instantiate reader
        sc_signal<bool> b1, b2; // declare boolean signal channel
        writer.p1(b1); // port binding
        writer.p2(b2);
        reader.p1(b1);
        reader.p2(b2);
        sc_start(4, SC_SEC);
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="29"></a>29.Communication: port array

When declarying a port:
  1. the first argument is the name of an interface proper, which is also the type of the port:
    a port can only be bound to a channel derived from the type of the port or to another port or export with a type derived from the type of the port.
  2. the second argument is an optional integer value that specifies the maximum number of channel instances to which the port instance may be bound:
    a) the default value is 1.
    b) if the value is zero, the port may be bound to an arbitrary number of channel instances.
    c) it's an error to bind a port to more channel instances than the number permitted.
  3. the third argument is an optional port policy of type sc_port_policy that determines the rules for binding multiports and the rules for unbound ports:
    a) [default] SC_ONE_OR_MORE_BOUND: the port shall be bound to one or more channels, the maximum number being determined by the value of the second argument. It's an error for the port to remain unbound at the end of elaboration.
    b) SC_ZERO_OR_MORE_BOUND: the port shall be bound to zero or more channels, the maximum number being determined by the value of the second argument. The port may remain unbound at the end of elaboration.
    c) SC_ALL_BOUND: the port shall be bound to exactly the number of channel instances given by value of the second argument, no more and no less, provided that value is greater than zero.
      1) If the value of the second argument is zero, policy SC_ALL_BOUND has the same meaning as policy SC_ONE_OR_MORE_BOUND.
      2) It's an error for the port to remain unbound at the end of elaboration, or to be bound to fewer channel than the number required by the second argument.

It's an error to bind a given port to a given channel more than once, whether directly or through another port.

Another way to define port array is using the C/C++ array syntax: sc_port<IF> p[10] or vector<sc_port<IF>> p(10);

Example:
  1. sc_port<IF>                         // Bound to exactly 1 channel instance
  2. sc_port<IF,0>                       // Bound to 1 or more channel instances, with no upper limit
  3. sc_port<IF,3>                       // Bound to 1, 2, or 3 channel instances
  4. sc_port<IF,0,SC_ZERO_OR_MORE_BOUND> // Bound to 0 or more channel instances, with no upper limit
  5. sc_port<IF,1,SC_ZERO_OR_MORE_BOUND> // Bound to 0 or 1 channel instances
  6. sc_port<IF,3,SC_ZERO_OR_MORE_BOUND> // Bound to 0, 1, 2, or 3 channel instances
  7. sc_port<IF,3,SC_ALL_BOUND>          // Bound to exactly 3 channel instances
  8. sc_port<IF, 3>                      // an array of 3 ports, each binds to exactly 1 channel instance
  9. vector<sc_port<IF>> p(3)            // an array of 3 ports, each binds to exactly 1 channel instance

```sh

    SC_MODULE(WRITER) {
        sc_port<sc_signal_out_if<int>> p1; // #1: exactly 1 channel
        sc_port<sc_signal_out_if<int>, 0> p2; // #2: 1 or more channels, no upper limit
        sc_port<sc_signal_out_if<int>, 3> p3; // #3: 1, 2, or 3 channels
        sc_port<sc_signal_out_if<int>, 0, SC_ZERO_OR_MORE_BOUND> p4; // #4: 0 or more channels, no upper limit
        sc_port<sc_signal_out_if<int>, 1, SC_ZERO_OR_MORE_BOUND> p5; // #5: 0 or 1 channels
        sc_port<sc_signal_out_if<int>, 3, SC_ZERO_OR_MORE_BOUND> p6; // #6: 0, 1, 2, or 3 channels
        sc_port<sc_signal_out_if<int>, 3, SC_ALL_BOUND> p7; // #7: exactly 3 channels
        std::vector<sc_port<sc_signal_out_if<int>>> p9; // #9: vector of port
        SC_CTOR(WRITER) : p9(3) { // init p9 to size of 3
            SC_THREAD(writer);
        }
        void writer() {
            int v = 1;
            while (true) {
            p9[0]->write(v); // write to p9[0]
            p7[1]->write(v++); // write to p7[1]
            wait(1, SC_SEC);
            }
        }
    };
    SC_MODULE(READER) {
        sc_port<sc_signal_in_if<int>> p1; // #1: exactly 1 channel
        sc_port<sc_signal_in_if<int>, 0> p2; // #2: 1 or more channels, no upper limit
        sc_port<sc_signal_in_if<int>, 3> p3; // #3: 1, 2, or 3 channels
        sc_port<sc_signal_in_if<int>, 0, SC_ZERO_OR_MORE_BOUND> p4; // #4: 0 or more channels, no upper limit
        sc_port<sc_signal_in_if<int>, 1, SC_ZERO_OR_MORE_BOUND> p5; // #5: 0 or 1 channels
        sc_port<sc_signal_in_if<int>, 3, SC_ZERO_OR_MORE_BOUND> p6; // #6: 0, 1, 2, or 3 channels
        sc_port<sc_signal_in_if<int>, 3, SC_ALL_BOUND> p7; // #7: exactly 3 channels
        std::vector<sc_port<sc_signal_in_if<int>>> p9; // #9: exactly 3 channels
        SC_CTOR(READER) : p9(3) { // init p9 to size of 3
            SC_THREAD(reader7);
            sensitive << p7; // sensitive to any element of port array p7
            dont_initialize();
            SC_THREAD(reader9);
            sensitive << p9[0] << p9[1] << p9[2]; // sensitive to any element of port array p9
            dont_initialize();
        }
        void reader7() {
            while (true) {
                std::cout << sc_time_stamp() << "; reader7, port 0/1/2 = " << p7[0]->read() << "/" << p7[1]->read() << "/" << p7[2]->read() << std::endl;
                wait();
            }
        }
        void reader9() {
            while (true) {
                std::cout << sc_time_stamp() << "; reader9, port 0/1/2 = " << p9[0]->read() << "/" << p9[1]->read() << "/" << p9[2]->read() << std::endl;
                wait();
            }
        }
    };

    int sc_main(int, char*[]) {
        WRITER writer("writer"); // instantiate writer
        READER reader("reader"); // instantiate reader
        // declare channels
        sc_signal<int> s1; // 1 channel
        std::vector<sc_signal<int>> s2(10); // 10 channels
        std::vector<sc_signal<int>> s3(2); // 2 channel
        // leave s4 un-bound
        sc_signal<int> s5; // 1 channel
        std::vector<sc_signal<int>> s6(2); // 2 channels
        std::vector<sc_signal<int>> s7(3); // 3 channels
        // #8 is same as #9, omitted
        std::vector<sc_signal<int>> s9(3); // 3 channels
        // bind ports
        writer.p1(s1); // #1
        reader.p1(s1); // #1
        for (unsigned int i = 0; i < s2.size(); ++i) { // #2
            writer.p2(s2[i]);
            reader.p2(s2[i]);
        }
        for (unsigned int i = 0; i < s3.size(); ++i) { // #3
            writer.p3(s3[i]);
            reader.p3(s3[i]);
        }
        // s4 un-bound
        writer.p5(s5); // #5
        reader.p5(s5); // #5
        for (unsigned int i = 0; i < s6.size(); ++i) { // #6
            writer.p6(s6[i]);
            reader.p6(s6[i]);
        }
        for (unsigned int i = 0; i < s7.size(); ++i) { // #7
            writer.p7(s7[i]);
            reader.p7(s7[i]);
        }
        for (unsigned int i = 0; i < s9.size(); ++i) { // #9
            writer.p9[i](s9[i]);
            reader.p9[i](s9[i]);
        }
        sc_start(2, SC_SEC);
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="30"></a>30.Primitive Channel

sc_prim_channel:
  1. is the base class for all primitive channels
  2. provides primitive channels with unique access to the update phase of the scheduler.
  3. contains no hierachy, ports, or simulation processes.
  4. in common with hierarchical channels, a primitive channel may provide public member functions that can be called using the interface method call paradigm.
  provides the following member functions:
    a) request_update(): scheduler to queue an update request for the channel
    b) async_request_update():
      1) scheduler to queue an update request for the channel in a thread-safe manner. May be called reliably from an operating system thread other than systemC kernel.
      2) not recommended to be called from functions executed in the context of the systemC kernel
    c) update():
      1) called back by the scheduler during the udpate phase in response to a call to request_update or async_request_update.
      2) an application may override this member function. The definition of this function in sc_prim_channel itself does nothing.
      3) typically only read and modify data members of the current object and create delta notifications.
      4) shall not:
        a) call any member function of class sc_prim_channel with the exception of member function update itself if overridden within a base class of the current object
        b) call member function notify() of class sc_event with no arguments to create an immediate notification
        c) call any of the member functions of class sc_process_handle for process control (suspend or kill, for example)
        d) change the state of any storage except for data members of the current object.
        e) read the state of any primitive channel instance other than the current object.
        f) call interface methods of other channel instances. In particular, member function update should not write to any signals.
    d) next_trigger()
    e) wait()

A channel shall implement one or more interface, thus needs to inherit from an interface class (of base class type sc_interface). The interface provides the required methods for the channel.

```sh

    #include <systemc>
    #include <string>
    using namespace sc_core;

    class GENERATOR_IF : public sc_interface { // interface for interrupt generator
        public:
        virtual void notify() = 0;
    };

    class RECEIVER_IF : public sc_interface { // interface for interrupt receiver
        public:
        virtual const sc_event& default_event() const = 0; // needed for sensitive
    };

    class INTERRUPT : public sc_prim_channel, public GENERATOR_IF, public RECEIVER_IF { // interrupt class
        public:
        INTERRUPT(sc_module_name name) : sc_prim_channel(name) {} // constructor, construct sc_prim_channel
        void notify() { // implement GENERATOR_IF
            e.notify();
        }
        const sc_event& default_event() const { // implement RECEIVER_IF
            return e;
        }
        private:
        sc_event e; // private event for synchronization
    };

    SC_MODULE(GENERATOR) { // interrupt generator class
    sc_port<GENERATOR_IF> p; // port to generate interrupt
    SC_CTOR(GENERATOR) { // constructor
        SC_THREAD(gen_interrupt);
    }
    void gen_interrupt() {
        while (true) {
        p->notify(); // calls notify function of the INTERRUPT channel
        wait(1, SC_SEC);
        }
    }
    };
    SC_MODULE(RECEIVER) { // interrupt receiver class
    sc_port<RECEIVER_IF> p; // port to receive interrupt
    
    SC_CTOR(RECEIVER) { // constructor
        SC_THREAD(rcv_interrupt);
        sensitive << p; // monitors interrupt on port p
        dont_initialize();
    }
    void rcv_interrupt() { // triggered upon interrupt
        while (true) {
        std::cout << sc_time_stamp() << ": interrupt received" << std::endl;
        wait();
        }
    }
    };

    int sc_main(int, char*[]) {
        GENERATOR generator("generator"); // instantiate generator
        RECEIVER receiver("receiver"); // instantiate receiver
        INTERRUPT interrupt("interrupt"); // instantiate interrupt
        generator.p(interrupt); // port binding
        receiver.p(interrupt); // port binding
        sc_start(2, SC_SEC);
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="31"></a>31.Hierarchical Channel

A hierarchical channel:
  1. shall inherit the sc_channel base class, which is, identical to sc_module. Thus, a hierarchical channel is a systemC module.
  2. shall inherit from an interface, to let it connect to a port.
Like a regular systemC module, a hierarchical channel may have simulation processes, ports etc.

This example shows a customized hierarchical channel that implements the sc_signal_inout_if<int>. As defined by sc_signal_inout_if, we have to implement the following functions:
  1. void write(const int&)
  2. const int& read() const
  3. const sc_event& value_changed_event() const
  4. const sc_event& default_event() const
  5. const int& get_data_ref() const
  6. bool event() const

```sh
    #include <systemc>
    using namespace sc_core;

    // this is a simple implementation as compared to sc_signal, just to illustrate the concept of a hieracical channel
        class SIGNAL : public sc_channel, public sc_signal_inout_if<int> { // declares SIGNAL channel, inherits from sc_chanel and signal_inout_if<int>
        public:
            SC_HAS_PROCESS(SIGNAL);
            SIGNAL(sc_module_name name = sc_gen_unique_name("SIG")) : sc_channel(name) {} // constructor, construct base class
            void write(const int& v) { // implements write method
                if (v != m_val) { // update only if value is new
                m_val = v; // update value
                e.notify(); // trigger event
                }
            }
            const int& read() const {
                return m_val;
            }
            const sc_event& value_changed_event() const {
                return e; // return reference to the event
            }
            const sc_event& default_event() const {
                return value_changed_event(); // allows used in static sensitivity list
            }
            const int& get_data_ref() const {
                return m_val;
            }
            bool event() const {
                return true; // dummy implementation, always return true
            }
        private:
            int m_val = 0;
            sc_event e;
        };

        SC_MODULE(TEST) { // a test class
        SIGNAL s; // declares SIGNAL channel
        SC_CTOR(TEST) { // no name provided to s, use default
            SC_THREAD(writer); // register a writer process
            SC_THREAD(reader); // register a reader process
            sensitive << s; // use SIGNAL channel in static sensitivity list
            dont_initialize();
        }
        void writer() {
            int v = 1;
            while (true) {
            s.write(v++); // write to channel
            wait(1, SC_SEC);
            }
        }
        void reader() {
            while (true) {
            std::cout << sc_time_stamp() << ": val = " << s.read() << std::endl; // read from channel
            wait();
            }
        }
    };
    int sc_main(int, char*[]) {
        TEST test("test"); // instantiate generator
        sc_start(2, SC_SEC);
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="32"></a>32.Trace File

A trace file:
  1. records a time-ordered sequence of value changes during simulation.
  2. uses VCD (Value change dump) file format.
  3. can only be created and opened by sc_create_vcd_trace_file.
  4. may be opened during elaboration or at any time during simulation.
  5. contains values that can only be traced by sc_trace.
  6. shall be opened before values can be traced to that file, and values shall not be traced to a given trace file if one or more delta cycles have elapsed since opening the file.
  7. shall be closed by sc_close_vcd_trace_file. A trace file shall not be closed before the final delta cycle of simulation.
run

```sh
    #include <systemc>
    using namespace sc_core;

    SC_MODULE(MODULE) { // a module write to a channel
        sc_port<sc_signal<int>> p; // a port
        SC_CTOR(MODULE) {
            SC_THREAD(writer); // a writer process
        }
        void writer() {
            int v = 1;
            while (true) {
            p->write(v++); // write to channel via port
            wait(1, SC_SEC); // write every 1 s
            }
        }
    };

    int sc_main(int, char*[]) {
        MODULE module("module"); // instantiate module
        sc_signal<int> s; // declares signal channel
        module.p(s); // bind port to channel

        sc_trace_file* file = sc_create_vcd_trace_file("trace"); // open trace file
        sc_trace(file, s, "signal"); // trace "s" under the name of "signal"
        sc_start(5, SC_SEC); // run simulation for 5 s
        sc_close_vcd_trace_file(file); // close trace file
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="33"></a>33.Error and Message Report

sc_report:
  1. represents an instance of a report as generated by function sc_report_handler::report.
  2. accessible to the application if the action SC_CACHE_REPORT is set for a given severity level and message type.
  3. may be caught by the application when thrown by the report handler.

sc_report_handler:
  provides features for writing out textual reports on the occurrence of exceptional circumstances and for defining application-specific behavior to be executed when those reports are generated.

sc_severity represents the severity level of a report:
  1. enum sc_severity {SC_INFO = 0, SC_WARNING, SC_ERROR, SC_FATAL, SC_MAX_SEVERITY};
  2. there are four severity levels. SC_MAX_SEVERITY is not a severity level. It's an error to pass SC_MAX_SEVERITY to a function requiring an argument of type sc_severity.

sc_verbosity provides the values of indicative verbosity levels that may be passed as arguments to member function set_verbosity_level of class sc_report_handler and to member function report of class sc_report_handler:
  enum sc_verbosity {SC_NONE = 0, SC_LOW = 100, SC_MEDIUM = 200, SC_HIGH = 300, SC_FULL = 400, SC_DEBUG = 500};

sc_actions represents a word where each bit in the word represents a distinct action. More than one bit may be set, in which case all of the corresponding actions shall be executed:
  1. enum {
     SC_UNSPECIFIED  = 0x0000, //is not an action, serves as the default value meaning that no action has been set.
     SC_DO_NOTHING   = 0x0001, // is a specified action
     SC_THROW        = 0x0002,
     SC_LOG          = 0x0004,
     SC_DISPLAY      = 0x0008,
     SC_CACHE_REPORT = 0x0010,
     SC_INTERRUPT    = 0x0020,
     SC_STOP         = 0x0040,
     SC_ABORT        = 0x0080
    }
  2. each severity level is associated with a set of default actions, which can be overridden by calling member function set_actions.
  3, Default actions:
    a) #define SC_DEFAULT_INFO_ACTIONS ( SC_LOG | SC_DISPLAY )
    b) #define SC_DEFAULT_WARNING_ACTIONS ( SC_LOG | SC_DISPLAY )
    c) #define SC_DEFAULT_ERROR_ACTIONS ( SC_LOG | SC_CACHE_REPORT | SC_THROW )
    d) #define SC_DEFAULT_FATAL_ACTIONS ( SC_LOG | SC_DISPLAY | SC_CACHE_REPORT | SC_ABORT )

void report(sc_severity, const char* msg_type, const char* msg, [int verbosity], const char* file, int line) generate a report and cause the appropriate actions to be taken:
  1. use the severity passed as the first argument and the message type passed as the second argument to determine the set of actions to be executed as a result of previous calls to functions set_actions, stop_after, suppress, and force.
  2. create an object of class sc_report initialized using all five argument values and pass this object to the handler set by the member function set_handler.
  3. not persist beyond the call to member function report unless the action SC_CACHE_REPORT is set, in which case the object can be retrieved by calling function get_cached_reports.
  4. be responsible for determining the set of actions to be executed. The handler function set by function set_handler is responsible for executing those actions.
  5. maintain counts of the number of reports generated. These counts shall be incremented regardless of whether actions are executed or suppressed, except where reports are ignored due to their verbosity level, in which case the counts shall not be incremented.

set_actions():
  1. set the actions to be taken by member function report when report is called with the given severity level, message type, or both.
  2. replace the actions set by the previous call for the given severity, message type, or severity-message type pair.

stop_after(): report shall call sc_stop when exactly the number of reports given by argument limit to function stop_after have been generated for the given severity level, message type, or severity-message type pair.

get_count(): return the count of the number of reports generated for each severity level, each message type, and each severity-message type pair maintained by member function report.

Verbosity level:
  1. int set_verbosity_level(int): set the maximum verbosity level to the value passed as an argument, return the previous value of the maximum verbosity level.
  2. int get_verbosity_level(): return the value of the maximum verbosity level.

```sh
    #include <systemc>
    using namespace sc_core;

    SC_MODULE(MODULE) { // a test module
        sc_port<sc_signal<int>> p; // a port
        SC_CTOR(MODULE) { // constructor
            SC_REPORT_WARNING("ctor", "register function"); // gen report to "ctor"
            SC_THREAD(writer); // a writer process
            SC_THREAD(reader); // a reader process
            sensitive << p; // sensitive to p
            dont_initialize();
        }
        void writer() {
            int v = 1;
            while (true) {
            SC_REPORT_INFO("writer", ("write " + std::to_string(v)).c_str()); // gen report to "writer"
            p->write(v++); // write to channel via port
            wait(1, SC_SEC); // write every 1 s
            }
        }
        void reader() {
            while (true) {
            SC_REPORT_INFO("reader", ("read " + std::to_string(p->read())).c_str()); // gen report to "reader"
            wait();
            }
        }
    };

    int sc_main(int, char*[]) {
        sc_report_handler::set_log_file_name("report.log"); // initialize report
        sc_report_handler::set_actions("writer", SC_INFO, SC_LOG); // INFO of "writer" saved in log, no display

        MODULE module("module"); // instantiate module
        sc_signal<int> s; // declares signal channel
        module.p(s); // bind port to channel

        SC_REPORT_INFO("main", "simulation starts"); // gen report to "main"
        sc_start(2, SC_SEC); // run simulation for 2 s
        SC_REPORT_INFO("main", "simulation ends"); // gen report to "main"
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="34"></a>34.Customized Data Type

sc_signal<T> and sc_fifo<T> can be used with various data types. SystemC has already supported T for build-in data types.
In order to use customized data types for sc_signal and sc_fifo, the following member functions shall be implemented for the data type:
  1. assignment operator, i.e. operator=(): needed for read and write methods
  2. equality operator, i.e. operator==(): needed by sc_signal for value_changed_event()
  3. streaming output, i.e. ostream& operator<<(): needed to print the data structure
  4. sc_trace(): allow the data type to be used with the systemC trace facility; enables viewing traced data with a wavefor viewer.

```sh
    #include <systemc>
    #include <ostream>
    using namespace sc_core;

    struct CUSTOMIZED_TYPE {
        int x, y; // member variables
        CUSTOMIZED_TYPE(int x = 0, int y = 0) : x(x), y(y) {} // constructor
        CUSTOMIZED_TYPE& operator=(const CUSTOMIZED_TYPE& rhs) { // assignment operator, needed for read() write()
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
        bool operator==(const CUSTOMIZED_TYPE& rhs) { // equality operator, needed for value_changed_event()
            return x == rhs.x && y == rhs.y;
        }
        };
        std::ostream& operator<<(std::ostream& os, const CUSTOMIZED_TYPE& val) { // streaming output, needed for printing
            os << "x = " << val.x << "; y = " << val.y << std::endl;
            return os;
        }
        inline void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) { // needed for tracing
            sc_trace(f, val.x, name + ".x");
            sc_trace(f, val.y, name + ".y");
        }

        SC_MODULE(MODULE) { // test module
        sc_signal<CUSTOMIZED_TYPE> s; // customized signal
        SC_CTOR(MODULE) { // constructor
            SC_THREAD(writer); // writer process
            SC_THREAD(reader); // reader process
            sensitive << s; // sensitive to customized signal s
            dont_initialize();
        }
        void writer() {
            int x = 1; // init signal
            int y = 2;
            while (true) {
            s.write(CUSTOMIZED_TYPE{x++, y++}); // write to signal
            wait(1, SC_SEC); // wait 1 s
            }
        }
        void reader() {
            while (true) {
            std::cout << sc_time_stamp() << ": receives " << s.read() << std::endl; // read from signal
            wait(); // wait for value_changed_event
            }
        }
    };

    int sc_main(int, char*[]) {
        MODULE module("module"); // instantiate module
        sc_trace_file* file = sc_create_vcd_trace_file("trace"); // open trace file
        sc_trace(file, module.s, "customized_type"); // trace customized signal
        sc_start(2, SC_SEC); // run simulation for 2 s
        sc_close_vcd_trace_file(file); // close trace file
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="35"></a>35.Clock

sc_clock s a predefined primitive channel derived from the class sc_signal and intended to model the behavior of a digital clock signal. 
The value and events associated with the clock are accessed through the interface sc_signal_in_if<bool>.

Constructor:
sc_clock(

  - constchar*name_, // unique module name
  - double period_v_, // the time interval between two consecutive transitions from false to true, also equal to the - time interval between two consecutive transitions from true to false. Greater than zero, default is 1 nanosecond.
  - sc_time_unit period_tu_, // time unit, used for period
  - double duty_cycle_, // the proportion of the period during which the clock has the value true. Between 0.0 and 1.0, exclusive. Default is 0.5.
  - double start_time_v_, // the absolute time of the first transition of the value of the clock (false to true or true to false). Default is zero.
  - sc_time_unit start_time_tu_,
  - bool posedge_first_ = true ); // if true, the clock is initialized to false, and changes to true at the start time. Vice versa. Default is true.

```sh
    #include <systemc>
    using namespace sc_core;

    SC_MODULE(CLOCK) {
        sc_port<sc_signal_in_if<bool>> clk; // a port to access clock
        SC_CTOR(CLOCK) {
            SC_THREAD(thread); // register a thread process
            sensitive << clk; // sensitive to clock
            dont_initialize();
        }
        void thread() {
            while (true) {
            std::cout << sc_time_stamp() << ", value = " << clk->read() << std::endl; // print current clock value
            wait(); // wait for next clock value change
            }
        }
    };

    int sc_main(int, char*[]) {
        sc_clock clk("clk", 10, SC_SEC, 0.2, 10, SC_SEC, false); // 10s period, 2s true, 8s false, start at 10s, start at false.
        CLOCK clock("clock"); // instantiate module
        clock.clk(clk); // bind port
        sc_start(31, SC_SEC); // run simulation for 31 s
        return 0;
    }
```

## [>>>>>>> Back](#start)
## <a id="36"></a>36.Process: CTHREAD

SC_CTHREAD:
  1. is depcrecated in SystemC 2.0. It is still supported for the case where the second argument is an event finder.
  2. requires a clock when registering a process.
  3. has no separate sensitivity list as SC_METHOD or SC_THREAD.
  4. is activated whenever the specified clock edge occurs.

```sh
    #include <systemc>
    using namespace sc_core;

    SC_MODULE(MODULE) {
        sc_in<bool> clk; // need event_finder method, cannot use basic sc_port
        SC_CTOR(MODULE) {
            SC_CTHREAD(cthread1, clk); // sensitive to clk pos edge
            // no static sensitivity, thus, cannot use dont_initialize()
            SC_CTHREAD(cthread2, clk.pos()); // sensitive to clk pos edge
            SC_CTHREAD(cthread3, clk.neg()); // sensitive to clk neg edge
        }
        void cthread1() {
            while (true) {
            wait(); // wait for clk pos edge; wait() right after while loop to avoid initialization
            std::cout << sc_time_stamp() << ", cthread1, value = " << clk->read() << std::endl;
            }
        }
        void cthread2() {
            while (true) {
            wait(); // wait for clk pos edge
            std::cout << sc_time_stamp() << ", cthread2, value = " << clk->read() << std::endl;
            }
        }
        void cthread3() {
            while (true) {
            wait(); // wait for clk neg edge
            std::cout << sc_time_stamp() << ", cthread3, value = " << clk->read() << std::endl;
            }
        }
    };

    int sc_main(int, char*[]) {
        sc_clock clk("clk", 10, SC_SEC, 0.2, 10, SC_SEC, false); // 10s period, 2s true, 8s false, start at 10s, start at false.
        MODULE module("module"); // instantiate module
        module.clk(clk); // bind port
        sc_start(31, SC_SEC); // run simulation for 31 s
        return 0;
    }
```