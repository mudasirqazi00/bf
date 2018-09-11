/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-consumer.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

#include "utils/ndn-ns3-packet-tag.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <ndn-cxx/lp/tags.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>
<<<<<<< HEAD
#include "../src/ndnSIM/LogManager.cpp"
=======

#include "../src/ndnSIM/NFD/daemon/aif-constants.cpp"
#include "../src/ndnSIM/NFD/daemon/aif-variables.hpp"
#include "../src/ndnSIM/LogManager.cpp"
#include "../src/ndnSIM/NameHelper.cpp"
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1

NS_LOG_COMPONENT_DEFINE("ndn.Consumer");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(Consumer);

TypeId
Consumer::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::Consumer")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddAttribute("StartSeq", "Initial sequence number", IntegerValue(0),
                    MakeIntegerAccessor(&Consumer::m_seq), MakeIntegerChecker<int32_t>())

      .AddAttribute("Prefix", "Name of the Interest", StringValue("/"),
                    MakeNameAccessor(&Consumer::m_interestName), MakeNameChecker())
      .AddAttribute("LifeTime", "LifeTime for interest packet", StringValue("2s"),
                    MakeTimeAccessor(&Consumer::m_interestLifeTime), MakeTimeChecker())

      .AddAttribute("RetxTimer",
                    "Timeout defining how frequent retransmission timeouts should be checked",
                    StringValue("50ms"),
                    MakeTimeAccessor(&Consumer::GetRetxTimer, &Consumer::SetRetxTimer),
                    MakeTimeChecker())

      .AddTraceSource("LastRetransmittedInterestDataDelay",
                      "Delay between last retransmitted Interest and received Data",
                      MakeTraceSourceAccessor(&Consumer::m_lastRetransmittedInterestDataDelay),
                      "ns3::ndn::Consumer::LastRetransmittedInterestDataDelayCallback")

      .AddTraceSource("FirstInterestDataDelay",
                      "Delay between first transmitted Interest and received Data",
                      MakeTraceSourceAccessor(&Consumer::m_firstInterestDataDelay),
                      "ns3::ndn::Consumer::FirstInterestDataDelayCallback");

  return tid;
}

Consumer::Consumer()
  : m_rand(CreateObject<UniformRandomVariable>())
  , m_seq(0)
  , m_seqMax(0) // don't request anything
{
  NS_LOG_FUNCTION_NOARGS();

  m_rtt = CreateObject<RttMeanDeviation>();
}

void
Consumer::SetRetxTimer(Time retxTimer)
{
  m_retxTimer = retxTimer;
  if (m_retxEvent.IsRunning()) {
    // m_retxEvent.Cancel (); // cancel any scheduled cleanup events
    Simulator::Remove(m_retxEvent); // slower, but better for memory
  }

  // schedule even with new timeout
  m_retxEvent = Simulator::Schedule(m_retxTimer, &Consumer::CheckRetxTimeout, this);
}

Time
Consumer::GetRetxTimer() const
{
  return m_retxTimer;
}

void
Consumer::CheckRetxTimeout()
{
  Time now = Simulator::Now();

  Time rto = m_rtt->RetransmitTimeout();
  // NS_LOG_DEBUG ("Current RTO: " << rto.ToDouble (Time::S) << "s");

  while (!m_seqTimeouts.empty()) {
    SeqTimeoutsContainer::index<i_timestamp>::type::iterator entry =
      m_seqTimeouts.get<i_timestamp>().begin();
    if (entry->time + rto <= now) // timeout expired?
    {
      uint32_t seqNo = entry->seq;
      m_seqTimeouts.get<i_timestamp>().erase(entry);
      OnTimeout(seqNo);
    }
    else
      break; // nothing else to do. All later packets need not be retransmitted
  }

  m_retxEvent = Simulator::Schedule(m_retxTimer, &Consumer::CheckRetxTimeout, this);
}

// Application Methods
void
Consumer::StartApplication() // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS();

  // do base stuff
  App::StartApplication();

  ScheduleNextPacket();
}

void
Consumer::StopApplication() // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS();

  // cancel periodic packet generation
  Simulator::Cancel(m_sendEvent);

  // cleanup base stuff
  App::StopApplication();
}

void
Consumer::SendPacket()
{
<<<<<<< HEAD
  if (!m_active)
    return;

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.start");

  NS_LOG_FUNCTION_NOARGS();

=======
	if (!m_active)
		    return;

		  NS_LOG_FUNCTION_NOARGS();

		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.start");
		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.DynamicAccumulationSize", Variables::getDynamicAccumulation());
		  std::vector<Name> v;
		  shared_ptr<Interest> interest;

		  for (uint32_t i = 0; i < Variables::getDynamicAccumulation(); i++) {

			  uint32_t seq = std::numeric_limits<uint32_t>::max(); // invalid
			  while (m_retxSeqs.size()) {
				seq = *m_retxSeqs.begin();
				m_retxSeqs.erase(m_retxSeqs.begin());
				break;
			  }

			  if (seq == std::numeric_limits<uint32_t>::max()) {
				if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
				  if (m_seq >= m_seqMax) {
					return; // we are totally done
				  }
				}

				seq = m_seq++;
			  }

			  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
			  nameWithSequence->appendSequenceNumber(seq);

			  v.push_back(*nameWithSequence);

			  interest = make_shared<Interest>();
			  interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
			  interest->setName(*nameWithSequence);
			  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
			  interest->setInterestLifetime(interestLifeTime);

			  interest->setPreviousNodeId(LogHelper::GetNodeId());

			  NS_LOG_INFO("> Interest for " << seq);
			  WillSendOutInterest(seq);
			  m_transmittedInterests(interest, this, m_face);
		  }


		  //LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.name", interest->getName().toUri());
		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.setPreviousNodeId", interest->getPreviousNodeId());

		  interest->setAccNames(v);
		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.accNames", NameHelper::join(interest->getAccNames()));

		  m_appLink->onReceiveInterest(*interest);

		  ScheduleNextPacket();

		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.completed");


	//Working code (before Dynamic Accumulation)
	/*
	if (!m_active)
	    return;

	  NS_LOG_FUNCTION_NOARGS();

	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.start");
	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.accumulationSize", Constants::AccumulationSize);
	  std::vector<Name> v;
	  shared_ptr<Interest> interest;

	  for (unsigned int i = 0; i < Constants::AccumulationSize; i++) {

		  uint32_t seq = std::numeric_limits<uint32_t>::max(); // invalid
		  while (m_retxSeqs.size()) {
			seq = *m_retxSeqs.begin();
			m_retxSeqs.erase(m_retxSeqs.begin());
			break;
		  }

		  if (seq == std::numeric_limits<uint32_t>::max()) {
			if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
			  if (m_seq >= m_seqMax) {
				return; // we are totally done
			  }
			}

			seq = m_seq++;
		  }

		  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
		  nameWithSequence->appendSequenceNumber(seq);

		  v.push_back(*nameWithSequence);

		  interest = make_shared<Interest>();
		  interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
		  interest->setName(*nameWithSequence);
		  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
		  interest->setInterestLifetime(interestLifeTime);

		  interest->setPreviousNodeId(LogHelper::GetNodeId());

		  NS_LOG_INFO("> Interest for " << seq);
		  WillSendOutInterest(seq);
		  m_transmittedInterests(interest, this, m_face);
	  }


	  //LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.name", interest->getName().toUri());
	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.setPreviousNodeId", interest->getPreviousNodeId());

	  interest->setAccNames(v);
	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.accNames", NameHelper::join(interest->getAccNames()));

	  m_appLink->onReceiveInterest(*interest);

	  ScheduleNextPacket();

	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.completed");
*/


	//Working code (before accumulation)
/*
  if (!m_active)
    return;

  NS_LOG_FUNCTION_NOARGS();

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.start");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  uint32_t seq = std::numeric_limits<uint32_t>::max(); // invalid

  while (m_retxSeqs.size()) {
    seq = *m_retxSeqs.begin();
    m_retxSeqs.erase(m_retxSeqs.begin());
    break;
  }

  if (seq == std::numeric_limits<uint32_t>::max()) {
    if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
      if (m_seq >= m_seqMax) {
        return; // we are totally done
      }
    }

    seq = m_seq++;
  }

<<<<<<< HEAD
  //
  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
  nameWithSequence->appendSequenceNumber(seq);
  //

  // shared_ptr<Interest> interest = make_shared<Interest> ();
=======
  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
  nameWithSequence->appendSequenceNumber(seq);

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  shared_ptr<Interest> interest = make_shared<Interest>();
  interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setName(*nameWithSequence);
  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
  interest->setInterestLifetime(interestLifeTime);

<<<<<<< HEAD
  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  NS_LOG_INFO("> Interest for " << seq);

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.name", interest->getName().toUri());

=======
  interest->setPreviousNodeId(LogHelper::GetNodeId());
  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.name", interest->getName().toUri());
  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.previousNodeId", interest->getPreviousNodeId());

  std::vector<Name> v;
  v.push_back(Name("abc1"));
  v.push_back(Name("abc2"));
  v.push_back(Name("abc3"));
  interest->setAccNames(v);
  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.accNames", NameHelper::join(interest->getAccNames()));

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  NS_LOG_INFO("> Interest for " << seq);

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  WillSendOutInterest(seq);

  m_transmittedInterests(interest, this, m_face);
  m_appLink->onReceiveInterest(*interest);

  ScheduleNextPacket();

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->SendPacket.completed");
<<<<<<< HEAD
=======
*/
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
Consumer::OnData(shared_ptr<const Data> data)
{
<<<<<<< HEAD
=======
	if (!m_active)
	    return;

	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.start");
	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.name", data->getName().toUri());

	  App::OnData(data); // tracing inside

	  NS_LOG_FUNCTION(this << data);

	  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

	  // This could be a problem......
	  uint32_t seq = data->getName().at(-1).toSequenceNumber();
	  NS_LOG_INFO("< DATA for " << seq);

	  int hopCount = 0;
	  auto hopCountTag = data->getTag<lp::HopCountTag>();
	  if (hopCountTag != nullptr) { // e.g., packet came from local node's cache
	    hopCount = *hopCountTag;
	  }
	  NS_LOG_DEBUG("Hop count: " << hopCount);

	  SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find(seq);
	  if (entry != m_seqLastDelay.end()) {
	    m_lastRetransmittedInterestDataDelay(this, seq, Simulator::Now() - entry->time, hopCount);
	  }

	  //Dynamic accumulation size
	  if(Constants::EnableDynamicAccumulation == 1){
		  Time spentTime = Simulator::Now() - entry->time;
		  double spentSeconds = spentTime.ToDouble(Time::S);

		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.seq", seq);
		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.spentSeconds", spentSeconds);
		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.Constants::DynamicAccumulationSize", Variables::getDynamicAccumulation());

		  if(spentSeconds < Constants::MinimumTime){
			  Variables::increaseAccumulation();
			  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.Constants::DynamicAccumulationSize.Increased");
		  }

		  if(spentSeconds > Constants::MinimumTime){
			  Variables::decreaseAccumulation();
			  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.Constants::DynamicAccumulationSize.Decreased");
		  }

		  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.Constants::DynamicAccumulationSize", Variables::getDynamicAccumulation());
	  }

	  entry = m_seqFullDelay.find(seq);
	  if (entry != m_seqFullDelay.end()) {
	    m_firstInterestDataDelay(this, seq, Simulator::Now() - entry->time, m_seqRetxCounts[seq], hopCount);
	  }

//	  Time sss = Simulator::Now() - entry->time;
//	  double aaa = sss.ToDouble(Time::S);
//	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.aaa", aaa);


	  m_seqRetxCounts.erase(seq);
	  m_seqFullDelay.erase(seq);
	  m_seqLastDelay.erase(seq);

	  m_seqTimeouts.erase(seq);
	  m_retxSeqs.erase(seq);

	  m_rtt->AckSeq(SequenceNumber32(seq));

	  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.completed");


	//Working code (before dynamic accumulation)
	/*
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  if (!m_active)
    return;

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.start");
  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.name", data->getName().toUri());

  App::OnData(data); // tracing inside

  NS_LOG_FUNCTION(this << data);

  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

  // This could be a problem......
  uint32_t seq = data->getName().at(-1).toSequenceNumber();
  NS_LOG_INFO("< DATA for " << seq);

  int hopCount = 0;
  auto hopCountTag = data->getTag<lp::HopCountTag>();
  if (hopCountTag != nullptr) { // e.g., packet came from local node's cache
    hopCount = *hopCountTag;
  }
  NS_LOG_DEBUG("Hop count: " << hopCount);

  SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find(seq);
  if (entry != m_seqLastDelay.end()) {
    m_lastRetransmittedInterestDataDelay(this, seq, Simulator::Now() - entry->time, hopCount);
  }

  entry = m_seqFullDelay.find(seq);
  if (entry != m_seqFullDelay.end()) {
    m_firstInterestDataDelay(this, seq, Simulator::Now() - entry->time, m_seqRetxCounts[seq], hopCount);
  }

  m_seqRetxCounts.erase(seq);
  m_seqFullDelay.erase(seq);
  m_seqLastDelay.erase(seq);

  m_seqTimeouts.erase(seq);
  m_retxSeqs.erase(seq);

  m_rtt->AckSeq(SequenceNumber32(seq));

  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnData.completed");
<<<<<<< HEAD
=======
  */
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Consumer::OnNack(shared_ptr<const lp::Nack> nack)
{
  /// tracing inside
  App::OnNack(nack);

  NS_LOG_INFO("NACK received for: " << nack->getInterest().getName()
              << ", reason: " << nack->getReason());
}

void
Consumer::OnTimeout(uint32_t sequenceNumber)
{
<<<<<<< HEAD
=======
//	LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnTimeout.start");
  NS_LOG_FUNCTION(sequenceNumber);
  // std::cout << Simulator::Now () << ", TO: " << sequenceNumber << ", current RTO: " <<
  // m_rtt->RetransmitTimeout ().ToDouble (Time::S) << "s\n";

//  double now = Simulator::Now().ToDouble(Time::S);
//  double a = m_rtt->RetransmitTimeout().ToDouble(Time::S);
//
//  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnTimeout.Now", now);
//  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnTimeout.RetransmitTimeout", a);

  m_rtt->IncreaseMultiplier(); // Double the next RTO
  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert(sequenceNumber);
  ScheduleNextPacket();
//  LogManager::AddLogWithNodeId("ndn-consumer.cpp->OnTimeout.completed");

  //Working
  /*
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  NS_LOG_FUNCTION(sequenceNumber);
  // std::cout << Simulator::Now () << ", TO: " << sequenceNumber << ", current RTO: " <<
  // m_rtt->RetransmitTimeout ().ToDouble (Time::S) << "s\n";

  m_rtt->IncreaseMultiplier(); // Double the next RTO
<<<<<<< HEAD
  m_rtt->SentSeq(SequenceNumber32(sequenceNumber),
                 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert(sequenceNumber);
  ScheduleNextPacket();
=======
  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert(sequenceNumber);
  ScheduleNextPacket();
   */
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Consumer::WillSendOutInterest(uint32_t sequenceNumber)
{
  NS_LOG_DEBUG("Trying to add " << sequenceNumber << " with " << Simulator::Now() << ". already "
                                << m_seqTimeouts.size() << " items");

  m_seqTimeouts.insert(SeqTimeout(sequenceNumber, Simulator::Now()));
  m_seqFullDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqLastDelay.erase(sequenceNumber);
  m_seqLastDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqRetxCounts[sequenceNumber]++;

  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1);
}

} // namespace ndn
} // namespace ns3
