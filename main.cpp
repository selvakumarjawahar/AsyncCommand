#include<iostream>
#include<thread>
#include<memory>

#include<boost\lockfree\spsc_queue.hpp>

#include"AsyncCommandClass.h"
/*!
	Some Typedefs
*/

/*!
	This typedef is creating a async command type, where command is of type int and its reponse is bool
*/
typedef AsyncCommandClass<int, bool> INT_CMD;

/*!
	As Asynccommand is non copyable, and boost lockfree spsc que requires a copyable type, use shared pointers
	to pass around the command objects between the threads.
*/
typedef std::shared_ptr<INT_CMD> CMDPTR;

/*!
	This typedef defines the boost SPSC QUE which takes in shared pointer object of the command.
	main thread will push commands in this que and service thread will receive the commands from this
	que
*/
typedef boost::lockfree::spsc_queue<CMDPTR> CMD_QUE;


/*!
	This is a simple test service which receives, process and responds to commands
*/
void MyTestService(CMD_QUE& cmdq) {
	while (true) {
		if (cmdq.read_available()) {
			auto cmd = cmdq.front();
			if (cmd->Command > 100)
				cmd->sendResponse(true);
			else if (cmd->Command < 0) {
				cmd->sendResponse(true);
				break;
			}
			else
				cmd->sendResponse(false);
			cmdq.pop();
		}
	}
}

int main() {
	CMD_QUE cmdq{ 10 }; //!create command que which can hold 10 commandptr objects
	auto th1 = std::thread{ MyTestService,std::ref(cmdq) }; //! Launch the service thread
	
	INT_CMD command1{ 10 }; //!create command with value 10
	auto command1_result = command1.getFuture(); //!get the result of the command as future
	cmdq.push(std::make_shared<INT_CMD>( std::move(command1)));//!push the command into que-note use of move
	/*!
		this is the real strength, the push is non blocking and asynchronous.
		Here you can continue to do your work and when you need result of the command
		call get.
	*/
	auto cmd1_res_val = command1_result.get(); //!call ".get()" to get the result
	std::cout << "Command1 Result = " << cmd1_res_val << '\n';

	
	INT_CMD command3{ -1 }; //!this command will make the service quit
	auto command3_result = command3.getFuture();
	cmdq.push(std::make_shared<INT_CMD>(std::move(command3)));
	

	th1.join();

	return 0;
}