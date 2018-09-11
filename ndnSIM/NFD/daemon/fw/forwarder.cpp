/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2017,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "forwarder.hpp"
#include "algorithm.hpp"
#include "best-route-strategy2.hpp"
#include "strategy.hpp"
#include "core/logger.hpp"
#include "table/cleanup.hpp"
#include <ndn-cxx/lp/tags.hpp>

#include "face/null-face.hpp"

<<<<<<< HEAD
=======
#include "../src/ndnSIM/LogManager.cpp"
#include "../src/ndnSIM/NameHelper.cpp"
#include "../src/ndnSIM/NFD/daemon/aif-constants.cpp"
#include "../src/ndnSIM/NFD/daemon/aif-variables.hpp"

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
namespace nfd {

NFD_LOG_INIT("Forwarder");

static Name
getDefaultStrategyName()
{
  return fw::BestRouteStrategy2::getStrategyName();
}

Forwarder::Forwarder()
  : m_unsolicitedDataPolicy(new fw::DefaultUnsolicitedDataPolicy())
  , m_fib(m_nameTree)
  , m_pit(m_nameTree)
  , m_measurements(m_nameTree)
  , m_strategyChoice(*this)
  , m_csFace(face::makeNullFace(FaceUri("contentstore://")))
{
  getFaceTable().addReserved(m_csFace, face::FACEID_CONTENT_STORE);

  m_faceTable.afterAdd.connect([this] (Face& face) {
    face.afterReceiveInterest.connect(
      [this, &face] (const Interest& interest) {
<<<<<<< HEAD
        this->startProcessInterest(face, interest);
      });
    face.afterReceiveData.connect(
      [this, &face] (const Data& data) {
        this->startProcessData(face, data);
=======

    	LogManager::AddLogWithNodeId("forwarder.cpp->afterReceiveInterest");
    	LogManager::AddLogWithNodeId("forwarder.cpp->startProcessInterest.name", interest.getName().toUri());
//    	LogManager::AddLogWithNodeId("forwarder.cpp->startProcessInterest.accNames", NameHelper::join(interest.getAccNames()));

        this->startProcessInterest(face, interest);
        LogManager::AddLogWithNodeId("forwarder.cpp->startProcessInterest.completed");

      });
    face.afterReceiveData.connect(
      [this, &face] (const Data& data) {

    	LogManager::AddLogWithNodeId("forwarder.cpp->afterReceiveData");
		LogManager::AddLogWithNodeId("forwarder.cpp->startProcessData.name", data.getName().toUri());
        this->startProcessData(face, data);
        LogManager::AddLogWithNodeId("forwarder.cpp->startProcessData.completed");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
      });
    face.afterReceiveNack.connect(
      [this, &face] (const lp::Nack& nack) {
        this->startProcessNack(face, nack);
      });
    face.onDroppedInterest.connect(
      [this, &face] (const Interest& interest) {
        this->onDroppedInterest(face, interest);
      });
  });

  m_faceTable.beforeRemove.connect([this] (Face& face) {
    cleanupOnFaceRemoval(m_nameTree, m_fib, m_pit, face);
  });

  m_strategyChoice.setDefaultStrategy(getDefaultStrategyName());
}

Forwarder::~Forwarder() = default;

void
Forwarder::onIncomingInterest(Face& inFace, const Interest& interest)
{
<<<<<<< HEAD
  // receive Interest
  NFD_LOG_DEBUG("onIncomingInterest face=" << inFace.getId() <<
                " interest=" << interest.getName());
  interest.setTag(make_shared<lp::IncomingFaceIdTag>(inFace.getId()));
  ++m_counters.nInInterests;

=======
	LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingInterest.start.name", interest.getName().toUri());
  // receive Interest
  NFD_LOG_DEBUG("onIncomingInterest face=" << inFace.getId() << " interest=" << interest.getName());
  interest.setTag(make_shared<lp::IncomingFaceIdTag>(inFace.getId()));
  ++m_counters.nInInterests;

//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.1");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // /localhost scope control
  bool isViolatingLocalhost = inFace.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL &&
                              scope_prefix::LOCALHOST.isPrefixOf(interest.getName());
  if (isViolatingLocalhost) {
    NFD_LOG_DEBUG("onIncomingInterest face=" << inFace.getId() <<
                  " interest=" << interest.getName() << " violates /localhost");
    // (drop)
    return;
  }

<<<<<<< HEAD
=======
//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.2");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // detect duplicate Nonce with Dead Nonce List
  bool hasDuplicateNonceInDnl = m_deadNonceList.has(interest.getName(), interest.getNonce());
  if (hasDuplicateNonceInDnl) {
    // goto Interest loop pipeline
    this->onInterestLoop(inFace, interest);
    return;
  }

<<<<<<< HEAD
=======
//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.3");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // strip forwarding hint if Interest has reached producer region
  if (!interest.getForwardingHint().empty() &&
      m_networkRegionTable.isInProducerRegion(interest.getForwardingHint())) {
    NFD_LOG_DEBUG("onIncomingInterest face=" << inFace.getId() <<
                  " interest=" << interest.getName() << " reaching-producer-region");
    const_cast<Interest&>(interest).setForwardingHint({});
  }

<<<<<<< HEAD
  // PIT insert
  shared_ptr<pit::Entry> pitEntry = m_pit.insert(interest).first;

=======
//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.4");
  // PIT insert
  shared_ptr<pit::Entry> pitEntry = m_pit.insert(interest).first;

//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.5");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // detect duplicate Nonce in PIT entry
  int dnw = fw::findDuplicateNonce(*pitEntry, interest.getNonce(), inFace);
  bool hasDuplicateNonceInPit = dnw != fw::DUPLICATE_NONCE_NONE;
  if (inFace.getLinkType() == ndn::nfd::LINK_TYPE_POINT_TO_POINT) {
    // for p2p face: duplicate Nonce from same incoming face is not loop
    hasDuplicateNonceInPit = hasDuplicateNonceInPit && !(dnw & fw::DUPLICATE_NONCE_IN_SAME);
  }
  if (hasDuplicateNonceInPit) {
    // goto Interest loop pipeline
    this->onInterestLoop(inFace, interest);
    return;
  }

<<<<<<< HEAD
=======
//  LogManager::AddLog("forwarder.cpp->onIncomingInterest.6");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // cancel unsatisfy & straggler timer
  this->cancelUnsatisfyAndStragglerTimer(*pitEntry);

  const pit::InRecordCollection& inRecords = pitEntry->getInRecords();
  bool isPending = inRecords.begin() != inRecords.end();
  if (!isPending) {
    if (m_csFromNdnSim == nullptr) {
      m_cs.find(interest,
                bind(&Forwarder::onContentStoreHit, this, ref(inFace), pitEntry, _1, _2),
                bind(&Forwarder::onContentStoreMiss, this, ref(inFace), pitEntry, _1));
    }
    else {
      shared_ptr<Data> match = m_csFromNdnSim->Lookup(interest.shared_from_this());
      if (match != nullptr) {
        this->onContentStoreHit(inFace, pitEntry, interest, *match);
      }
      else {
        this->onContentStoreMiss(inFace, pitEntry, interest);
      }
    }
  }
  else {
    this->onContentStoreMiss(inFace, pitEntry, interest);
  }
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingInterest.completed");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Forwarder::onInterestLoop(Face& inFace, const Interest& interest)
{
  // if multi-access or ad hoc face, drop
  if (inFace.getLinkType() != ndn::nfd::LINK_TYPE_POINT_TO_POINT) {
    NFD_LOG_DEBUG("onInterestLoop face=" << inFace.getId() <<
                  " interest=" << interest.getName() <<
                  " drop");
    return;
  }

  NFD_LOG_DEBUG("onInterestLoop face=" << inFace.getId() <<
                " interest=" << interest.getName() <<
                " send-Nack-duplicate");

  // send Nack with reason=DUPLICATE
  // note: Don't enter outgoing Nack pipeline because it needs an in-record.
  lp::Nack nack(interest);
  nack.setReason(lp::NackReason::DUPLICATE);
  inFace.sendNack(nack);
}

<<<<<<< HEAD
=======

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
void
Forwarder::onContentStoreMiss(const Face& inFace, const shared_ptr<pit::Entry>& pitEntry,
                              const Interest& interest)
{
  NFD_LOG_DEBUG("onContentStoreMiss interest=" << interest.getName());
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onContentStoreMiss.name", interest.getName().toUri());

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  ++m_counters.nCsMisses;

  // insert in-record
  pitEntry->insertOrUpdateInRecord(const_cast<Face&>(inFace), interest);

  // set PIT unsatisfy timer
  this->setUnsatisfyTimer(pitEntry);

  // has NextHopFaceId?
  shared_ptr<lp::NextHopFaceIdTag> nextHopTag = interest.getTag<lp::NextHopFaceIdTag>();
  if (nextHopTag != nullptr) {
    // chosen NextHop face exists?
    Face* nextHopFace = m_faceTable.get(*nextHopTag);
    if (nextHopFace != nullptr) {
      NFD_LOG_DEBUG("onContentStoreMiss interest=" << interest.getName() << " nexthop-faceid=" << nextHopFace->getId());
      // go to outgoing Interest pipeline
      // scope control is unnecessary, because privileged app explicitly wants to forward
      this->onOutgoingInterest(pitEntry, *nextHopFace, interest);
    }
    return;
  }

  // dispatch to strategy: after incoming Interest
  this->dispatchToStrategy(*pitEntry,
    [&] (fw::Strategy& strategy) { strategy.afterReceiveInterest(inFace, interest, pitEntry); });
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onContentStoreMiss.completed");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Forwarder::onContentStoreHit(const Face& inFace, const shared_ptr<pit::Entry>& pitEntry,
                             const Interest& interest, const Data& data)
{
  NFD_LOG_DEBUG("onContentStoreHit interest=" << interest.getName());
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onContentStoreHit.start.name", interest.getName().toUri());

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  ++m_counters.nCsHits;

  beforeSatisfyInterest(*pitEntry, *m_csFace, data);
  this->dispatchToStrategy(*pitEntry,
    [&] (fw::Strategy& strategy) { strategy.beforeSatisfyInterest(pitEntry, *m_csFace, data); });

  data.setTag(make_shared<lp::IncomingFaceIdTag>(face::FACEID_CONTENT_STORE));
  // XXX should we lookup PIT for other Interests that also match csMatch?

  // set PIT straggler timer
  this->setStragglerTimer(pitEntry, true, data.getFreshnessPeriod());

  // goto outgoing Data pipeline
  this->onOutgoingData(data, *const_pointer_cast<Face>(inFace.shared_from_this()));
<<<<<<< HEAD
}

void
Forwarder::onOutgoingInterest(const shared_ptr<pit::Entry>& pitEntry, Face& outFace, const Interest& interest)
{
  NFD_LOG_DEBUG("onOutgoingInterest face=" << outFace.getId() <<
                " interest=" << pitEntry->getName());

=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onContentStoreHit.completed");

}

std::vector<Name> m_localNames;

void
Forwarder::onOutgoingInterest(const shared_ptr<pit::Entry>& pitEntry, Face& outFace, const Interest& interest)
{
	NFD_LOG_DEBUG("onOutgoingInterest face=" << outFace.getId() <<
			                " interest=" << pitEntry->getName());

		  int size = interest.getAccNames().size();
		  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.start.size", size);

		  pitEntry->insertOrUpdateOutRecord(outFace, interest);

		  const_cast<Interest&>(interest).setPreviousNodeId(LogHelper::GetNodeId());
		  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.setPreviousNodeId", interest.getPreviousNodeId());

		  if(size == 0){
				LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.start.name", interest.getName().toUri());
				LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.accNames", NameHelper::join(interest.getAccNames()));

				// send Interest
				outFace.sendInterest(interest);
		  }
		  else{
			  m_localNames.push_back(interest.getName());
			  int localSize = m_localNames.size();
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.localSize", localSize);

			  if(localSize < Variables::getDynamicAccumulation()){
				  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.m_localNames", NameHelper::join(m_localNames));
				  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.returnWithoutFlooding");
			  }

			  if(localSize == Variables::getDynamicAccumulation()){
				  const_cast<Interest&>(interest).setAccNames(m_localNames);
				  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.flooding");
				  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.previousNodeId", interest.getPreviousNodeId());
				  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.accNames", NameHelper::join(interest.getAccNames()));

				  m_localNames.clear();

				  // send Interest
				  outFace.sendInterest(interest);
				  ++m_counters.nOutInterests;
			  }
		  }

		  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.completed");


	//Working code (before dynamic accumulation)
	/*
	NFD_LOG_DEBUG("onOutgoingInterest face=" << outFace.getId() <<
		                " interest=" << pitEntry->getName());

	  int size = interest.getAccNames().size();
	  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.start.size", size);

	  pitEntry->insertOrUpdateOutRecord(outFace, interest);

	  const_cast<Interest&>(interest).setPreviousNodeId(LogHelper::GetNodeId());
	  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.setPreviousNodeId", interest.getPreviousNodeId());

	  if(size == 0){
			LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.start.name", interest.getName().toUri());
			LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.accNames", NameHelper::join(interest.getAccNames()));

			// send Interest
			outFace.sendInterest(interest);
			++m_counters.nOutInterests;
	  }
	  else{
		  m_localNames.push_back(interest.getName());
		  int localSize = m_localNames.size();
		  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.localSize", localSize);

		  if(localSize < Constants::AccumulationSize){
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.m_localNames", NameHelper::join(m_localNames));
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.returnWithoutFlooding");
		  }

		  if(localSize == Constants::AccumulationSize){
			  const_cast<Interest&>(interest).setAccNames(m_localNames);
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.flooding");
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.previousNodeId", interest.getPreviousNodeId());
			  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.accNames", NameHelper::join(interest.getAccNames()));

			  m_localNames.clear();

			  // send Interest
			  outFace.sendInterest(interest);
			  ++m_counters.nOutInterests;
		  }
	  }

	  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.completed");
*/

	//Working code (before accumulation)
/*
  NFD_LOG_DEBUG("onOutgoingInterest face=" << outFace.getId() <<
                " interest=" << pitEntry->getName());

  const_cast<Interest&>(interest).setPreviousNodeId(LogHelper::GetNodeId());
  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.start.name", interest.getName().toUri());
  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.previousNodeId", interest.getPreviousNodeId());
  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.accNames", NameHelper::join(interest.getAccNames()));

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // insert out-record
  pitEntry->insertOrUpdateOutRecord(outFace, interest);

  // send Interest
  outFace.sendInterest(interest);
  ++m_counters.nOutInterests;
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.completed");
*/
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Forwarder::onInterestReject(const shared_ptr<pit::Entry>& pitEntry)
{
  if (fw::hasPendingOutRecords(*pitEntry)) {
    NFD_LOG_ERROR("onInterestReject interest=" << pitEntry->getName() <<
                  " cannot reject forwarded Interest");
    return;
  }
  NFD_LOG_DEBUG("onInterestReject interest=" << pitEntry->getName());

  // cancel unsatisfy & straggler timer
  this->cancelUnsatisfyAndStragglerTimer(*pitEntry);

  // set PIT straggler timer
  this->setStragglerTimer(pitEntry, false);
}

void
Forwarder::onInterestUnsatisfied(const shared_ptr<pit::Entry>& pitEntry)
{
  NFD_LOG_DEBUG("onInterestUnsatisfied interest=" << pitEntry->getName());

  // invoke PIT unsatisfied callback
  beforeExpirePendingInterest(*pitEntry);
  this->dispatchToStrategy(*pitEntry,
    [&] (fw::Strategy& strategy) { strategy.beforeExpirePendingInterest(pitEntry); });

  // goto Interest Finalize pipeline
  this->onInterestFinalize(pitEntry, false);
}

void
Forwarder::onInterestFinalize(const shared_ptr<pit::Entry>& pitEntry, bool isSatisfied,
                              ndn::optional<time::milliseconds> dataFreshnessPeriod)
{
  NFD_LOG_DEBUG("onInterestFinalize interest=" << pitEntry->getName() <<
                (isSatisfied ? " satisfied" : " unsatisfied"));

  // Dead Nonce List insert if necessary
  this->insertDeadNonceList(*pitEntry, isSatisfied, dataFreshnessPeriod, 0);

  // PIT delete
  this->cancelUnsatisfyAndStragglerTimer(*pitEntry);
  m_pit.erase(pitEntry.get());
}

void
Forwarder::onIncomingData(Face& inFace, const Data& data)
{
  // receive Data
  NFD_LOG_DEBUG("onIncomingData face=" << inFace.getId() << " data=" << data.getName());
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingData.start.name", data.getName().toUri());

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  data.setTag(make_shared<lp::IncomingFaceIdTag>(inFace.getId()));
  ++m_counters.nInData;

  // /localhost scope control
  bool isViolatingLocalhost = inFace.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL &&
                              scope_prefix::LOCALHOST.isPrefixOf(data.getName());
  if (isViolatingLocalhost) {
    NFD_LOG_DEBUG("onIncomingData face=" << inFace.getId() <<
                  " data=" << data.getName() << " violates /localhost");
    // (drop)
<<<<<<< HEAD
=======

    LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingData.dropping");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
    return;
  }

  // PIT match
  pit::DataMatchResult pitMatches = m_pit.findAllDataMatches(data);
  if (pitMatches.begin() == pitMatches.end()) {
    // goto Data unsolicited pipeline
    this->onDataUnsolicited(inFace, data);
<<<<<<< HEAD
=======

    LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingData.onDataUnsolicited");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
    return;
  }

  shared_ptr<Data> dataCopyWithoutTag = make_shared<Data>(data);
  dataCopyWithoutTag->removeTag<lp::HopCountTag>();

  // CS insert
  if (m_csFromNdnSim == nullptr)
    m_cs.insert(*dataCopyWithoutTag);
  else
    m_csFromNdnSim->Add(dataCopyWithoutTag);

<<<<<<< HEAD
=======
  LogManager::AddLogWithNodeId("forwarder.cpp->onIncomingData.m_cs.insert");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  std::set<Face*> pendingDownstreams;
  // foreach PitEntry
  auto now = time::steady_clock::now();
  for (const shared_ptr<pit::Entry>& pitEntry : pitMatches) {
    NFD_LOG_DEBUG("onIncomingData matching=" << pitEntry->getName());

    // cancel unsatisfy & straggler timer
    this->cancelUnsatisfyAndStragglerTimer(*pitEntry);

    // remember pending downstreams
    for (const pit::InRecord& inRecord : pitEntry->getInRecords()) {
      if (inRecord.getExpiry() > now) {
        pendingDownstreams.insert(&inRecord.getFace());
      }
    }

    // invoke PIT satisfy callback
    beforeSatisfyInterest(*pitEntry, inFace, data);
    this->dispatchToStrategy(*pitEntry,
      [&] (fw::Strategy& strategy) { strategy.beforeSatisfyInterest(pitEntry, inFace, data); });

    // Dead Nonce List insert if necessary (for out-record of inFace)
    this->insertDeadNonceList(*pitEntry, true, data.getFreshnessPeriod(), &inFace);

    // mark PIT satisfied
    pitEntry->clearInRecords();
    pitEntry->deleteOutRecord(inFace);

    // set PIT straggler timer
    this->setStragglerTimer(pitEntry, true, data.getFreshnessPeriod());
  }

  // foreach pending downstream
  for (Face* pendingDownstream : pendingDownstreams) {
    if (pendingDownstream->getId() == inFace.getId() &&
        pendingDownstream->getLinkType() != ndn::nfd::LINK_TYPE_AD_HOC) {
      continue;
    }
    // goto outgoing Data pipeline
    this->onOutgoingData(data, *pendingDownstream);
  }
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingInterest.completed");

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Forwarder::onDataUnsolicited(Face& inFace, const Data& data)
{
  // accept to cache?
  fw::UnsolicitedDataDecision decision = m_unsolicitedDataPolicy->decide(inFace, data);
  if (decision == fw::UnsolicitedDataDecision::CACHE) {
    // CS insert
    if (m_csFromNdnSim == nullptr)
      m_cs.insert(data, true);
    else
      m_csFromNdnSim->Add(data.shared_from_this());
  }

  NFD_LOG_DEBUG("onDataUnsolicited face=" << inFace.getId() <<
                " data=" << data.getName() <<
                " decision=" << decision);
}

void
Forwarder::onOutgoingData(const Data& data, Face& outFace)
{
  if (outFace.getId() == face::INVALID_FACEID) {
    NFD_LOG_WARN("onOutgoingData face=invalid data=" << data.getName());
    return;
  }
  NFD_LOG_DEBUG("onOutgoingData face=" << outFace.getId() << " data=" << data.getName());

<<<<<<< HEAD
=======
  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingData.start.name", data.getName().toUri());

>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
  // /localhost scope control
  bool isViolatingLocalhost = outFace.getScope() == ndn::nfd::FACE_SCOPE_NON_LOCAL &&
                              scope_prefix::LOCALHOST.isPrefixOf(data.getName());
  if (isViolatingLocalhost) {
    NFD_LOG_DEBUG("onOutgoingData face=" << outFace.getId() <<
                  " data=" << data.getName() << " violates /localhost");
    // (drop)
    return;
  }

  // TODO traffic manager

  // send Data
  outFace.sendData(data);
  ++m_counters.nOutData;
<<<<<<< HEAD
=======

  LogManager::AddLogWithNodeId("forwarder.cpp->onOutgoingData.completed");
>>>>>>> 7eb7425ea45060c99508dd3504ae5a5a9efcb0c1
}

void
Forwarder::onIncomingNack(Face& inFace, const lp::Nack& nack)
{
  // receive Nack
  nack.setTag(make_shared<lp::IncomingFaceIdTag>(inFace.getId()));
  ++m_counters.nInNacks;

  // if multi-access or ad hoc face, drop
  if (inFace.getLinkType() != ndn::nfd::LINK_TYPE_POINT_TO_POINT) {
    NFD_LOG_DEBUG("onIncomingNack face=" << inFace.getId() <<
                  " nack=" << nack.getInterest().getName() <<
                  "~" << nack.getReason() << " face-is-multi-access");
    return;
  }

  // PIT match
  shared_ptr<pit::Entry> pitEntry = m_pit.find(nack.getInterest());
  // if no PIT entry found, drop
  if (pitEntry == nullptr) {
    NFD_LOG_DEBUG("onIncomingNack face=" << inFace.getId() <<
                  " nack=" << nack.getInterest().getName() <<
                  "~" << nack.getReason() << " no-PIT-entry");
    return;
  }

  // has out-record?
  pit::OutRecordCollection::iterator outRecord = pitEntry->getOutRecord(inFace);
  // if no out-record found, drop
  if (outRecord == pitEntry->out_end()) {
    NFD_LOG_DEBUG("onIncomingNack face=" << inFace.getId() <<
                  " nack=" << nack.getInterest().getName() <<
                  "~" << nack.getReason() << " no-out-record");
    return;
  }

  // if out-record has different Nonce, drop
  if (nack.getInterest().getNonce() != outRecord->getLastNonce()) {
    NFD_LOG_DEBUG("onIncomingNack face=" << inFace.getId() <<
                  " nack=" << nack.getInterest().getName() <<
                  "~" << nack.getReason() << " wrong-Nonce " <<
                  nack.getInterest().getNonce() << "!=" << outRecord->getLastNonce());
    return;
  }

  NFD_LOG_DEBUG("onIncomingNack face=" << inFace.getId() <<
                " nack=" << nack.getInterest().getName() <<
                "~" << nack.getReason() << " OK");

  // record Nack on out-record
  outRecord->setIncomingNack(nack);

  // trigger strategy: after receive NACK
  this->dispatchToStrategy(*pitEntry,
    [&] (fw::Strategy& strategy) { strategy.afterReceiveNack(inFace, nack, pitEntry); });
}

void
Forwarder::onOutgoingNack(const shared_ptr<pit::Entry>& pitEntry, const Face& outFace,
                          const lp::NackHeader& nack)
{
  if (outFace.getId() == face::INVALID_FACEID) {
    NFD_LOG_WARN("onOutgoingNack face=invalid" <<
                  " nack=" << pitEntry->getInterest().getName() <<
                  "~" << nack.getReason() << " no-in-record");
    return;
  }

  // has in-record?
  pit::InRecordCollection::iterator inRecord = pitEntry->getInRecord(outFace);

  // if no in-record found, drop
  if (inRecord == pitEntry->in_end()) {
    NFD_LOG_DEBUG("onOutgoingNack face=" << outFace.getId() <<
                  " nack=" << pitEntry->getInterest().getName() <<
                  "~" << nack.getReason() << " no-in-record");
    return;
  }

  // if multi-access or ad hoc face, drop
  if (outFace.getLinkType() != ndn::nfd::LINK_TYPE_POINT_TO_POINT) {
    NFD_LOG_DEBUG("onOutgoingNack face=" << outFace.getId() <<
                  " nack=" << pitEntry->getInterest().getName() <<
                  "~" << nack.getReason() << " face-is-multi-access");
    return;
  }

  NFD_LOG_DEBUG("onOutgoingNack face=" << outFace.getId() <<
                " nack=" << pitEntry->getInterest().getName() <<
                "~" << nack.getReason() << " OK");

  // create Nack packet with the Interest from in-record
  lp::Nack nackPkt(inRecord->getInterest());
  nackPkt.setHeader(nack);

  // erase in-record
  pitEntry->deleteInRecord(outFace);

  // send Nack on face
  const_cast<Face&>(outFace).sendNack(nackPkt);
  ++m_counters.nOutNacks;
}

void
Forwarder::onDroppedInterest(Face& outFace, const Interest& interest)
{
  m_strategyChoice.findEffectiveStrategy(interest.getName()).onDroppedInterest(outFace, interest);
}

static inline bool
compare_InRecord_expiry(const pit::InRecord& a, const pit::InRecord& b)
{
  return a.getExpiry() < b.getExpiry();
}

void
Forwarder::setUnsatisfyTimer(const shared_ptr<pit::Entry>& pitEntry)
{
  pit::InRecordCollection::iterator lastExpiring =
    std::max_element(pitEntry->in_begin(), pitEntry->in_end(), &compare_InRecord_expiry);

  time::steady_clock::TimePoint lastExpiry = lastExpiring->getExpiry();
  time::nanoseconds lastExpiryFromNow = lastExpiry - time::steady_clock::now();
  if (lastExpiryFromNow <= time::seconds::zero()) {
    // TODO all in-records are already expired; will this happen?
  }

  scheduler::cancel(pitEntry->m_unsatisfyTimer);
  pitEntry->m_unsatisfyTimer = scheduler::schedule(lastExpiryFromNow,
    bind(&Forwarder::onInterestUnsatisfied, this, pitEntry));
}

void
Forwarder::setStragglerTimer(const shared_ptr<pit::Entry>& pitEntry, bool isSatisfied,
                             ndn::optional<time::milliseconds> dataFreshnessPeriod)
{
  time::nanoseconds stragglerTime = time::milliseconds(100);

  scheduler::cancel(pitEntry->m_stragglerTimer);
  pitEntry->m_stragglerTimer = scheduler::schedule(stragglerTime,
    bind(&Forwarder::onInterestFinalize, this, pitEntry, isSatisfied, dataFreshnessPeriod));
}

void
Forwarder::cancelUnsatisfyAndStragglerTimer(pit::Entry& pitEntry)
{
  scheduler::cancel(pitEntry.m_unsatisfyTimer);
  scheduler::cancel(pitEntry.m_stragglerTimer);
}

static inline void
insertNonceToDnl(DeadNonceList& dnl, const pit::Entry& pitEntry,
                 const pit::OutRecord& outRecord)
{
  dnl.add(pitEntry.getName(), outRecord.getLastNonce());
}

void
Forwarder::insertDeadNonceList(pit::Entry& pitEntry, bool isSatisfied,
                               ndn::optional<time::milliseconds> dataFreshnessPeriod, Face* upstream)
{
  // need Dead Nonce List insert?
  bool needDnl = true;
  if (isSatisfied) {
    BOOST_ASSERT(dataFreshnessPeriod);
    BOOST_ASSERT(*dataFreshnessPeriod >= time::milliseconds::zero());
    needDnl = static_cast<bool>(pitEntry.getInterest().getMustBeFresh()) &&
              *dataFreshnessPeriod < m_deadNonceList.getLifetime();
  }

  if (!needDnl) {
    return;
  }

  // Dead Nonce List insert
  if (upstream == nullptr) {
    // insert all outgoing Nonces
    const pit::OutRecordCollection& outRecords = pitEntry.getOutRecords();
    std::for_each(outRecords.begin(), outRecords.end(),
                  bind(&insertNonceToDnl, ref(m_deadNonceList), cref(pitEntry), _1));
  }
  else {
    // insert outgoing Nonce of a specific face
    pit::OutRecordCollection::iterator outRecord = pitEntry.getOutRecord(*upstream);
    if (outRecord != pitEntry.getOutRecords().end()) {
      m_deadNonceList.add(pitEntry.getName(), outRecord->getLastNonce());
    }
  }
}

} // namespace nfd
