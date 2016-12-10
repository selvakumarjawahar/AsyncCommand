#pragma once
#include<future>
#include<memory>
#include<functional>

/*!
	This templated class takes 2 template parameters. One is the command type and other one is Response type
	Both these types can be a user defined types or can be inbult types. The thread which is sending the command
	should first get the future by calling getFuture() method. The receiver thread can access the command 
	by accessing the Command Variable. The receiver thread can return the result of command execution by calling
	the operator(). The sending thread can then get this result from the future which it acquired earlier
*/
template<typename CommandType,typename ResponseType> 
class AsyncCommandClass{
public:
	/*!
		The constructor takes command and intializes its internal command object.
		sender thread creates this object by calling this constructor with command
		it wants to send to the receiver thread.
	*/
	explicit AsyncCommandClass(CommandType&& cmd) :
		ComputeResult_{ [](ResponseType&& response) {return response;} }, Command{ cmd } {
		}

	/*!
		command sender thread should call this before sending the command
	*/
	std::future<ResponseType> getFuture() { 
		return ComputeResult_.get_future();
	}

	/*!
		command receiver thread should call this to return the result of command execution back to command sender		
	*/
	void sendResponse(ResponseType&& Response) { 
		ComputeResult_(std::move(Response));
	}
	CommandType Command; //! this represents actual command
private:
	std::packaged_task<ResponseType(ResponseType&&)> ComputeResult_;
};

