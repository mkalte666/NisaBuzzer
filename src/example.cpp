#include "../include/NisaBuzzer.hpp"

int main(int argc, char** argv)
{
	try {
		if(argc < 2) {
			std::cout << "USAGE: example <port>" << std::endl;
			return 0;
		}
		std::string port(argv[1]);
		//lets fire this thing up!
		NisaBuzzer::BuzzerManager manager(port);
		manager.FullReset();
		manager.Arm();
		//add the handler
		bool Stage = true;
		NisaBuzzer::BuzzerEventHandler h1;
		h1.SetHandlerFunction([&Stage](int type, int param) {
			if (type == NisaBuzzer::BuzzerManager::Event::EventType::EVENT_TRIGGER) {
				std::cout << "Triggered " << param << "! now enabling staged mode!" << std::endl;
				Stage = false;
			}
		});
		manager.AddEventHandler(h1);
		while (Stage) {
			manager.Update();
		}
		Stage = true;
		//now we enable threaded event handling. exit by presisng button 4 :P
		manager.RemoveEventHandler(h1);
		manager.SetEnableThreadedEvents(true);
		NisaBuzzer::BuzzerEventHandler h2;
		h2.SetHandlerFunction([&Stage,&manager](int type, int param) {
			if (type == NisaBuzzer::BuzzerManager::Event::EventType::EVENT_TRIGGER) {
				std::cout << "Triggered " << param << "!" << std::endl;
				if (param == 4) {
					Stage = false;
				}
				manager.FullReset();
				manager.Arm();
			}
		});
		manager.AddEventHandler(h2);
		manager.FullReset();
		manager.Arm();
		while (Stage);
	} 
	catch( NisaBuzzer::BuzzerException e) {
		std::cout << "ERR: " << e.what() << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "SOMETHING SOMETHING HELP" << std::endl;
	}
}