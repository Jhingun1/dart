/*
 * Copyright (c) 2014-2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "dart/common/Subscription.h"
#include "dart/common/Subscriber.h"

namespace dart {
namespace common {

Subscriber::~Subscriber()
{
  std::set<const Subscription*>::iterator it = mSubscriptions.begin(),
                                          end = mSubscriptions.end();
  while( it != end )
    (*(it++))->removeSubscriber(this);
  // We do this tricky iterator method to deal with the fact that mSubscribers
  // will be changing as we go through the loop
}

//==============================================================================
void Subscriber::receiveNotification(const Subscription*, int)
{
  // Do nothing
}

//==============================================================================
void Subscriber::receiveDestructionNotification(
    const Subscription* _subscription)
{
  removeSubscription(_subscription);
  handleDestructionNotification(_subscription);
}

//==============================================================================
void Subscriber::handleDestructionNotification(const Subscription*)
{
  // Do nothing
}

//==============================================================================
void Subscriber::addSubscription(const Subscription* _subscription)
{
  if(nullptr == _subscription)
    return;

  if(mSubscriptions.find(_subscription) != mSubscriptions.end())
    return;

  mSubscriptions.insert(_subscription);
  _subscription->addSubscriber(this);
}

//==============================================================================
void Subscriber::removeSubscription(const Subscription* _subscription)
{
  if(nullptr == _subscription)
    return;

  if(mSubscriptions.find(_subscription) == mSubscriptions.end())
    return;

  mSubscriptions.erase(_subscription);
  _subscription->removeSubscriber(this);
}

//==============================================================================
void Subscriber::clearSubscriptions()
{
  std::set<const Subscription*>::iterator it = mSubscriptions.begin(),
                                          end = mSubscriptions.end();

  while(it != end)
    removeSubscription(*(it++));
}

} // namespace common
} // namespace dart