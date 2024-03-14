//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Sink.h"
#include "Scheduler.h"

Define_Module(Sink);

void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");

}

void Sink::handleMessage(cMessage *msg)
{
   // int hp_packs = network->par("nrHP");
    simtime_t lifetime1 = simTime(); //nu e ok
    if (msg->arrivedOn("rxPackets",0)) {
        EV<<"SUNT AICI"<<endl;
        cModule *network = getParentModule();
            int nrHP = network->par("nrHP");
            EV<<"packs PRIO:"<<nrHP<<endl;
            double totalDelayHP = network->par("totalDelayHP"); //aici acelasi timp pe care l-am luat din coada
            EV<<"totalDelayHP:"<<totalDelayHP<<endl;
            EV<<"lifetime:"<<lifetime1.dbl()<<endl;
            double delay = lifetime1.dbl() - totalDelayHP; //aici nu e ok
            total += delay;
            EV<<"total:"<<total<<endl;
            if (nrHP == 10) {

                double averageDelayHP = total/nrHP;
                network->par("averageDelayHP").setDoubleValue(averageDelayHP);

                EV << "Average delay for high priority packets: " << (total / nrHP) << endl; //da mereu 0, you get the idea

                cMessage *start_FLC = new cMessage("start_FLC");
                send(start_FLC, "start_FLC");

                //aici resetam toate pachetele? sau doar cele de high prio
            }
        }

      simtime_t lifetime = simTime() - msg->getCreationTime();
      EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
      emit(lifetimeSignal, lifetime);
      delete msg;
}