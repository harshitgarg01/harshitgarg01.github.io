# Fault-Tolerance

## First Model 

# Overview 

The purpose of our application is to take in a unique identifier every 1 second and invoke each of the 6 methods with this identifier, until a true value is returned. The application uses retry logic (exponential back off) to solve the problem. The program runs across multiple threads which are synchronized properly to avoid any errors. It consists of a main thread that splits into 6 extra threads where each thread runs a particular method. Each sub thread is then split into 10 more threads to run for different identifiers at the same time. Along with that a few separate threads are also created to control the parameters of methods according to the program need. Below are the list of functions and classes that help run the application effectively. 

 

## Functions 

# main function 

The main function begins with calling below listed functions on new threads like calling 6 threads for function retryLimitingHandler , 6 threads for particularMethodExecutor where each thread works for a particular method from 1 to 6. Next it inputs data from files -> DegradedMode.txt and InputFile.txt. Then it starts adding one identifier after 1 sec to global vector variable identifierStorage. After finishing with inputs, it begins to join all the new threads which were created to the main thread. Then it starts writing the desired statistics in a csv file which include identifier name, identifier completion time, number of attempts, time at which each attempt is called and total time taken for that attempt. This is done for all methods and identifier and stored in the csv file for further plotting. 

retryLimitingHandler function 

The aim of this function is to set retryLimit to 0 after every 1 second. This helps us in maintaining the desired 2 true values per second property of the normal mode. It takes in index as an argument to know which methods retryLimit it has to work on. It works on a separate thread from rest of the program. 

setDegradationTime function 

This function is used to set the degradation time for a particular method according to the need of the application. It sets this time randomly in the run time of program and it too works on a separate thread from the rest of the program. 

stringToInt function 

This method is used to convert a string of numbers into integers. 

particularMethodExecutor function 

This function contains ten threads which could be activated to call randomSeedSetUp as soon as the application receives identifier input. It consists of two arrays named availableThread and threadClosure which help in initiating and joining these ten threads to the current function thread. Thus, this function is used to run 10 identifiers at a time for a method to speed up the performance in case of a backlog by degradation. It implements a particular method on its thread based on the index provided to it by the main function. 

randomSeedSetUp function 

This method is used to generate a specific seed for each thread before going to retryLogicImplementation function to prevent different threads from getting the same values from rand () and hence leading to the desired randomness. 

retryLogicImplementation function 

This function applies the exponential back off retry logic for a particular identifier for a particular method. This is where we use the global 2d vector of timeStorage class to get the useful metric time values of the program that could later be used for statistical analysis. 

Classes 

Method1, Method2, Method3, Method4, Method5, Method6 

All these classes are similar whose main motive is to call the modeGuesser method(public), which on taking the identifier returns either true or false. Internally it has two private methods which are called normalMode and degradedMode and modeGuesser calls them randomly using the rand () function. Other desired properties which are needed to be satisfied by these classes are taken in care by an instance of MethodParameters. 

TimeStorage 

This class is basically used to store the completion time of identifier, starting and ending time for each attempt for an identifier for all methods. It uses chrono library to store time in a format that is easily convertible to seconds later on. This class also stores the number of attempts it took for a particular identifier to successfully compete the method. 

MethodParameters 

This class consist of attributes in form of 3 arrays of length 6 storing state, mode and retryLimit for each method. This class aim is to change these parameters of Methods according to the requirement of the program. Thus, it has its required getters and setters. Also, it contains a mutex which helps in synchronized execution of class across multiple threads. 

 

 

Second Model 

Overview 

The purpose of our application is to take in a unique identifier every 1 second and invoke each of the 6 methods with this identifier, until a true value is returned. The application uses retry logic (exponential back off) to solve the problem.  The program though runs across multiple threads but they are considerably limited as compared to first model. The main architecture here involves just division of main thread (specifically thread running the identifierDistributor function) into 10 extra different threads. Each thread implements a unique identifier but here it implements all the methods across the single thread in a sequential manner instead of having separate threads for each method. This helps in extending our application to dependent methods (example method2 can be only implemented after method1 for an identifier) and limiting our thread pool. 

Functions 

main function 

The main functions main purpose is to take inputs from files like DegradedMode.txt and InputFile.txt. After that it calls the identifierDistributor function across a new thread and then begins to provide a unique identifier every second via a global vector. Besides that, after implementation of retry logic it is used to store the necessary statistics in csv as done in first model. 

 identifierDistributor function 

This function is called by the main function along a different thread. This function is basically used to create and maintain 10 threads with each thread acts all the methods for a particular identifier. Thus, it contains arrays like threadClosure and availableThread to maintain the thread pool. Besides this at the start it also calls retryLimitingHandler function and setDegradationTime function along different threads like the main function did in the First Model to maintain the desired properties of methods. 

allMethodExecutor function 

This function calls all the methods in a sequential order and hence ends up acting like an extension to the problem statement where we can also work with methods which are dependent on each other. Thus, it ends up calling retryLogicImplementation function in a sequential order according to dependency of the methods. No separate threads are created for each function. 

retryLimitingHandler function 

The aim of this function is to set retryLimit to 0 after every 1 second. This helps us in maintaining the desired 2 true values per second property of the normal mode. It takes in index as an argument to know which method retryLimit it has to work on. It works on a separate thread from rest of the program. 

setDegradationTime function 

This function is used to set the degradation time for a particular method according to the need of the application. It sets this time randomly in the run time of program and it too works on a separate thread from the rest of the program. 

stringToInt function 

This method is used to convert a string of numbers into integers. 

randomSeedSetUp function 

This method is used to generate a specific seed for each thread before going to allMethodExecutor function to prevent different threads from getting the same values from rand () and hence leading to the desired randomness. 

retryLogicImplementation function 

This function applies the exponential back off retry logic for a particular identifier for a particular method. This is where we use the global 2d vector of timeStorage class to get the useful metric time values of the program that could later be used for statistical analysis. 

Classes 

Method1, Method2, Method3, Method4, Method5, Method6 

All these classes are similar whose main motive is to call the modeGuesser method(public), which on taking the identifier returns either true or false. Internally it has two private methods which are called normalMode and degradedMode and modeGuesser calls them randomly using the rand () function. Other desired properties which are needed to be satisfied by these classes are taken in care by an instance of MethodParameters. 

TimeStorage 

This class is basically used to store the time of completion of identifier, starting and ending time for each attempt for an identifier for all methods. It uses chrono library to store time in a format that is easily convertible to seconds later on. This class also stores the number of attempts it took for a particular identifier to successfully compete the method. 

MethodParameters 

This class consist of attributes in form of 3 arrays of length 6 storing state, mode and retryLimit for each method. This class aim is to change these parameters of Methods according to the requirement of the program. Thus, it has its required getters and setters. Also, it contains a mutex which helps in synchronized execution of class across multiple threads. 

Third Model 

Overview 

It has exactly similar setup as the first model just it is extended to incorporate dependent method nature. This is achieved by using a global vector identifierPassed where the following vector restrict the threads for a certain method to work before completion of its dependent method. 
