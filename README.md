# AsyncCommand
C++11 based AsyncCommandClass, which facilitates sending commands and receiving response asynchronously between threads. 
This is a header only file, just include AsyncCommandClass.h in your project to use this class. There are no other dependencies, all you
need is c++11 compiler and you are good to go.

This Class is a simple wrapper around c++11 packaged_task. This class takes in a command type and response type as its template arguments
and creates a object which is initialized with a packaged_task along with a command object. when the command receiver thread needs to
send back the response to the command issuer thread it just calls the packaged_task. This packaged_task then returns the response object
in the form of std::future. 

The source code is sufficiently documented and there is a main.cpp which provides a sample usage of this class. To complile this main.cpp
you will need boost::lockfree::spsc_que. Because the commands are pushed to another thread using this que. once you have boost installed 
then this main.cpp can be easily compiled and tested. This main.cpp file is also well document so that one can easily understand the usage
of this class.

For further queries contact me at selvakumarjawahar@gmail.com



