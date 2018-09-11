#ifndef bf_controller_cpp
#define bf_controller_cpp

#include <iostream>
#include <string>
#include "bf-timer.cpp"
using namespace std;

class BFController {

public:
	vector<string> m_deferredInterests;
	vector<string> m_deferredData;
	bool m_shouldFloodInterest = true;
	bool m_shouldFloodData = true;
	bool t_shouldFloodInterest = true;
	bool t_shouldFloodData = true;
	BFTimer timerInterest;
	BFTimer timerData;

	void startInterestTimer(){
		timerInterest.StartTimer(1); // 1 for interest timer
	}

	void startDataTimer(){
		timerData.StartTimer(2); // 2 for data timer
	}

	void checkInterest(string interests) {

<<<<<<< HEAD
		LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.interests", interests);
=======
//		LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.interests", interests);
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1

		if (interests.size() == 0){
			m_shouldFloodInterest = false;
			return;
		}

		while(!timerInterest.IsWaitOver()){
<<<<<<< HEAD
			LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.waiting...");
=======
			//LogManager::AddLogWithNodeId("bf-controller.cpp->checkInterest.waiting...");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
			if(existsInterest(interests))
				m_shouldFloodInterest = false;
			else
				m_shouldFloodInterest = true;

			addInterest(interests);
			timerInterest.UpdateTimer();
		}
	}

	void checkData(string data) {

<<<<<<< HEAD
		LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.data", data);
=======
//		LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.data", data);
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1

		if (data.size() == 0){
			m_shouldFloodData = false;
			return;
		}

		while(!timerData.IsWaitOver()){
<<<<<<< HEAD
			LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.waiting...");
=======
//			LogManager::AddLogWithNodeId("bf-controller.cpp->checkData.waiting...");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
			if(existsData(data))
				m_shouldFloodData = false;
			else
				m_shouldFloodData = true;

			addData(data);
			timerData.UpdateTimer();
		}
	}

	void clearInterests() {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.start");
		m_deferredInterests.clear();
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.completed");
	}

	void clearData() {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.start");
		m_deferredData.clear();
		//LogManager::AddLogWithNodeId("bf-controller.cpp->clear.completed");
	}

	void addInterest(string interests) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->add.start.interests", interests);
		if (std::find(m_deferredInterests.begin(), m_deferredInterests.end(), interests) == m_deferredInterests.end()){
			//LogManager::AddLogWithNodeId("bf-controller.cpp->add.if");
			m_deferredInterests.push_back(interests);
		}
//		else
//			LogManager::AddLogWithNodeId("bf-controller.cpp->add.else");
	}

	void addData(string data) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->add.start.interests", interests);
		if (std::find(m_deferredData.begin(), m_deferredData.end(), data) == m_deferredData.end()){
			//LogManager::AddLogWithNodeId("bf-controller.cpp->add.if");
			m_deferredData.push_back(data);
		}
//		else
//			LogManager::AddLogWithNodeId("bf-controller.cpp->add.else");
	}

	bool existsInterest(string interests) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.start");
		for (unsigned int j = 0; j < (unsigned) m_deferredInterests.size(); j++){
			if (m_deferredInterests[j].compare(interests) == 0){
				//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.true");
				return true;
			}
		}

		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.false");
		return false;
	}

	bool existsData(string data) {
		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.start");
		for (unsigned int j = 0; j < (unsigned) m_deferredData.size(); j++){
			if (m_deferredData[j].compare(data) == 0){
				//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.true");
				return true;
			}
		}

		//LogManager::AddLogWithNodeId("bf-controller.cpp->exists.false");
		return false;
	}

	bool shouldFloodInterest(){
		//LogManager::AddLogWithNodeId("bf-controller.cpp->shouldFloodInterest", m_shouldFloodInterest);
		return t_shouldFloodInterest;
	}

	bool shouldFloodData(){
		//LogManager::AddLogWithNodeId("bf-controller.cpp->shouldFloodInterest", m_shouldFloodInterest);
		return t_shouldFloodData;
	}
};
#endif
